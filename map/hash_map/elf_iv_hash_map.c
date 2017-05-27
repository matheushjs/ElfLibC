#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
	const void *value;
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

ElfIVHashMap *elfIVHashMap_new_withAmount(int expectedAmount){
	ElfIVHashMap *map;
	map = malloc(sizeof(ElfIVHashMap));
	map->count = 0;
	map->level = next_capacity_level(expectedAmount);
	map->baseLevel = map->level;
	map->buckets = malloc(sizeof(ElfList *) * g_capacity_levels[map->level]);
	
	int i, n;
	n = g_capacity_levels[map->level];
	for(i = 0; i < n; i++)
		map->buckets[i] = elfList_new_withEqual(
				(bool (*)(void*,void*)) node_greaterThan,
				(bool (*)(void*,void*)) node_equal);

	return map;
}

ElfIVHashMap *elfIVHashMap_new(){
	return elfIVHashMap_new_withAmount(0);
}


