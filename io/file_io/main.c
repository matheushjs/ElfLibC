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

#include <elf_file.h>

const static
char test[] = "This is a test array.";

void pbool(bool proof, bool val){
	printf("%s? %s\n", proof ? "true" : "false", val ? "true" : "false");
}

void test_lib(){
	elfFile_setContent("teste.txt", test, sizeof(test) - 1);
	pbool(true, elfFile_exists("teste.txt"));
	pbool(false, elfFile_exists("nopeeeee.txt"));
	
	printf("%d\n", elfFile_size("teste.txt"));

	FILE *fp = fopen("teste.txt", "r+");
	printf("%c\n", elfFile_fpeek(fp));
	pbool(false, elfFile_feof(fp));
	fseek(fp, 0, SEEK_END);
	pbool(true, elfFile_feof(fp));
	printf("%c\n", elfFile_fpeek(fp));
	fseek(fp, -1, SEEK_END);
	printf("%c\n", elfFile_fpeek(fp));
	elfFile_fappendContent(fp, test, sizeof(test) - 1);
	printf("%d\n", elfFile_fsize(fp));
	fclose(fp);
	
	elfFile_printByteInfo("teste.txt");
	elfFile_printContent("teste.txt");
	printf("\n");
	elfFile_appendContent("teste.txt", test, sizeof(test) - 1);
	elfFile_appendContent("teste.txt", "\na\n\n", 4);
	printf("Size: %d\n", elfFile_size("teste.txt"));
	printf("Lines: %d\n", elfFile_countLines("teste.txt"));

	int size;
	char *v = elfFile_getContent("teste.txt", &size);
	fwrite(v, size, sizeof(char), stdout);
	free(v);
	printf("\n");

	char *line;
	printf("Type a line:\n");
	line = elfFile_freadLine(stdin);
	printf("{%s}\n", line);
	free(line);

	printf("Type a line:\n");
	line = elfFile_freadLine(stdin);
	printf("{%s}\n", line);
	free(line);
}

int main(int argc, char *argv[]){
	test_lib();
	return 0;
}
