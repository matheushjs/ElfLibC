#ifndef _UTILS_H
#define _UTILS_H

#define SWAP(X,Y) X=(Y=(X=X^Y,X^Y),X^Y)

#include <vector.h>

void swap(data_t *, int, int);

#endif