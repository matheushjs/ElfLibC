#include <stdio.h>
#include <stdlib.h>

#include <elf_csv.h>

void print_strings(const char **strings){
	while(*strings){
		printf("{%s} ", *strings);
		strings++;
	}
	printf("\n");
}

void print_csv(const char *filename, char sep){
	ElfCsvR *reader;
	const char **strings;

	reader = elfCsvR_new_withSep(filename, sep);
	while(elfCsvR_hasNext(reader)){
		strings = elfCsvR_nextLine_managed(reader);
		print_strings(strings);
	}
	elfCsvR_destroy(&reader);
}

void copy_csv(const char *in, const char *out, char sep){
	ElfCsvR *reader;
	ElfCsvW *writer;
	const char **strings;
	int count;

	reader = elfCsvR_new_withSep(in, sep);
	writer = elfCsvW_new_withArgs(out, sep, false);
	
	while(elfCsvR_hasNext(reader)){
		strings = elfCsvR_nextLine_managed(reader);
		for(count = 0; strings[count] != NULL; count++);
		elfCsvW_writeLine_size(writer, strings, count);
	}

	elfCsvR_destroy(&reader);
	elfCsvW_destroy(&writer);
}

int main(int argc, char *argv[]){
	printf("=================================\n");
	print_csv("test1.csv", ',');
	printf("=================================\n");
	print_csv("test2.csv", ',');
	printf("=================================\n");
	print_csv("test3.csv", ',');
	printf("=================================\n");
	print_csv("test4.csv", '+');
	printf("=================================\n");

	copy_csv("test1.csv", "out1.csv", ',');
	copy_csv("test2.csv", "out2.csv", ',');
	copy_csv("test3.csv", "out3.csv", ',');
	copy_csv("test4.csv", "out4.csv", '+');

	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	printf("=================================\n");
	print_csv("out1.csv", ',');
	printf("=================================\n");
	print_csv("out2.csv", ',');
	printf("=================================\n");
	print_csv("out3.csv", ',');
	printf("=================================\n");
	print_csv("out4.csv", '+');
	printf("=================================\n");

	return 0;
}
