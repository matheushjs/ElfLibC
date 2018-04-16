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
#ifndef _SLIST_H
#define _SLIST_H

//The list will store data_t entirely inside each node in the list.
typedef int data_t;
typedef struct slist_struct slist_t;
typedef enum { FALSE, TRUE } bool;

//cmp_data() should yield TRUE when comparing cmp_data(first_node, last_node);
void slist_set_cmp_data(bool (*)(data_t *, data_t *));
void slist_set_equal_data(bool (*)(data_t *, data_t *));
void slist_set_print_data(void (*)(data_t *));

//Use with caution. Leave as default functions, if possible.
//free_data should free all memory blocks allocated by copy_data.
void slist_set_free_data(void (*)(data_t *));
void slist_set_copy_data(void (*)(data_t *dest, data_t *src));

slist_t *slist_alloc();
void slist_destroy(slist_t **);
bool slist_is_empty(slist_t *);
void slist_insert(slist_t *, data_t *);
data_t *slist_search(slist_t *, data_t *);

void slist_remove_key(slist_t *, data_t *);
void slist_print(slist_t *);
int slist_size(slist_t *);

#endif
