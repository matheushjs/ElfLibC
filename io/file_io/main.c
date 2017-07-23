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
	printf("%d\n", elfFile_size("teste.txt"));

	int size;
	char *v = elfFile_getContent("teste.txt", &size);
	fwrite(v, size, sizeof(char), stdout);
	free(v);
	printf("\n");
}

int main(int argc, char *argv[]){
	test_lib();
	return 0;
}
