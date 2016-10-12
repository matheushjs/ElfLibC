#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <slist.h>
#include <time.h>

#define die(X) fprintf(stderr, "%s:%d:%s(): ERROR: %s", __FILE__, __LINE__, __func__, X "\n"), exit(EXIT_FAILURE)

typedef struct mnode_struct mnode_t;
typedef struct pnode_struct pnode_t;

//pointer node.
struct pnode_struct {
	mnode_t *target;
	pnode_t *next;
	//pnode_t *up;
	pnode_t *down;
};

//main node.
struct mnode_struct {
	data_t data;
	mnode_t *next;
	pnode_t *up;
};

//root, main structure for the skiplist.
struct slist_struct {
	mnode_t *first;
	pnode_t **tower;
	int levels;
	int size;
};


static void (*free_data)(data_t *) = NULL;
static void (*copy_data)(data_t *, data_t *) = NULL;
static bool (*cmp_data)(data_t *, data_t *) = NULL;
static bool (*equal_data)(data_t *, data_t *) = NULL;
static void (*print_data)(data_t *) = NULL;


/*
 * General functions
 */

static
bool chance(){
	static unsigned int next = 1;
	next = next * 11227 + 423 + time(NULL); 
	srand(next);
	return rand()%2 == 1 ? TRUE : FALSE;
}


/*
 * Node functions
 */

static
mnode_t *mnode_alloc(){
	return (mnode_t *) calloc(sizeof(mnode_t), 1);
}

static
pnode_t *pnode_alloc(){
	return (pnode_t *) calloc(sizeof(pnode_t), 1);
}

static
void mnode_destroy(mnode_t **node){
	if(*node){
		if(free_data) free_data(&(*node)->data);
		free(*node);
		*node = NULL;
	}
}

static
void pnode_destroy(pnode_t **node){
	if(*node){
		free(*node);
		*node = NULL;
	}
}

static
mnode_t *mnode_create(data_t *data){
	mnode_t *node = mnode_alloc();
	if(!copy_data) memcpy(&node->data, data, sizeof(data_t));
	else copy_data(&node->data, data);
	return node;
}

/*
 * SkipList auxiliary functions
 */

//Generates as many pnode_t with target 'target' as needed.
//Adds each new pnode_t after the pnode_t in the given level of the matrix 'prev_tower'.
static
void slist_add_pnodes(slist_t *list, pnode_t **prev_tower, mnode_t *target){
	int i, counter, tsize;
	pnode_t *down = NULL, *node;
	
	counter = 0;
	tsize = prev_tower ? list->levels : 0;	//stores size of prev_tower.
	while(chance()) counter++;
	//printf("\tDEBUG: Checking if realloc() is needed\n");
	if(counter > list->levels){
		//printf("\tDEBUG: It was needed. New size: %d\n", counter);
		list->tower = (pnode_t **) realloc(list->tower, sizeof(pnode_t *) * counter);
		list->levels = counter;
	}
//counter = 1, tsize = 0;
	for(i = 0; i < counter; i++){
		//printf("\tDEBUG: Creating pointer node\n");
		node = pnode_alloc();
		node->target = target;
		node->down = down;
	
		//printf("\tDEBUG: Adding node\n");
		if(i >= tsize){
			//printf("\tDEBUG: Adding to a new pointer in list->tower\n");
			node->next = NULL;
			list->tower[i] = node;
			//printf("\tDEBUG: Added.\n");
		} else if(prev_tower[i]){
			//printf("\tDEBUG: Adding to a pointer in prev_tower\n");
			node->next = prev_tower[i]->next;
			prev_tower[i]->next = node;
		} else {
			//printf("\tDEBUG: Adding to an old pointer in list->tower\n");
			node->next = list->tower[i];
			list->tower[i] = node;
		}
		//printf("\tDEBUG: Addition completed\n");

		down = node;
	}
}


/*
 * SkipList main functions
 */

void slist_set_free_data(void (*free_func)(data_t *)){ free_data = free_func; }
void slist_set_copy_data(void (*copy)(data_t *, data_t *)){ copy_data = copy; }
void slist_set_cmp_data(bool (*cmp)(data_t *, data_t *)){ cmp_data = cmp; }
void slist_set_equal_data(bool (*equal)(data_t *, data_t *)){ equal_data = equal; }
void slist_set_print_data(void (*print)(data_t *)){ print_data = print; }

slist_t *slist_alloc(){
	return (slist_t *) calloc(sizeof(slist_t), 1);
}

bool slist_is_empty(slist_t *list){
	return list->size ? FALSE : TRUE;
}

