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

int main(int argc, char *argv[]){
	//strip_test();
	//format_test();
	//split_test();
	join_test();

	return 0;
}


const static char test_string[] = "\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Cras luctus maximus dui, sed porttitor ipsum tincidunt sed. Mauris elit enim, varius vel eros in, ultricies ultricies mi. Curabitur nec tempor nisi. Integer in sagittis odio, a venenatis felis. In volutpat ut nisi eleifend blandit. Pellentesque consequat, nisi in commodo sollicitudin, quam ligula pulvinar lorem, ut sodales ante augue in orci. Mauris a tempus leo, eu imperdiet lorem. Nunc a mauris sed ipsum vestibulum luctus vel at velit.\n\nInteger sit amet gravida enim. Nulla vitae consequat orci, in convallis felis. In et ex eu turpis pellentesque ornare vitae tempor risus. Sed tristique consequat urna, et mollis lorem iaculis sit amet. Curabitur ullamcorper felis vitae magna lacinia, ut vestibulum urna efficitur. Donec vel interdum nibh, at auctor quam. Mauris id auctor est, id dictum neque. Proin facilisis at lacus eget pellentesque. Duis non nisi eu lorem porttitor accumsan. Donec eget fringilla leo. Sed bibendum est eu nisi interdum ultrices. Suspendisse eu placerat dolor, et fringilla erat. Maecenas elit eros, vehicula sed maximus eu, bibendum quis orci.\n";
