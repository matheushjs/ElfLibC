#include <stdlib.h>
#include <stdio.h>
#include <list.h>

int main(int argc, char *argv[]){
	bool lower(data_t *d1, data_t *d2){ return *d1 < *d2 ? TRUE : FALSE; }
	void print(data_t *data){ printf("%d", *data); }

	int data = 0;
	list_t *list = list_alloc();
	list_insert(list, &data, lower);
	data++;
	list_insert(list, &data, lower);
	data++;
	list_insert(list, &data, lower);
	data++;
	list_insert(list, &data, lower);
	data++;
	list_insert(list, &data, lower);
	data++;

	printf("List size: %d\n", list_size(list));
	list_remove(list, 2);
	list_remove(list, 0);
	printf("List size: %d\n", list_size(list));

	int *p = list_retrieve(list, 0);
	printf("Retrieving: %d\n", *p);
	free(p);

	list_print_r(list, print);

	list_destroy(&list);
	return 0;
}