void slist_insert(slist_t *list, data_t *data){
	int i;
	mnode_t *node, *mcurr, *mprev;
	pnode_t *pprev, *pcurr, **prev_tower;

	if(!list) return;
	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }

	prev_tower = (pnode_t **) malloc(sizeof(pnode_t *) * list->levels);

	//Navigate through the skip list, storing in prev_tower all nodes that are located
	//immediately before where the node containing 'data' would be if it existed in the
	//same level in the tower.
	pprev = NULL;
	//printf("DEBUG: Beginning to navigate the tower\n");
	for(i = list->levels - 1; i >= 0; i--){
		//printf("DEBUG: Finding initial point\n");
		if(pprev){
			pprev = pprev->down;
			pcurr = pprev->next;
		} else pcurr = list->tower[i];
		//printf("DEBUG: Finding previous point\n");
		while(pcurr && cmp_data(&pcurr->target->data, data))
			pcurr = (pprev = pcurr, pcurr->next);
		prev_tower[i] = pprev;
	}
	//printf("DEBUG: Finding initial point in floor\n");
	if(pprev){
		mprev = pprev->target;
		mcurr = mprev->next;
	} else {
		mprev = NULL;
		mcurr = list->first;
	}

	//printf("DEBUG: Beginning to navigate the floor\n");
	while(mcurr && cmp_data(&mcurr->data, data))
		mcurr = (mprev = mcurr, mcurr->next);

	//printf("DEBUG: Checking if data exists\n");
	//If data exists in the list already.
	if(mcurr && equal_data(&mcurr->data, data)){ free(prev_tower); return; }

	//printf("DEBUG: Adding node\n");
	node = mnode_create(data);
	if(mprev){
		node->next = mcurr;
		mprev->next = node;
		slist_add_pnodes(list, prev_tower, node);
	} else {
		node->next = list->first;
		list->first = node;
		slist_add_pnodes(list, NULL, node);
	}
	//printf("DEBUG: Insertion completed\n");

	free(prev_tower);
	list->size++;
}

void slist_print(slist_t *list){
	if(!list) return;
	if(!print_data){ fprintf(stderr, "Print function is not set\n"); return; }

/**/
	pnode_t *pcurr;
	int i;
	printf("list (levels, size) = (%d, %d)\n", list->levels, list->size);
	for(i = list->levels - 1; i >= 0; i--){
		pcurr = list->tower[i];
		while(pcurr){
			print_data(&pcurr->target->data);
			printf(" ");
			pcurr = pcurr->next;
		}
		printf("\n");
	}
/**/

	mnode_t *curr = list->first;
	while(curr){
		print_data(&curr->data);
		printf(" ");
		curr = curr->next;
	}

	/**/printf("\n");
}

/*
//Returns NULL if element 'key' isn't found.
//If found, stores the element's index in 'index'
data_t *list_search(list_t *list, data_t *key, int *index){
	int i = 0;
	node_t *curr;

	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }
	if(list){
		curr = list->first;
		while(curr && cmp_data(&curr->data, key))
			curr = (i++, curr->next);

		if(curr && equal_data(&curr->data, key)){
			if(index) *index = i-1;
			return &curr->data;
		}
	}

	if(index) *index = -1;
	return NULL;
}

data_t *list_retrieve(list_t *list, int index){
	if(!list || index < 0 || index > list->size) return NULL;
	node_t *node;
	node = node_walk(list->first, index);
	return &node->data;
}

void list_remove(list_t *list, int index){
	if(!list) return;
	if(list_is_empty(list)) return;
	
	node_t *node, *aux;
	if(!index){
		aux = list->first->next;
		node_destroy(&list->first);
		list->first = aux;
	} else {
		node = node_walk(list->first, index-1);
		if(!node) return;	//out of bounds
		aux = node->next;
		node->next = aux->next;
		node_destroy(&aux);
	}
	list->size--;
}

void list_remove_key(list_t *list, data_t *key){
	node_t *prev = NULL, *curr;

	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }
	if(!list || list_is_empty(list)) return;
	
	curr = list->first;
	while(curr && cmp_data(&curr->data, key))
		curr = (prev = curr, curr->next);

	if(curr && equal_data(&curr->data, key)){
		if(prev) prev->next = curr->next;
		else list->first = curr->next;
		node_destroy(&curr);
		list->size--;
	}
}

void list_print(list_t *list){
	if(!list) return;
	if(!print_data){ fprintf(stderr, "Print function is not set\n"); return; }
	node_t *curr = list->first;
	while(curr){
		print_data(&curr->data);
		printf(" ");
		curr = curr->next;
	}
}

int list_size(list_t *list){
	if(!list) return -1;
	return list->size;
}
*/
