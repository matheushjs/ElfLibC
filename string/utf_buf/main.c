/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

	EPRINT("setChar");
	for(i = elfUtfBuf_getLength(buf) - 1; i >= 0; i--)
		elfUtfBuf_setChar(buf, i, "ん");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("0> %s\n", elfUtfBuf_getString(buf));
	for(i = elfUtfBuf_getLength(buf) - 1; i >= 0; i--)
		elfUtfBuf_setChar(buf, i, "é");
	printf("Len: %d\n", elfUtfBuf_getLength(buf));
	printf("Bytes: %d\n", elfUtfBuf_getSize(buf));
	printf("0> %s\n", elfUtfBuf_getString(buf));

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
