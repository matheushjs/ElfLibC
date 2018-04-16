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
	elfStringBuf_setChar(buf, 0, '2');
	elfStringBuf_insertChar(buf, elfStringBuf_getLength(buf), '9');
	elfStringBuf_setChar(buf, elfStringBuf_getLength(buf)-1, '8');
	printf("Length: %d (43)\n", elfStringBuf_getLength(buf));

	printf("Char at 0: %c\n", elfStringBuf_getChar(buf, 0));
	printf("Char at end: %c\n", elfStringBuf_getChar(buf, elfStringBuf_getLength(buf)-1));

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
