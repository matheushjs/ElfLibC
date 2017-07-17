#include <stdio.h>
#include <stdlib.h>

#include <elf_string.h>

void strip_test(){
	char test[][100] = {
		"Hello",
		"\n\t\v\rHello   ",
		"\n\t\v        \r",
		"  Hello World ! \n",
		"",
		"\b"
	};
	char *t;
	int i;

	for(i = 0; test[i][0] != '\b'; i++){
		t = elfString_strip((char *) test[i]);
		printf("Source: {%s}   Output: {%s}\n", (char *) test[i], t);
		free(t);
	}
}

int main(int argc, char *argv[]){
	strip_test();

	return 0;
}
