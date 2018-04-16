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
#ifndef _AVLTREE_H
#define _AVLTREE_H

typedef struct tree_struct tree_t;

//printing function may be given as NULL.
tree_t *tree_alloc(bool (*higher)(const void*,const void*), bool (*equal)(const void*,const void*),
		void (*dfree)(void*), void (*print)(const void *));
void tree_print_function(tree_t *tree, void (*print)(const void *));
void tree_insert(tree_t *, void *);
bool tree_search(const tree_t *, const void *);
bool tree_remove(tree_t *, const void *);
void tree_destroy(tree_t **);
void tree_print_inorder(const tree_t *);
void tree_print_postorder(const tree_t *);
void tree_print_preorder(const tree_t *);
void *tree_highest(const tree_t *);
void *tree_lowest(const tree_t *);
void *tree_successor(const tree_t *, void *);
void *tree_predecessor(const tree_t *, void *);

#endif
