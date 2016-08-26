#ifndef _STACK_H
#define _STACK_H

#define TYPE int		//type being stored in stack
#define TYPENULL 0		//Define a null value for TYPE
#define TYPE_PRINTARG "%d"	//string format to print TYPE correctly
#define C_TYPE int		//type used for counting elements in stack

typedef struct {
	TYPE *vec;
	C_TYPE count;
} stack_t;

#endif
