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
	//Stores in tsize the size of prev_tower.
	tsize = prev_tower ? list->levels : 0;	
	while(chance()) counter++;
	//Check if realloc() is needed
	if(counter > list->levels){
		list->tower = (pnode_t **) realloc(list->tower, sizeof(pnode_t *) * counter);
		list->levels = counter;
	}

	for(i = 0; i < counter; i++){
		node = pnode_alloc();
		node->target = target;
		node->down = down;
	
		if(i >= tsize){
			node->next = NULL;
			list->tower[i] = node;
		} else if(prev_tower[i]){
			node->next = prev_tower[i]->next;
			prev_tower[i]->next = node;
		} else {
			node->next = list->tower[i];
			list->tower[i] = node;
		}

		down = node;
	}
}

//Navigate through the list->tower, storing in prev_tower all nodes that are located
//immediately before where the node containing 'data' would be if it existed in the
//same level in the tower.
//	Returns the prev_tower.
//	Stores in 'mprev' the furthest mnode_t whose data is 'lower' than the given data
//		that we managed to find while building the prev_tower.
static
pnode_t **slist_build_prev_tower(slist_t *list, data_t *data, mnode_t **mprev){
	int i;
	pnode_t *pprev, *pcurr, **prev_tower;

	prev_tower = (pnode_t **) malloc(sizeof(pnode_t *) * list->levels);
	pprev = NULL;
	for(i = list->levels - 1; i >= 0; i--){
		//Finding initial point in current level.
		if(pprev){
			pprev = pprev->down;
			pcurr = pprev->next;
		} else pcurr = list->tower[i];

		while(pcurr && cmp_data(&pcurr->target->data, data))
			pcurr = (pprev = pcurr, pcurr->next);

		prev_tower[i] = pprev;
	}

	if(mprev) *mprev = pprev ? pprev->target : NULL;
	return prev_tower;
}

//Change list->levels and list->tower to ignore floors that are empty.
static
void slist_fix_empty_levels(slist_t *list){
	int i, count;
	count = 0;
	for(i = list->levels - 1; i >= 0 && list->tower[i] == NULL; i--) count++;
	list->levels -= count;
	list->tower = (pnode_t **) realloc(list->tower, sizeof(pnode_t *) * list->levels);
	if(!list->levels) list->tower = NULL;
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

void slist_destroy(slist_t **list){
	int i;
	slist_t *l = *list;
	pnode_t *pcurr, *pnext;
	mnode_t *curr, *next;

	if(!l) return;
	
	for(i = l->levels - 1; i >= 0; i--){
		pcurr = l->tower[i];
		while(pcurr){
			pnext = pcurr->next;
			pnode_destroy(&pcurr);
			pcurr = pnext;
		}
	}

	curr = l->first;
	while(curr){
		next = curr->next;
		mnode_destroy(&curr);
		curr = next;
	}
	
	free(l->tower);
	free(l);
	*list = NULL;
}

bool slist_is_empty(slist_t *list){
	return list->size ? FALSE : TRUE;
}

void slist_insert(slist_t *list, data_t *data){
	mnode_t *node, *mcurr, *mprev;
	pnode_t **prev_tower;

	if(!list) return;
	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }
	
	//Find previous nodes for each level in the list->tower.
	prev_tower = slist_build_prev_tower(list, data, &mprev);

	//Find initial mnode_t in floor
	if(mprev) mcurr = mprev->next;
	else mcurr = list->first;

	while(mcurr && cmp_data(&mcurr->data, data))
		mcurr = (mprev = mcurr, mcurr->next);

	//Check if data exists in the list already.
	if(mcurr && equal_data(&mcurr->data, data)){ free(prev_tower); return; }

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

	free(prev_tower);
	list->size++;
}

void slist_remove_key(slist_t *list, data_t *key){
	int i;
	mnode_t *mcurr, *mprev;
	pnode_t *prev, *curr, **prev_tower;

	if(!list) return;
	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }

	//Find previous nodes for each level in the list->tower.
	prev_tower = slist_build_prev_tower(list, key, &mprev);

	printf("DEBUG: Finding initial point in floor\n");
	if(mprev) mcurr = mprev->next;
	else mcurr = list->first;

	printf("DEBUG: Beginning to navigate the floor\n");
	while(mcurr && cmp_data(&mcurr->data, key))
		mcurr = (mprev = mcurr, mcurr->next);

	printf("DEBUG: Checking if data exists\n");
	//Check data exists in the list already.
	if(!mcurr || !equal_data(&mcurr->data, key)){ free(prev_tower); return; }
	
	printf("DEBUG: Beginning wiping tower matrix\n");
	for(i = 0; i < list->levels; i++){
		printf("DEBUG: Picking a tower pointer\n");
		prev = prev_tower[i];
		curr = prev ? prev->next : list->tower[i];
		
		//Checks if current pnode_t points to the mnode_t that contains 'key'
		if(!curr || curr->target != mcurr) break;

		printf("DEBUG: Swaping pointers\n");
		if(prev) prev->next = curr->next;
		else list->tower[i] = curr->next;

		pnode_destroy(&curr);
	}
	free(prev_tower);
	
	printf("DEBUG: Destroy main node\n");
	if(mprev) mprev->next = mcurr->next;
	else list->first = mcurr->next;
	mnode_destroy(&mcurr);
	list->size--;

	slist_fix_empty_levels(list);
}

data_t *slist_search(slist_t *list, data_t *data){
	int i;
	pnode_t *pprev, *pcurr;
	mnode_t *curr;

	pprev = NULL;
	for(i = list->levels - 1; i >= 0; i--){
		if(pprev){
			pprev = pprev->down;
			pcurr = pprev->next;
		} else pcurr = list->tower[i];
		
		while(pcurr && cmp_data(&pcurr->target->data, data))
			pcurr = (pprev = pcurr, pcurr->next);
	}
	
	if(pprev) curr = pprev->target->next;
	else curr = list->first;

	while(curr && cmp_data(&curr->data, data))
		curr = curr->next;

	if(curr && equal_data(&curr->data, data)) return &curr->data;
	else return NULL;
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

int slist_size(slist_t *list){
	return list ? list->size : -1;
}
