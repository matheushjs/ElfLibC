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
