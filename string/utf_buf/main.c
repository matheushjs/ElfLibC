#include <stdio.h>
#include <stdlib.h>
#include <elf_utf_buf.h>

#define EPRINT(X) printf("\n -=-=- " X " -=-=- \n")

void test1(){
	int i;
	ElfUtfBuf *buf = elfUtfBuf_new();

	// Adds 3 characters.
	elfUtfBuf_appendChar(buf, "");
	elfUtfBuf_appendChar(buf, "é");
	elfUtfBuf_appendChar(buf, "e");
	elfUtfBuf_appendChar(buf, "は");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("%s\n", elfUtfBuf_getString(buf));

	// Adds 4 characters.
	elfUtfBuf_appendString(buf, "");
	elfUtfBuf_appendString(buf, "おはよう");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("%s\n", elfUtfBuf_getString(buf));

	int len;
	char *made = elfUtfBuf_makeString(buf, &len);
	EPRINT("makeString");
	printf("Len: %d\n", len);
	printf("%s\n", made);
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("%s\n", elfUtfBuf_getString(buf));
	free(made);

	EPRINT("insertChar");
	elfUtfBuf_insertChar(buf, 0, "へ");
	elfUtfBuf_insertChar(buf, 0, "ふ");
	elfUtfBuf_insertChar(buf, 0, "は");
	elfUtfBuf_insertChar(buf, 1, "ひ");
	elfUtfBuf_insertChar(buf, 4, "ほ");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("%s (はひふへほ)\n", elfUtfBuf_getString(buf));

	EPRINT("insertString");
	elfUtfBuf_insertString(buf, 5, " たちつてと");
	elfUtfBuf_insertString(buf, 0, "まみむめも ");
	elfUtfBuf_insertString(buf, 0, "");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("0> %s\n1> まみむめも はひふへほ たちつてと\n", elfUtfBuf_getString(buf));

	EPRINT("getChar");
	for(i = elfUtfBuf_getLength(buf) - 1; i >= 0; i--){
		printf("%s\n", elfUtfBuf_getChar(buf, i));
	}

	EPRINT("removeString");
	elfUtfBuf_removeString(buf, 0, 0);
	elfUtfBuf_removeString(buf, 17, 1);
	elfUtfBuf_removeString(buf, 16, 1);
	elfUtfBuf_removeString(buf, 11, 100);
	elfUtfBuf_removeString(buf, 0, 6);
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("0> %s\n1> はひふへほ\n", elfUtfBuf_getString(buf));

	made = elfUtfBuf_makeString(buf, &len);
	EPRINT("makeString");
	printf("Len: %d\n", len);
	printf("%s\n", made);
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("%s\n", elfUtfBuf_getString(buf));
	free(made);

	elfUtfBuf_destroy(&buf);
}

int main(int argc, char *argv[]){
	test1();
	return 0;
}
