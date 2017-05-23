#include <stdlib.h>

#include <elf_int_uf_set.h>

//Set implemented using Union-Find algorithms.
//It's not supposed to have the intersection() operation.
typedef struct _ElfIntUFSet {
	int *id;    //Vector of each element's set
	int *weight;   //Vector of each element's weight (number of elements in its subset tree)
	int size;      //Number of elements
} ElfIntUFSet;

// Documented in header file.
ElfIntUFSet *elfIntUFSet_new(int size){
	ElfIntUFSet *set = malloc(sizeof(ElfIntUFSet));
	set->id = malloc(sizeof(int) * size);
	set->weight = malloc(sizeof(int) * size);
	set->size = size;
	for(size--; size >= 0; size--){
		set->id[size] = size; //Each element begins in its own set.
		set->weight[size] = 1;   //Each element begins being pointer only by itself. 
	}
	return set;
}

// Documented in header file.
void elfIntUFSet_destroy(ElfIntUFSet **set_p){
	ElfIntUFSet *set;
	if(set_p){
		set = *set_p;
		free(set->id);
		free(set->weight);
		free(set);
		*set_p = NULL;
	}
}

// Documented in header file.
int elfIntUFSet_size(ElfIntUFSet *set){
	return set->size;
}

// Documented in header file.
int elfIntUFSet_find(ElfIntUFSet *set, int elem){
	if( set->id[elem] == elem ){
		return elem;
	} else {
		// Strong path compression
		set->id[elem] = elfIntUFSet_find(set, set->id[elem]);
		return set->id[elem];
	}
}

// Documented in header file.
void elfIntUFSet_union(ElfIntUFSet *set, int e1, int e2){
	int parent1, parent2;
	parent1 = elfIntUFSet_find(set, e1);
	parent2 = elfIntUFSet_find(set, e2);

	// Weighted Quick Union
	if( parent1 != parent2 ){
		if( set->weight[parent1] > set->weight[parent2] ){
			set->id[parent2] = parent1;
			set->weight[parent1] += set->weight[parent2];
		} else {
			set->id[parent1] = parent2;
			set->weight[parent2] += set->weight[parent1];
		}
	}
}
