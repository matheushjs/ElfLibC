#include <stdio.h>
#include <stdlib.h>
#include <elf_graph_matrix.h>
#include <elf_graph_list.h>

#define CHECK_OP(OP,C1,C2) OP[0]==C1 && OP[1]==C2

typedef enum {
	SIMPLE,
	ORIENTED
} GraphType;

void handle_matrix_graph(int nvertix, int nedge, GraphType type){
	int i;
	ElfGraphMatrix *graph = elf_graph_matrix_new(nvertix, (bool) type == ORIENTED);

	for(i = 0; i < nedge; i++){
		int src, dest, wei;
		scanf("%d %d %d", &src, &dest, &wei);
		elf_graph_matrix_add_edge(graph, src, dest, wei);
	}

	char op[3];
	while(scanf("%2s", op) != EOF){
		if(CHECK_OP(op, 'I', 'G')){
			elf_graph_matrix_print_matrix(graph, stdout, '.', ' ', '\n');
		} else if(CHECK_OP(op, 'V', 'A')){
			int index;
			scanf("%d", &index);
			elf_graph_matrix_print_adjacent_vertex(graph, index, stdout);
		} else if(CHECK_OP(op, 'A', 'A')){
			int src, dest, wei;
			scanf("%d %d %d", &src, &dest, &wei);
			elf_graph_matrix_add_edge(graph, src, dest, wei);
		} else if(CHECK_OP(op, 'R', 'A')){
			int src, dest;
			scanf("%d %d", &src, &dest);
			elf_graph_matrix_remove_edge(graph, src, dest);
		} else if(CHECK_OP(op, 'I', 'T')){
			elf_graph_matrix_print_matrix_transposed(graph, stdout, '.', ' ', '\n');
		} else if(CHECK_OP(op, 'M', 'P')){
			elf_graph_matrix_print_edge_lowest_weight(graph, stdout);
		} else exit(EXIT_FAILURE);
	}

	elf_graph_matrix_destroy(&graph);
}

void handle_matrix_list(int nvertix, int nedge, GraphType type){
	int i;
	ElfGraphList *graph = elf_graph_list_new(nvertix, (bool) type == ORIENTED);

	for(i = 0; i < nedge; i++){
		int src, dest, wei;
		scanf("%d %d %d", &src, &dest, &wei);
		elf_graph_list_add_edge(graph, src, dest, wei);
	}

	char op[3];
	while(scanf("%2s", op) != EOF){
		if(CHECK_OP(op, 'I', 'G')){
			elf_graph_list_print_list(graph, stdout);
		} else if(CHECK_OP(op, 'V', 'A')){
			int index;
			scanf("%d", &index);
			elf_graph_list_print_adjacent_vertex(graph, index, stdout);
		} else if(CHECK_OP(op, 'A', 'A')){
			int src, dest, wei;
			scanf("%d %d %d", &src, &dest, &wei);
			elf_graph_list_add_edge(graph, src, dest, wei);
		} else if(CHECK_OP(op, 'R', 'A')){
			int src, dest;
			scanf("%d %d", &src, &dest);
			elf_graph_list_remove_edge(graph, src, dest);
		} else if(CHECK_OP(op, 'I', 'T')){
			elf_graph_list_print_matrix_transposed(graph, stdout);
		} else if(CHECK_OP(op, 'M', 'P')){
			elf_graph_list_print_edge_lowest_weight(graph, stdout);
		} else exit(EXIT_FAILURE);
	}

	elf_graph_list_destroy(&graph);
}

int main(int argc, char *argv[]){
	char type, oriented;
	int nvertix, nedge;
	GraphType graph_type;

	scanf("%c %c %d %d", &oriented, &type, &nvertix, &nedge);
	graph_type = oriented == 'D' ? ORIENTED : SIMPLE;

	if(type == 'M') handle_matrix_graph(nvertix, nedge, graph_type);
	else handle_matrix_list(nvertix, nedge, graph_type);

	return 0;
}
