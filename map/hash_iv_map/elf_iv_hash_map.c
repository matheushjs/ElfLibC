/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <elf_list.h>
#include <elf_iv_hash_map.h>

static int g_capacity_levels[] = {
7, 13, 29, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739,
6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189,
805306457, 1610612741, 2147000041 };

/*
 * Static struct-unrelated functions
 */

// Given a 'capacity', finds the next capacity level that can hold it.
static inline
int next_capacity_level(int capacity){
	int i;
	static int n = sizeof(g_capacity_levels) / sizeof(int) - 1;

	for(i = 0; i < n; i++)
		if(g_capacity_levels[i] >= capacity) break;

	return i;
}

/*
 * Node Structure and Functions
 */

typedef struct _Node {
	int key;
	void *value;
} Node;

bool node_greaterThan(const Node *me, const Node *other){
	return me->key > other->key ? true : false;
}

bool node_equal(const Node *me, const Node *other){
	return me->key == other->key ? true : false;
}

/*
 * HashMap Structure and Functions
 */

typedef struct _ElfIVHashMap {
	ElfList **buckets;
	int count;      //Element counter
	int level;      //Capacity level
	int baseLevel;  //Base capacity level, below which the Hash cannot shrink.
} ElfIVHashMap;

// Returns a vector of buckets, each bucket initialized.
static inline
ElfList **buckets_getInitialized(int size){
	ElfList **buckets;
	int i;

	buckets = malloc(sizeof(ElfList *) * size);
	for(i = 0; i < size; i++){
		buckets[i] = elfList_new_withEqual(
			(bool (*)(void*,void*)) node_greaterThan,
			(bool (*)(void*,void*)) node_equal);
	}

	return buckets;
}

// For use anywhere where nodes need to be freed.
// Function to be run on the void* value.
static void (*g_nodeFree_freeFunc)(void *) = NULL; 
// Function that frees the node and applies freeFunc in each value stored in the nodes.
static
void g_nodeFree(void *node){
	if(g_nodeFree_freeFunc != NULL){
		g_nodeFree_freeFunc( ((Node *) node)->value);
	}
	free(node);
}

// Deallocates all memory for the buckets.
// if 'free_nodes' is true, every Node* passes through a call to free().
// if 'value_free' is not NULL, every value stored in the nodes are passed through value_free().
// if 'value_free' is given, but 'free_nodes' is false, 'value_free' has no effect.
static inline
void buckets_destroy(ElfList **buckets, int size, bool free_nodes, void (*value_free)(void *)){
	int i;

	g_nodeFree_freeFunc = value_free;

	for(i = 0; i < size; i++){
		if(free_nodes)
			elfList_destroyF(&buckets[i], g_nodeFree);
		else
			elfList_destroy(&buckets[i]);
	}
	free(buckets);

	g_nodeFree_freeFunc = NULL;
}

// Documented in header file.
ElfIVHashMap *elfIVHashMap_new_withSize(int expectedSize){
	ElfIVHashMap *map;
	map = malloc(sizeof(ElfIVHashMap));
	map->count = 0;
	
	// Set initial level
	// Multiplies expected size because we want it to be 66% of the total capacity.
	map->level = next_capacity_level((int) ( (1/0.66) * expectedSize));

	// Make the initial level the base level.
	map->baseLevel = map->level;

	// Get the buckets
	map->buckets = buckets_getInitialized(g_capacity_levels[map->level]);
	
	return map;
}

// Documented in header file.
ElfIVHashMap *elfIVHashMap_new(){
	return elfIVHashMap_new_withSize(0);
}

