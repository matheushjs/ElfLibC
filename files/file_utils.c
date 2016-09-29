#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <file_utils.h>

//Retorna o número de linhas no arquivo.
int count_row(char *filename){
	char c;
	int result = 0;
	FILE *fp;

	fp = fopen(filename, "r");
	if(!fp) return -1;

	while(!feof(fp)){
		c = fgetc(fp);
		if(c == '\n') result++;
		
		if(feof(fp)){
			//Trata do caso em que o arquivo termina com ENTER seguido de EOF.
			fseek(fp, -1, SEEK_CUR);
			if(fgetc(fp) != '\n') result++;
			break;
		}
	}

	fclose(fp);
	return result;
}

//Conta quantos números, reais ou inteiros, há na primeira linha do arquivo.
int count_num_col(char *filename){
	char c;
	int result = 0;
	FILE *fp;

	fp = fopen(filename, "r");
	if(!fp) return -1;

	do{
		c = fgetc(fp);
		if(c == '\n') break;
		if(isdigit(c) || c == '-'){
			result++;
			fseek(fp, -1, SEEK_CUR);
			//fscanf "pula" o número.
			fscanf(fp, "%*f");
		}
	}while(!feof(fp));

	fclose(fp);
	return result;
}

int file_size(FILE *fp){
	int result;
	fseek(fp, 0, SEEK_END);
	result = (int) ftell(fp);
	return result;
}

bool file_exist(char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return FALSE;
	fclose(fp);
	return TRUE;
}

bool file_EOF(FILE *fp){
	char c = fgetc(fp);
	if(c == EOF) return TRUE;
	fseek(fp, -1, SEEK_CUR);
	return FALSE;
}
