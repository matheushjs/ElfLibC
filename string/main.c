#include <stdio.h>
#include <stdlib.h>

#include <elf_string.h>

// Defined on the bottom
const static char test_string[];

void print_stringArray(char **strings){
	while(*strings != NULL){
		printf("{%s} ", *strings);
		strings++;
	}
	printf("\n");
}

void free_stringArray(char **strings){
	char **aux = strings;
	while(*aux != NULL){
		free(*aux);
		aux++;
	}
	free(strings);
}

void strip_test(){
	char test[][100] = {
		"Hello",
		"\n\t\v\rHello   ",
		"\n\t\v        \r",
		"  Hello World ! \n",
		"",
		"\b"
	};
	char *t;
	int i;

	for(i = 0; test[i][0] != '\b'; i++){
		t = elfString_strip((char *) test[i]);
		printf("Source: {%s}   Output: {%s}\n", (char *) test[i], t);
		free(t);
	}
}

void format_test(){
	char *test;

	test = elfString_format("%d %s", 32, "is a cool number");
	printf("{%s}\n", test);
	free(test);

	test = elfString_format("%d %f %lf [%s] %p", 32, 0.5, 0.5, "are\ncool\nnumbers", test);
	printf("{%s}\n", test);
	free(test);
}

void split_test(){
	char **strings;

	printf("\n=== SPLIT ===\n\n");

	strings = elfString_split(test_string, " ");
	print_stringArray(strings);
	free_stringArray(strings);

	strings = elfString_split(test_string, ". ");
	print_stringArray(strings);
	free_stringArray(strings);

	strings = elfString_split(test_string, "\n\n");
	print_stringArray(strings);
	free_stringArray(strings);

	strings = elfString_split(test_string, "Donec");
	print_stringArray(strings);
	free_stringArray(strings);

	printf("\n=== SPLIT BAG ===\n\n");

	strings = elfString_split_bag(test_string, " ,.;-:/\\");
	print_stringArray(strings);
	free_stringArray(strings);

	strings = elfString_split_bag(test_string, "abcdefghijklmnopqrstuvxwyz \n,.");
	print_stringArray(strings);
	free_stringArray(strings);
	
	strings = elfString_split_bag(test_string, "\n");
	print_stringArray(strings);
	free_stringArray(strings);

	printf("\n=== SPLIT LINES ===\n\n");

	strings = elfString_splitLines(test_string);
	print_stringArray(strings);
	free_stringArray(strings);

}

void join_test(){
	char *str;

	str = elfString_join("<-->", "Hello", "World", "This", "Is", "Variadic!", NULL);
	printf("%s\n", str);
	free(str);

	str = elfString_join("<-->", "Hello", "", "", "", "Variadic!", NULL);
	printf("%s\n", str);
	free(str);
}

void center_test(){
	char *str;

	str = elfString_center_fill(" Hello ", 30, '*');
	printf("{%s}\n", str);
	free(str);

	str = elfString_center_fill(" Hello ", 1, '*');
	printf("{%s}\n", str);
	free(str);

	str = elfString_center_fill(" Hello ", 7, '*');
	printf("{%s}\n", str);
	free(str);

	str = elfString_center(" Hello ", 10);
	printf("{%s}\n", str);
	free(str);

	str = elfString_center(" Hello ", 11);
	printf("{%s}\n", str);
	free(str);
}

void countFind_test(){
	int count, find;

	count = elfString_count(test_string, "Donec");
	find = elfString_find(test_string, "Donec");
	printf("Donec: %d\n", count);
	printf("Donec: %d\n", find);
	find = elfString_find( ((char *) test_string) + find + 1, "Donec");
	printf("Donec: %d\n", find);
	
	count = elfString_count("",  "");
	find = elfString_find("", "");
	printf("(empty): %d\n", count);
	printf("(empty): %d\n", find);

	count = elfString_count("", "Donec");
	find = elfString_find("", "Donec");
	printf("Donec: %d\n", count);
	printf("Donec: %d\n", find);

	count = elfString_count("hey", "");
	find = elfString_find("hey", "");
	printf("(empty): %d\n", count);
	printf("(empty): %d\n", find);

	count = elfString_count("aaaaaaa", "aa");
	find = elfString_find("aaaaaaa", "aa");
	printf("aa: %d\n", count);
	printf("aa: %d\n", find);
}

