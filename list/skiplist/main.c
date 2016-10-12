#include <stdlib.h>
#include <stdio.h>
#include <slist.h>

bool lower(int *d1, int *d2){
	return *d1 < *d2 ? TRUE : FALSE;
}

bool equal(int *d1, int *d2){
	return *d1 == *d2 ? TRUE : FALSE;
}

void print(int *d){
	printf("%d", *d);
}

int main(int argc, char *argv[]){
	int data = 0;

	slist_set_cmp_data(lower);
	slist_set_print_data(print);
	slist_set_equal_data(equal);
	slist_t *list = slist_alloc();
	
	slist_insert(list, &data);
	slist_insert(list, &data);
	slist_insert(list, &data);
	slist_print(list);
	printf("\n");

	data++;
	slist_insert(list, &data);
	slist_insert(list, &data);
	slist_insert(list, &data);
	slist_print(list);
	printf("\n");

	data++;
	slist_insert(list, &data);
	slist_print(list);
	printf("\n");

	data++;
	slist_insert(list, &data);
	slist_insert(list, &data);
	slist_insert(list, &data);
	slist_print(list);
	printf("\n");

	data++;
	slist_insert(list, &data);
	slist_print(list);
	printf("\n");
/*
	printf("List size: %d\n", slist_size(list));
	slist_remove(list, 2);
	slist_print(list);
	printf("\n");
	
	slist_remove(list, 0);
	slist_print(list);
	printf("\n");

	data = 3;
	slist_remove_key(list, &data);
	slist_remove_key(list, &data);

	data = 10;
	slist_remove_key(list, &data);
	
	slist_print(list);
	printf("\n");

	data = 4;
	slist_remove_key(list, &data);
	
	data = 1;
	slist_remove_key(list, &data);
	
	//slist_remove(list, 0);

	printf("List size: %d\n", slist_size(list));

	int *p = slist_retrieve(list, 0);
	if(p) printf("Retrieving: %d\n", *p);

	slist_print(list);
	printf("\n");

	slist_destroy(&list);
*/	return 0;
}
