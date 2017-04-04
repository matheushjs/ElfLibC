#ifndef _ELF_LIST_MATRIX_H_
#define _ELF_LIST_MATRIX_H_

typedef struct _ElfGraph ElfGraph;

#include <stdio.h>
#include <stdbool.h>

ElfGraph *elf_graph_new(int N, bool oriented);
void elf_graph_destroy(ElfGraph **graph);
void elf_graph_add_edge(ElfGraph *graph, int src, int dest, int weight);
void elf_graph_remove_edge(ElfGraph *graph, int src, int dest);
void elf_graph_print_list(const ElfGraph *graph, FILE *fp);
void elf_graph_print_adjacent_vertex(const ElfGraph *graph, int subjectVertex, FILE *fp);
void elf_graph_print_edge_lowest_weight(const ElfGraph *graph, FILE *fp);
void elf_graph_print_matrix_transposed(ElfGraph *graph, FILE *fp);

#endif
