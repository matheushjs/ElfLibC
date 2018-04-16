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
#ifndef _ELF_GRAPH_MATRIX_H_
#define _ELF_GRAPH_MATRIX_H_

struct ElfGraphMatrix;
typedef struct ElfGraphMatrix ElfGraphMatrix;

#include <stdio.h>
#include <stdbool.h>

ElfGraphMatrix *elfGraph_matrix_new(int N, bool oriented);
void elfGraph_matrix_destroy(ElfGraphMatrix **graph);
void elfGraph_matrix_add_edge(ElfGraphMatrix *graph, int src, int dest, int weight);
void elfGraph_matrix_remove_edge(ElfGraphMatrix *graph, int src, int dest);
void elfGraph_matrix_print_matrix(const ElfGraphMatrix *graph, FILE *fp, char failChar, char sep, char end);
void elfGraph_matrix_print_adjacent_vertex(const ElfGraphMatrix *graph, int subjectVertex, FILE *fp);
void elfGraph_matrix_print_edge_lowest_weight(const ElfGraphMatrix *graph, FILE *fp);
void elfGraph_matrix_print_matrix_transposed(ElfGraphMatrix *graph, FILE *fp, char failChar, char sep, char end);

#endif
