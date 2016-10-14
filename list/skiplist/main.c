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

	data = 1;
	slist_remove_key(list, &data);
	slist_remove_key(list, &data);
	slist_remove_key(list, &data);
	slist_print(list);
	printf("\n");

	data = 10;
	slist_remove_key(list, &data);
	slist_remove_key(list, &data);
	slist_remove_key(list, &data);
	slist_print(list);
	printf("\n");
	
	data = 0;
	if(slist_search(list, &data)) printf("Found %d\n", data);
	
	data = 1;
	if(slist_search(list, &data)) printf("Found %d\n", data);

	data = 3;
	if(slist_search(list, &data)) printf("Found %d\n", data);

	data = 4;
	if(slist_search(list, &data)) printf("Found %d\n", data);

	data = 10;
	if(slist_search(list, &data)) printf("Found %d\n", data);

	slist_destroy(&list);
	return 0;
}
