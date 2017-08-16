#include <stdio.h>
#include <stdlib.h>
#include <elf_string_buf.h>

void test1(){
	int i;
	char *str;
	ElfStringBuf *buf = elfStringBuf_new();

	// Adds 10 characters
	for(i = 0; i < 10; i++)
		elfStringBuf_appendChar(buf, '*');

	// Adds 1 character
	elfStringBuf_appendChar(buf, '\n');

	// Adds 30 characters
	for(i = 0; i < 10; i++)
		elfStringBuf_appendString(buf, "+++");

	printf("Length: %d (41)\n", elfStringBuf_getLength(buf));
	printf("Current string: %s\n", elfStringBuf_getString(buf));

	// Adds 2 characters 
	elfStringBuf_insertChar(buf, 0, '1');
	elfStringBuf_insertChar(buf, elfStringBuf_getLength(buf), '9');
	printf("Length: %d (43)\n", elfStringBuf_getLength(buf));

	str = elfStringBuf_makeString(buf, NULL);
	printf("%s\n", str);
	free(str);

	elfStringBuf_destroy(&buf);
}

void test2(){
	ElfStringBuf *buf = elfStringBuf_new();

	// Adds 15 characters
	elfStringBuf_insertBytes(buf, 0, "-------------------", 5);
	elfStringBuf_insertBytes(buf, elfStringBuf_getLength(buf), "+++++++++++++++++++", 10);
	printf("Length: %d (15)\n", elfStringBuf_getLength(buf));
	printf("Current string: %s\n", elfStringBuf_getString(buf));

	// Removes 4 characters
	elfStringBuf_removeBytes(buf, 0, 2);
	elfStringBuf_removeBytes(buf, elfStringBuf_getLength(buf)-2, 2);
	printf("Length: %d (11)\n", elfStringBuf_getLength(buf));
	printf("Current string: %s\n", elfStringBuf_getString(buf));

	// Removes all characters
	int len = elfStringBuf_getLength(buf);
	elfStringBuf_removeBytes(buf, len-1, 1000);
	printf("Current string: {%s}\n", elfStringBuf_getString(buf));
	elfStringBuf_removeBytes(buf, len-2, 1000);
	printf("Current string: {%s}\n", elfStringBuf_getString(buf));
	elfStringBuf_removeBytes(buf, len-5, 1000);
	printf("Current string: {%s}\n", elfStringBuf_getString(buf));
	elfStringBuf_removeBytes(buf, 0, 1000);
	printf("Current string: {%s}\n", elfStringBuf_getString(buf));
	printf("Length: %d (0)\n", elfStringBuf_getLength(buf));

	elfStringBuf_destroy(&buf);
}

int main(int argc, char *argv[]){
	test1();
	test2();
	return 0;
}
