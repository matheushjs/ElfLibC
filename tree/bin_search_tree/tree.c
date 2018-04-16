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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <tree.h>
#include <queue.h>

typedef struct node_struct node_t;
struct node_struct {
	void *data;
	node_t *left, *right;
};

struct tree_struct {
	node_t *root;
	
	bool (*data_higher)(void *, void *);
	bool (*data_lower)(void *, void *);
	void (*data_free)(void *);
	void (*data_print)(void *);
};

tree_t *tree_alloc(bool (*higher)(void *, void *), bool (*lower)(void *, void *),
		void (*free)(void *), void (*print)(void *)){
	tree_t *tree = (tree_t *) calloc(sizeof(tree_t), 1);
	tree->data_higher = higher;
	tree->data_lower = lower;
	tree->data_free = free;
	tree->data_print = print;
	return tree;
}

bool tree_insert(tree_t *tree, void *key){
	node_t *aux, **curr = &tree->root;

	while(*curr != NULL){
		aux = *curr;
		if(tree->data_lower(key, aux->data))
			curr = &aux->left;
		else if(tree->data_higher(key, aux->data))
			curr = &aux->right;
		else return false;
	}

	*curr = calloc(sizeof(node_t), 1);
	(*curr)->data = key;
	return true;
}

bool tree_search(tree_t *tree, void *key){
	node_t *cur = tree->root;
	while(cur != NULL){
		if(tree->data_lower(key, cur->data))
			cur = cur->left;
		else if(tree->data_higher(key, cur->data))
			cur = cur->right;
		else return true;
	}
	return false;
}

static
node_t **find_min(node_t **node){
	if(!*node) return NULL;
	while((*node)->left)
		node = &(*node)->left;
	return node;
}

bool tree_remove(tree_t *tree, void *key){
	node_t *aux, **cur = &tree->root;
	while(*cur != NULL){
		if(tree->data_lower(key, (*cur)->data))
			cur = &(*cur)->left;
		else if(tree->data_higher(key, (*cur)->data))
			cur = &(*cur)->right;
		else break;
	}

	if(!*cur) return false;
	
	node_t **min;
	if(!(*cur)->right){
		aux = *cur;
		*cur = (*cur)->left;
		free(aux);
	} else if(!(*cur)->left){
		aux = *cur;
		*cur = (*cur)->right;
		free(aux);
	} else {
		min = find_min(&(*cur)->right);
		(*cur)->data = (*min)->data;
		aux = *min;
		*min = (*min)->right;
		free(aux);
	}

	return true;
}

static
void print_preord(node_t *n, void (*print)(void *)){
	if(!n) return;
	print(n->data);
	print_preord(n->left, print);
	print_preord(n->right, print);
}

static
void print_inord(node_t *n, void (*print)(void *)){
	if(!n) return;
	print_inord(n->left, print);
	print(n->data);
	print_inord(n->right, print);
}

static
void print_posord(node_t *n, void (*print)(void *)){
	if(!n) return;
	print_posord(n->left, print);
	print_posord(n->right, print);
	print(n->data);
}

static
void print_breadth(tree_t *n){
	queue_t *q;
	node_t *node;

	q = queue_alloc();

	queue_insert(q, (void *) n->root);

	while(1){
		node = queue_get(q);
		if(!node) break;
		n->data_print(node->data);
		if(node->left) queue_insert(q, node->left);
		if(node->right) queue_insert(q, node->right);
	}

	queue_destroy(&q);
}

void tree_print(tree_t *tree, order_t order){
	switch(order){
		case PREORD:
			print_preord(tree->root, tree->data_print);
			break;
		case INORD:
			print_inord(tree->root, tree->data_print);
			break;
		case POSORD:
			print_posord(tree->root, tree->data_print);
			break;
		case BREADTH:
			print_breadth(tree);
			break;
	}
}

static
void destroy_pre(node_t *node){
	if(!node) return;
	destroy_pre(node->right);
	destroy_pre(node->left);
	free(node);
}

void tree_destroy(tree_t **tree_p){
	tree_t *tree = *tree_p;
	if(!tree) return;
	destroy_pre(tree->root);
	free(tree);
	*tree_p = NULL;
}
