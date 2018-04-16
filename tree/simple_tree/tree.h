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
#ifndef _TREE_H
#define _TREE_H

/* Set the tree data_type here	*/
/* The declaration of the data_type should be in this file also	*/
typedef int data_t;

typedef enum {FALSE, TRUE} bool;
typedef struct tree_struct tree_t;

tree_t * tree_alloc();
void tree_destroy(tree_t **);
void tree_destroy_f(tree_t **, void (*)(data_t));
bool tree_is_empty(tree_t *);
bool tree_reached_leaf(tree_t *);
data_t *tree_get_data(tree_t *);
bool tree_change_data(tree_t *, data_t *);
bool tree_append_data(tree_t *, data_t *, bool);
bool tree_descend(tree_t *, bool);
bool tree_rewind(tree_t *);
tree_t *tree_merge(tree_t **, tree_t **, data_t *);
void tree_print_curr(tree_t *, void (*)(data_t *));
void tree_print(tree_t *, void (*)(data_t *));

#endif
