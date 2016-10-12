#include <stdlib.h>
#include <stdio.h>
#include <list.h>

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

	list_set_cmp_data(lower);
	list_set_print_data(print);
	list_set_equal_data(equal);
	list_t *list = list_alloc();
	
	list_insert(list, &data);
	list_insert(list, &data);
	list_insert(list, &data);
	data++;
	list_print(list);
	printf("\n");

	list_insert(list, &data);
	list_insert(list, &data);
	list_insert(list, &data);
	data++;
	list_print(list);
	printf("\n");

	list_insert(list, &data);
	data++;
	list_print(list);
	printf("\n");

	list_insert(list, &data);
	list_insert(list, &data);
	list_insert(list, &data);
	data++;
	list_print(list);
	printf("\n");

	list_insert(list, &data);
	data++;
	list_print(list);
	printf("\n");

	printf("List size: %d\n", list_size(list));
	list_remove(list, 2);
	list_print(list);
	printf("\n");
	
	list_remove(list, 0);
	list_print(list);
	printf("\n");

	data = 3;
	list_remove_key(list, &data);
	list_remove_key(list, &data);

	data = 10;
	list_remove_key(list, &data);
	
	list_print(list);
	printf("\n");

	data = 4;
	list_remove_key(list, &data);
	
	data = 1;
	list_remove_key(list, &data);
	
	//list_remove(list, 0);

	printf("List size: %d\n", list_size(list));

	int *p = list_retrieve(list, 0);
	if(p) printf("Retrieving: %d\n", *p);

	list_print(list);
	printf("\n");

	list_destroy(&list);
	return 0;
}
