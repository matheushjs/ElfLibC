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
#include <queue.h>

//front		    back
//n1 -> n2 -> n3 -> n4
typedef struct node_struct node_t;
struct node_struct{
	void *key;
	node_t *back;
};

struct queue_struct {
	node_t *front, *back;
};

queue_t *queue_alloc(){
	return (queue_t *) calloc(sizeof(queue_t), 1);
}

void queue_insert(queue_t *queue, void *p){
	node_t *node = calloc(sizeof(node_t), 1);
	node->key = p;
	if(queue->back)
		queue->back->back = node;
	else
		queue->front = node;
	queue->back = node;
}

void *queue_get(queue_t *queue){
	node_t *node;
	void *ret;
	node = queue->front;
	if(!node) return NULL;
	ret = node->key;
	queue->front = node->back;
	free(node);
	if(!queue->front) queue->back = NULL;
	return ret;
}

void queue_destroy(queue_t **queue_p){
	node_t *aux, *node;
	queue_t *queue = *queue_p;

	if(!queue) return;
	node = queue->front;
	while(node){
		aux = node;
		node = node->back;
		free(aux);
	}
	free(queue);
	*queue_p = NULL;
};
