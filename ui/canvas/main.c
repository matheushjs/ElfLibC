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
