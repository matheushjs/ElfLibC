#include <stdlib.h>
#include <stdio.h>
#include <queue.h>

typedef struct person_struct {
	char name[80];
	int age;
} person;

person *person_create(){
	static int counter = 1;
	person *result = (person *) calloc(sizeof(person), 1);
	result->age = counter++;
	return result;
}

int main(int argc, char *argv[]){
	queue_t *queue = queue_alloc();
	person *p;

	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	
	queue_invert(queue);
	
	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);

	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);
	
	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);
	
	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);
	
	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);
	
	queue_invert(queue);
	
	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);
	
	queue_invert(queue);
	
	p = queue_pop(queue);
	printf("Age: %d\n", p->age);
	free(p);

	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);

	queue_destroy(&queue);

	printf("Label 1\n");

	queue = queue_alloc();

	void freePerson(void *p){
		free(p);
	}

	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);
	p = person_create();
	queue_push(queue, p);

	queue_destroy_f(&queue, (void (*)(person *)) freePerson);

	return 0;
}