void replace_test(){
	char *str;

	str = elfString_replace(test_string, "Donec", "   REPLACED   ");
	printf("{%s}\n", str);
	free(str);

	str = elfString_replace(test_string, "", "   REPLACED   ");
	printf("{%s}\n", str);
	free(str);

	str = elfString_replace(test_string, "C", "   REPLACED   ");
	printf("{%s}\n", str);
	free(str);
}

void swith_test(){
	printf("%d\n", elfString_endsWith("Hello", "llo"));
	printf("%d\n", elfString_endsWith("llo", "Hello"));
	printf("%d\n", elfString_endsWith("Hello", ""));
	printf("%d\n", elfString_endsWith("", "Hello"));
	printf("%d\n", elfString_startsWith("Hello", "Hell"));
	printf("%d\n", elfString_startsWith("llo", "Hello"));
	printf("%d\n", elfString_startsWith("Hello", ""));
	printf("%d\n", elfString_startsWith("", "Hello"));
}

void slice_test(){
	char *str;

	str = elfString_slice("hello", 0, 1);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("hello", 1, 3);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("hello", 0, -1);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("hello", -5, -1);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("hello", 0, INT_MAX);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("hello", -5000, 0);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("hello", -5000, -5005);
	printf("{%s}\n", str);
	free(str);

	str = elfString_slice("", 0, 0);
	printf("{%s}\n", str);
	free(str);
}

void invert_test(){
	char *str;

	str = elfString_invert(test_string);
	printf("{%s}\n", str);
	free(str);

	str = elfString_invert("");
	printf("{%s}\n", str);
	free(str);
}

void case_test(){
	char *str;

	str = elfString_lower("heLlO");
	printf("{%s}\n", str);
	free(str);

	str = elfString_upper("heLlO");
	printf("{%s}\n", str);
	free(str);

	str = elfString_capitalize("heLlO");
	printf("{%s}\n", str);
	free(str);

	str = elfString_swapCase("heLlO");
	printf("{%s}\n", str);
	free(str);
}

int main(int argc, char *argv[]){
	//strip_test();
	//format_test();
	//split_test();
	//join_test();
	//center_test();
	//countFind_test();
	//replace_test();
	//swith_test();
	//slice_test();
	//invert_test();
	case_test();

	return 0;
}


const static char test_string[] = "\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Cras luctus maximus dui, sed porttitor ipsum tincidunt sed. Mauris elit enim, varius vel eros in, ultricies ultricies mi. Curabitur nec tempor nisi. Integer in sagittis odio, a venenatis felis. In volutpat ut nisi eleifend blandit. Pellentesque consequat, nisi in commodo sollicitudin, quam ligula pulvinar lorem, ut sodales ante augue in orci. Mauris a tempus leo, eu imperdiet lorem. Nunc a mauris sed ipsum vestibulum luctus vel at velit.\n\nInteger sit amet gravida enim. Nulla vitae consequat orci, in convallis felis. In et ex eu turpis pellentesque ornare vitae tempor risus. Sed tristique consequat urna, et mollis lorem iaculis sit amet. Curabitur ullamcorper felis vitae magna lacinia, ut vestibulum urna efficitur. Donec vel interdum nibh, at auctor quam. Mauris id auctor est, id dictum neque. Proin facilisis at lacus eget pellentesque. Duis non nisi eu lorem porttitor accumsan. Donec eget fringilla leo. Sed bibendum est eu nisi interdum ultrices. Suspendisse eu placerat dolor, et fringilla erat. Maecenas elit eros, vehicula sed maximus eu, bibendum quis orci.\n";
