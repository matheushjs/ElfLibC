#include <stdio.h>
#include <elf_utf_buf.h>

void test1(){
	ElfUtfBuf *buf = elfUtfBuf_new();

	// Adds 3 characters.
	elfUtfBuf_appendChar(buf, "é");
	elfUtfBuf_appendChar(buf, "e");
	elfUtfBuf_appendChar(buf, "は");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("%s\n", elfUtfBuf_getString(buf));

	elfUtfBuf_destroy(&buf);
}

int main(int argc, char *argv[]){
	test1();
	return 0;
}
