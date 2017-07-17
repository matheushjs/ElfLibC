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

void format_test(){
	char *test;

	test = elfString_format("%d %s", 32, "is a cool number");
	printf("{%s}\n", test);
	free(test);

	test = elfString_format("%d %f %lf [%s] %p", 32, 0.5, 0.5, "are\ncool\nnumbers", test);
	printf("{%s}\n", test);
	free(test);
}

int main(int argc, char *argv[]){
	strip_test();
	format_test();

	return 0;
}
