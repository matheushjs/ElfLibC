#ifndef _ELF_GRAPH_MATRIX_H_
#define _ELF_GRAPH_MATRIX_H_

struct ElfGraphMatrix;
typedef struct ElfGraphMatrix ElfGraphMatrix;

#include <stdio.h>
#include <stdbool.h>

ElfGraphMatrix *elf_graph_matrix_new(int N, bool oriented);
void elf_graph_matrix_destroy(ElfGraphMatrix **graph);
void elf_graph_matrix_add_edge(ElfGraphMatrix *graph, int src, int dest, int weight);
void elf_graph_matrix_remove_edge(ElfGraphMatrix *graph, int src, int dest);
void elf_graph_matrix_print_matrix(const ElfGraphMatrix *graph, FILE *fp, char failChar, char sep, char end);
void elf_graph_matrix_print_adjacent_vertex(const ElfGraphMatrix *graph, int subjectVertex, FILE *fp);
void elf_graph_matrix_print_edge_lowest_weight(const ElfGraphMatrix *graph, FILE *fp);
void elf_graph_matrix_print_matrix_transposed(ElfGraphMatrix *graph, FILE *fp, char failChar, char sep, char end);

#endif
