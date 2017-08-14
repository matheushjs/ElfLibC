#include <stdio.h>
#include <stdlib.h>
#include <elf_string_buf.h>

void test1(){
	int i;
	char *str;
	ElfStringBuf *buf = elfStringBuf_new();

	for(i = 0; i < 10; i++)
		elfStringBuf_appendChar(buf, '*');
	elfStringBuf_appendChar(buf, '\n');
	for(i = 0; i < 10; i++)
		elfStringBuf_appendString(buf, "+++");

	str = elfStringBuf_makeString(buf, NULL);
	printf("%s\n", str);
	free(str);

	elfStringBuf_destroy(&buf);
}

int main(int argc, char *argv[]){
	test1();
	return 0;
}
