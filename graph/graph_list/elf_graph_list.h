#ifndef _ELF_LIST_MATRIX_H_
#define _ELF_LIST_MATRIX_H_

typedef struct _ElfGraphList ElfGraphList;

#include <stdio.h>
#include <stdbool.h>

ElfGraphList *elf_graph_list_new(int N, bool oriented);
void elf_graph_list_destroy(ElfGraphList **graph);
void elf_graph_list_add_edge(ElfGraphList *graph, int src, int dest, int weight);
void elf_graph_list_remove_edge(ElfGraphList *graph, int src, int dest);
void elf_graph_list_print_list(const ElfGraphList *graph, FILE *fp);
void elf_graph_list_print_adjacent_vertex(const ElfGraphList *graph, int subjectVertex, FILE *fp);
void elf_graph_list_print_edge_lowest_weight(const ElfGraphList *graph, FILE *fp);
void elf_graph_list_print_matrix_transposed(ElfGraphList *graph, FILE *fp);

#endif
