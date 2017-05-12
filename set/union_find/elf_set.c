

//Set implemented using Union-Find algorithms.
//It's not supposed to have the intersection() operation. If it exists, though, it won't be efficient.
typedef _ElfSet {
	int *array;    //Vector of each element's set
	int *weight;   //Vector of each element's weight (number of elements in its subset tree)
	int size;      //Number of elements
} ElfSet;

ElfSet *elfSet_new(int size){
	ElfSet *set = malloc(sizeof(ElfSet));
	set->array = malloc(sizeof(int) * size);
	set->weight = malloc(sizeof(int) * size);
	set->size = size;
	for(size--; size >= 0; size--){
		set->array[size] = size; //Each element begins in its own set.
		set->weight[size] = 1;   //Each element begins being pointer only by itself. 
	}
	return set;
}

void elfSet_destroy(ElfSet **set_p){
	ElfSet *set;
	if(set_p){
		set = *set_p;
		free(set->array);
		free(set->weight);
		free(set);
		*set_p = NULL;
	}
}

int elfSet_find(ElfSet *set, int elem){
	if( set->array[elem] == elem ){
		return elem;
	} else {
		return elfSet_find(set, set->array[elem]);
	}
}
