#include <stdlib.h>
#include <stdio.h>
#include <stack_st.h>

stack_t *stack_create(){
	stack_t *s;
	s = (stack_t *) calloc(1, sizeof(s));
}

void stack_destroy(stack_t **s){
	if(*s){
		if((*s)->vec) free((*s)->vec);
		free(*s);
		*s = NULL;
	}
}

void stack_print(stack_t *s){
	int i;
	for(i = 0; i < s->count-1; i++)
		printf(TYPE_PRINTARG", ",  s->vec[i]);
	if(s->count) printf(TYPE_PRINTARG,  s->vec[i]);
	printf("\n");
}

void stack_push(stack_t *s, TYPE value){
	s->vec = realloc(s->vec, sizeof(TYPE) * (s->count+1));
	s->vec[s->count++] = value;
}

TYPE stack_pop(stack_t *s){
	TYPE ret;
	if(!s->count) return TYPENULL;
	ret = s->vec[--s->count];
	s->vec = realloc(s->vec, sizeof(TYPE) * (s->count));
	return ret;
}
