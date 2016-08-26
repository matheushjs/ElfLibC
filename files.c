#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* Atenção
 * muitos arquivos terminam com um ENTER antes do -1
 * por isso, nunca assuma que depois de um ENTER haverá alguma coisa
 */


/*
 * Imprime os valores no arquivo dado, sequencialmente, byte por byte.
 */
#include <stdio.h>
void printFileSeq(char *filename){
	FILE *fp;
	char c;

	fp = fopen(filename, "r");
	if(!fp) return;

	do{
		c = fgetc(fp);
		if(feof(fp)) break;
		printf("%c %d\n", c, c);
	}while(1);

	fclose(fp);
}


/*
 * Retorna a próxima palavra do arquivo apontado por fp
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
char *readWord(FILE *fp){
	int counter = 0;
	char c, *s = NULL;

	if(!fp) return NULL;

	//Navega até encontrar uma letra
	do{
		c = fgetc(fp);
		if(feof(fp)) return NULL;
	}while(!isalpha(c));
	fseek(fp, -1, SEEK_CUR);

	//Lê a palavra a partir da letra
	do{
		c = fgetc(fp);
		s = (char *) realloc(s, sizeof(char) * (counter+1));
		s[counter++] = c;
	}while(c != '\n' && c != ' ' && c != '\t' && c != ',' && c != EOF);	//isalpha(c) é menos custoso, mas não detecta acentos.
	s[counter-1] = '\0';

	return s;
}


/*
 * Retorna a próxima sequencia de caracteres que estejam entre separadores em *sep
 */
char *readWord(FILE *fp, const char *sep){
	char c, *s = NULL;
	int i, counter = 0;

	if(!fp) return NULL;

	//navega até encontrar qualquer coisa que não seja o separador
	do{
		c = fgetc(fp);
		if(feof(fp)) return NULL;
		for(i = 0; sep[i] != c && sep[i] != '\0'; i++);
		if(sep[i] == '\0') break;
	}while(1);
	fseek(fp, -1, SEEK_CUR);

	//lê tudo até o próximo separador
	do{
		c = fgetc(fp);

		s = (char *) realloc(s, sizeof(char) * (counter+1));
		s[counter++] = c;
		
		if(feof(fp)) break;
		for(i = 0; sep[i] != c && sep[i] != '\0'; i++);
		if(sep[i] != '\0') break;				//se c é um separador. "if(sep[i] != '\0' || c == '\0') break;" permitiria \0 como separador.
	}while(1);
	s[counter-1] = '\0';

	if(!feof(fp)) fseek(fp, -1, SEEK_CUR);		//caso o usuário queira ler o separador, após ler a token atual.
	return s;
}


/*
 * Retorna o próximo número inteiro do arquivo, em variável int.
 * Retorna 0 caso falhe.
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
int readInt(FILE *fp){
	int result;
	char c;

	if(!fp) return 0;
	
	//Navega pelo arquivo até encontrar um dígito decimal.
	do{
		c = fgetc(fp);
		if(feof(fp)) return 0;
	}while(!isdigit(c));
	fseek(fp, -1, SEEK_CUR);

	fscanf(fp, "%d", &result);

	return result;
}


/*
 * Retorna o próximo número real do arquivo, com precisão float.
 * Retorna 0 caso não haja nenhum.
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
float readFloat(FILE *fp){
	float result;
	char c;

	if(!fp) return 0;

	//Navega pelo arquivo até encontrar um dígito decimal.
	do{
		c = fgetc(fp);
		if(feof(fp)) return 0;
	}while(!isdigit(c));
	fseek(fp, -1, SEEK_CUR);

	fscanf(fp, "%f", &result);

	return result;
}


/*
 * Retorna a próxima linha do arquivo, terminando em \n ou no fim do arquivo.
 */
#include <stdlib.h>
#include <stdio.h>
char *readLine(FILE *fp){
	int counter = 0;
	char c, *s = NULL;

	if(!fp) return NULL;

	do{
		c = fgetc(fp);
		if(feof(fp)) break;
		s = (char *) realloc(s, sizeof(char) * (counter+1));
		s[counter++] = c;
	}while(c != '\n');
	s[counter-1] = '\0';
		
	return s;
}

/*
 * Retorna o número de linhas no arquivo.
 */
#include <stdio.h>
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

/*
 * Conta quantos números, reais ou inteiros, há na primeira linha do arquivo.
 */
#include <stdio.h>
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
