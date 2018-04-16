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
#include <elf_canvas.h>

void test1(){
	ElfCanvas *elf = elfCanvas_new(10, 10);

	elfCanvas_drawChar(elf, 0, 0, "*");
	elfCanvas_drawChar(elf, 9, 9, "*");
	elfCanvas_drawChar(elf, 9, 0, "*");
	elfCanvas_drawChar(elf, 0, 9, "*");
	elfCanvas_print(elf);
	
	elfCanvas_drawText(elf, 0, 0, "Mathéússsssssssss");
	elfCanvas_drawText_v(elf, 0, 0, "Mathéússssssssss");
	elfCanvas_drawText_v(elf, 9, 9, "Mathéússssssssss");
	elfCanvas_print(elf);

	elfCanvas_fillRow_span(elf, 0, 20, 0, "á");
	elfCanvas_fillRow(elf, 9, "í");
	elfCanvas_fillCol_span(elf, 0, 20, 0, "ú");
	elfCanvas_fillCol(elf, 9, "é");

	elfCanvas_fprint(elf, stdout);

	char *string = elfCanvas_buildString(elf);
	printf("=======\n%s=======\n", string);
	free(string);

	elfCanvas_destroy(&elf);
}

void test2(){
	ElfCanvas *elf;

	elf = elfCanvas_new(1, 1);
	
	char *string;
	string = elfCanvas_buildString(elf);

	printf("{%s}\n", string);
	free(string);

	elfCanvas_destroy(&elf);
}

int main(int argc, char *argv[]){
	test1();
	test2();
	return 0;
}
