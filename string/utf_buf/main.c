#include <stdio.h>
#include <elf_utf_buf.h>

int main(int argc, char *argv[]){
	ElfUtfBuf *buf = elfUtfBuf_new();
	elfUtfBuf_destroy(&buf);

	return 0;
}
