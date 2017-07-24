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

int main(int argc, char *argv[]){
	printf("=================================\n");
	print_csv("test1.csv", ',');
	printf("=================================\n");
	print_csv("test2.csv", ',');
	printf("=================================\n");
	print_csv("test3.csv", ',');
	printf("=================================\n");
	print_csv("test4.csv", ',');
	printf("=================================\n");
	print_csv("test5.csv", ',');
	printf("=================================\n");
	print_csv("test6.csv", '+');
	printf("=================================\n");

	return 0;
}
