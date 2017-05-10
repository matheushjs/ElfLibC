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