// Documented in header file.
void elfIVHashMap_destroy_F(ElfIVHashMap **elf_p, void (*func)(void *data)){
	ElfIVHashMap *elf = *elf_p;

	if(elf){
		buckets_destroy(elf->buckets, g_capacity_levels[elf->level], true, func);
		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
void elfIVHashMap_destroy(ElfIVHashMap **elf_p){
	elfIVHashMap_destroy_F(elf_p, NULL);
}

// For use in elfIVHashMap_traverse
void (*g_traverse_func)(int key, void*value) = NULL;
// For use in elfIVHashMap_traverse
static
void traverse_consumer(Node *node){
	g_traverse_func(node->key, node->value);
}

// Documented in headers file.
void elfIVHashMap_traverse(ElfIVHashMap *elf, void(*func)(int key, void* value)){
	int i, size = g_capacity_levels[elf->level];

	g_traverse_func = func;
	for(i = 0; i < size; i++)
		elfList_traverse(elf->buckets[i], (void(*)(void*)) traverse_consumer);
}

// Hashes 'key' into any integer number from 0 to size-1.
static inline
int hashing(int key, int size){
	static double phi;
	static char lock = 0;
	double x;

	if(!lock){
		phi = (sqrt(5) - 1) / (double) 2;
		lock = 1;
	}

	x = phi * abs(key);
	x -= floor(x);

	return (int) size * x;
}

// For use in _shrink() and _grow() functions
static int g_resize_newSize; // The new size of the buckets being regenerated.
static ElfList **g_resize_buckets = NULL; // Pointer to the new buckets.
static // Adds node 'node' to g_resize_buckets.
void resize_addNode(Node *node){
	int hash;
	ElfList *list;

	hash = hashing(node->key, g_resize_newSize);
	list = g_resize_buckets[hash];
	elfList_insert(list, node);
}

// Verifies the current load factor of the hashMap and shrinks it if needed.
// The hashMap is shrunk only at a factor below 20%.
// The hashMap is not shrunk below map->baseLevel.
static
void elfIVHashMap_shrink(ElfIVHashMap *elf){
	const static double lowbound = 0.2;
	int newLevel, capacity, i, n;
	double lambda;

	// Check load factor
	capacity = g_capacity_levels[elf->level];
	lambda = elf->count / (double) capacity;
	if(lambda > lowbound) return;

	// Then we need to shrink the hashMap.
	// Begin by checking the baseLevel
	newLevel = elf->level - 1;
	if(newLevel < elf->baseLevel) return;

	// Get new capacity and set up global static variables
	capacity = g_capacity_levels[newLevel];
	g_resize_newSize = capacity;
	g_resize_buckets = buckets_getInitialized(capacity);
	
	// Transfer nodes from old buckets to new ones
	n = g_capacity_levels[elf->level];
	for(i = 0; i < n; i++){
		elfList_traverse(elf->buckets[i], (void(*)(void*)) resize_addNode);
	}

	// Swap and free due variables. Nodes are not freed.
	buckets_destroy(elf->buckets, n, false, NULL);
	elf->buckets = g_resize_buckets;
	elf->level = newLevel;
	g_resize_buckets = NULL;
}

// Verifies the current load factor of the hashMap and grows it if needed.
// The hashMap grows only at a factor above 66%.
static
void elfIVHashMap_grow(ElfIVHashMap *elf){
	const static double upbound = 2 / (double)3;
	int newLevel, capacity, i, n;
	double lambda;

	// Check load factor
	capacity = g_capacity_levels[elf->level];
	lambda = elf->count / (double) capacity;
	if(lambda < upbound) return;
	
	// Then we need to grow the hashMap.
	
	// Get new capacity and set up global static variables
	newLevel = elf->level + 1;
	capacity = g_capacity_levels[newLevel];
	g_resize_newSize = capacity;
	g_resize_buckets = buckets_getInitialized(capacity);

	// Transfer nodes
	n = g_capacity_levels[elf->level];
	for(i = 0; i < n; i++){
		elfList_traverse(elf->buckets[i], (void(*)(void*)) resize_addNode);
	}

	// Swap and free due variables. Nodes are not freed.
	buckets_destroy(elf->buckets, n, false, NULL);
	elf->buckets = g_resize_buckets;
	elf->level = newLevel;
	g_resize_buckets = NULL;
}

// Documented in header file.
void *elfIVHashMap_put(ElfIVHashMap *elf, int key, void *value){
	int hash, i;
	ElfList *list;
	Node *node, *aux;
	void *retvalue;
	bool retval;
		
	hash = hashing(key, g_capacity_levels[elf->level]);
	list = elf->buckets[hash];

	node = malloc(sizeof(Node));
	node->key = key;
	node->value = value;

	retval = elfList_insertUnique(list, node);

	// Checks if element was not inserted
	// Our policy is to replace the existing node with the new one
	if(!retval){
		i = elfList_indexOf(list, node);
		aux = elfList_removeIndex(list, i);
		retvalue = aux->value;
		free(aux);
		elfList_insertUnique(list, node);
	} else {
		retvalue = NULL;
		(elf->count)++;
		elfIVHashMap_grow(elf);
	}

	return retvalue;
}

// Documented in header file.
void *elfIVHashMap_remove(ElfIVHashMap *elf, int key){
	int hash, idx;
	ElfList *list;
	Node *node;
	void *value;

	hash = hashing(key, g_capacity_levels[elf->level]);
	list = elf->buckets[hash];

	node = malloc(sizeof(Node));
	node->key = key;

	// Search node
	idx = elfList_indexOf(list, node);
	free(node);

	// Checks if elements does not exist.
	if(idx < 0) return NULL;
	
	// Remove node
	node = elfList_removeIndex(list, idx);
	
	// Get its value
	value = node->value;
	free(node);

	(elf->count)--;
	elfIVHashMap_shrink(elf);

	return value;
}

// Documented in header file.
void *elfIVHashMap_get(const ElfIVHashMap *elf, int key){
	int hash, idx;
	ElfList *list;
	Node node, *node_p;
	void *value;

	hash = hashing(key, g_capacity_levels[elf->level]);
	list = elf->buckets[hash];

	node.key = key;
	idx = elfList_indexOf(list, &node);
	
	// Check element existence
	if(idx < 0) return NULL;

	// Get the value
	node_p = elfList_get(list, idx);
	value = node_p->value;

	return value;
}

// Documented in header file.
int elfIVHashMap_size(const ElfIVHashMap *elf){
	return elf->count;
}

// Documented in header file.
int elfIVHashMap_capacity(const ElfIVHashMap *elf){
	return g_capacity_levels[elf->level];
}

// Documented in header file.
double elfIVHashMap_loadFactor(const ElfIVHashMap *elf){
	return elfIVHashMap_size(elf) / (double) elfIVHashMap_capacity(elf);
}

// Documented in header file.
void elfIVHashMap_printBucketsStatus(const ElfIVHashMap *elf){
	int i, n;

	n = elfIVHashMap_capacity(elf);
	for(i = 0; i < n; i++){
		printf("%d ", elfList_size(elf->buckets[i]));
	}
	printf("\n");
}
