#include <stdio.h>
#include <elf_canvas.h>

void test1(){
	ElfCanvas *elf = elfCanvas_new(10, 10);

	elfCanvas_drawChar(elf, 0, 0, "*");
	elfCanvas_drawChar(elf, 9, 9, "*");
	elfCanvas_drawChar(elf, 9, 0, "*");
	elfCanvas_drawChar(elf, 0, 9, "*");
	elfCanvas_print(elf);
	
	elfCanvas_drawText(elf, 0, 0, "Matheusssssssssss");
	elfCanvas_drawText_v(elf, 0, 0, "Matheussssssssss");
	elfCanvas_drawText_v(elf, 9, 9, "Matheussssssssss");
	elfCanvas_print(elf);

	elfCanvas_fillRow_span(elf, 0, 20, 0, "a");
	elfCanvas_fillRow(elf, 9, "b");
	elfCanvas_fillCol_span(elf, 0, 20, 0, "c");
	elfCanvas_fillCol(elf, 9, "d");

	elfCanvas_fprint(elf, stdout);
	elfCanvas_destroy(&elf);
}

int main(int argc, char *argv[]){
	test1();
	
	return 0;
}
