#include <stdlib.h>
#include <stdio.h>
#include <string.h>	//memset()
#include <float.h>	//FLT_MAX
#include <assert.h>	//assert()
#include <ctype.h>	//isalnum()

#define TRUE 1
#define FALSE 0
#define EPSILON 1E-5	//Tolerância de erro escolhida para as operações de ponto flutuante.

//PGM_type
typedef struct{
	int magic_number,
	    col,
	    row,
	    max;
	float **pixels;
} PGM_t;

typedef char bool;

//Lê uma matriz do arquivo de nome 'filename'
//Armazena as dimensões da matriz na variavel 'dim'
float **matrix_read(char *filename, int *dim){
	FILE *fp;
	int i, j;
	float **result;

	fp = fopen(filename, "r");
	assert(fp);

	fscanf(fp, "%d", dim);
	result = (float **) matrix_create(*dim, *dim, sizeof(float));
	for(i = 0; i < (*dim); i++)
		for(j = 0; j < (*dim); j++)
			fscanf(fp, "%f", &result[i][j]);

	fclose(fp);
	return result;
}

//Libera uma matriz de qualquer tipo, que tenha 'row' linhas
void matrix_free(void **m, int row){
	int i;
	if(m){
		for(i = 0; i < row; free(m[i++]));
		free(m);
	}
}

//Libera um registro PGM_t
void PGM_free(PGM_t *pgm){
	if(!pgm) return;
	
	matrix_free((void **) pgm->pixels, pgm->row);
	free(pgm);
}

//Lê, de um arquivo PGM, a próxima string útil e armazena em 'array', de tamanho 'a_size'.
//Função feita especificamente para a função PGM_read().
bool PGM_readWord(FILE *fp, char *array, int a_size){
	char c;
	int size = 0;

	assert(fp);

	//navega até encontrar um caracter alfanumérico.
	//Se encontrar um '#', navega até o fim da linha e continua a busca.
	do{
		c = fgetc(fp);
		if(feof(fp)) return FALSE;			//Não há nada para ler.
		if(c == '#'){
			while(fgetc(fp) != '\n');		//lê até o fim da linha
			continue;
		}
	}while(!isalnum(c));
	fseek(fp, -1, SEEK_CUR);

	//lê a string útil.
	//Se for um '#', posiciona o cursor nesse caracter.
	do{
		if(size > a_size) return FALSE;		//Se a 'array' dada é muito pequena.
		c = fgetc(fp);
		array[size++] = c;
	}while(isalnum(c) && !feof(fp));
	if(c == '#') fseek(fp, -1, SEEK_CUR);		//Poderia retroceder o cursor em qualquer caso. Desnecessário.
	array[size-1] = '\0';

	return TRUE;
}

// Entrada:
// 	filename: nome do arquivo .PGM que vai ser manipulado
// Retorna:
// 	registro PGM_t que caracteriza o arquivo .PGM 'filename'
PGM_t *PGM_read(char *filename){
	int i, j;
	FILE *fp;
	char aux[20];
	PGM_t *pgm;

	fp = fopen(filename, "r");
	assert(fp);

	pgm = (PGM_t *) malloc(sizeof(PGM_t));

	//Ler informações iniciais
	PGM_readWord(fp, aux, sizeof(aux));
	pgm->magic_number = atoi(aux+1);	//+1 para ignorar o caracter 'P'.

	PGM_readWord(fp, aux, sizeof(aux));
	pgm->col = atoi(aux);
	
	PGM_readWord(fp, aux, sizeof(aux));
	pgm->row = atoi(aux);

	PGM_readWord(fp, aux, sizeof(aux));
	pgm->max = atoi(aux);

	assert(!feof(fp));

	//Ler matriz de pixels
	pgm->pixels = (float **) malloc(sizeof(float *) * pgm->row);
	for(i = 0; i < pgm->row; i++){
		(pgm->pixels)[i] = (float *) malloc(sizeof(float) * pgm->col);
		for(j = 0; j < pgm->col; j++){
			PGM_readWord(fp, aux, sizeof(aux));
			pgm->pixels[i][j] = atoi(aux);
		}
	}

	fclose(fp);
	return pgm;
}

//Imprime em 'stdin' o registro PGM_t, com formatação padrão de arquivos PGM
void PGM_print(PGM_t *pgm){
	int i, j;
	
	assert(pgm);
	
	printf("P%d\n", pgm->magic_number);
	printf("%d %d\n", pgm->col, pgm->row);
	printf("%d\n", pgm->max);
	for(i = 0; i < pgm->row; i++){
		for(j = 0; j < pgm->col; j++){
			printf("%d ", (int) pgm->pixels[i][j]);
		}
		printf("\n");
	}
}

//Inverte todos os pixels da matriz de pixels do registro PGM_t dado como parâmetro.
void PGM_invert(PGM_t *pgm){
	int i, j;
	
	assert(pgm);
	for(i = 0; i < pgm->row; i++)
		for(j = 0; j < pgm->col; j++)
			pgm->pixels[i][j] = pgm->max - pgm->pixels[i][j] + EPSILON;	//EPSILON vai previnir truncamentos indevidos.
}

//Realiza, em uma submatriz de 'matrix', a operação de convolução, com base na matriz 'conv'.
//Essa submatriz tem dimensão 'dim', a mesma dimensão de 'conv'.
//O elemento (0,0) dessa submatriz tem coordenadas (init_row, init_col) em 'matrix'.
float submatrix_op(float **matrix, int init_row, int init_col, float **conv, int dim){
	int i, j, end_row, end_col;
	float result = 0;

	assert(matrix && conv);

	//Obter as coordenadas da ponta inferior direita da submatriz.
	end_row = init_row + dim - 1;
	end_col = init_col + dim - 1;

	//Operação de convolução.
	for(i = 0; i < dim; i++)
		for(j = 0; j < dim; j++)
			result += conv[i][j] * matrix[end_row-i][end_col-j];
	
	return result;
}

//Expande 'matrix', de dimensões row x col, em 'expansion' linhas e colunas em cada borda.
//Os elementos novos contêm o valor 0.
//A matriz resultante é do tipo float, por conveniência para a função PGM_convolution.
float **matrix_expand(float **matrix, int row, int col, int expansion){
	int i, j;
	float **result = (float **) matrix_create(row+(expansion*2), col+(expansion*2), sizeof(float));	//matrix_create já fornece uma matriz zerada

	for(i = 0; i < row; i++)
		for(j = 0; j < col; j++)
			result[i+expansion][j+expansion] = matrix[i][j];
	return result;
}

//Realiza a operação de convolução sobre 'pgm', utilizando a matriz de convolução 'conv' de dimensão 'conv_dim'.
void PGM_convolution(PGM_t *pgm, float **conv, int conv_dim){
	int i, j;
	float value, max_val = -FLT_MAX, min_val = FLT_MAX;
	float **expanded;
	bool **check;		//Vai nos dizer quais elementos precisam ser normalizados.

	assert(pgm && conv);

	//Obtêm a matriz expandida. conv_dim>>1 divide por 2 e trunca a parte fracionária, como desejado.
	expanded = matrix_expand(pgm->pixels, pgm->row, pgm->col, conv_dim>>1);

	//Obtêm matriz zerada de checagem.
	check = (bool **) matrix_create(pgm->row, pgm->col, sizeof(bool));

	//Início da convolução.
	for(i = 0; i < pgm->row; i++){
		for(j = 0; j < pgm->col; j++){
			value = submatrix_op(expanded, i, j, conv, conv_dim);
	
			//Obtêm dados para a normalização.
			if(value > max_val)
				max_val = value;
			else if(value < min_val)
				min_val = value;
	
			//Marca 'value' para ser normalizado ou não.
			if(value < 0 || value > pgm->max)
				check[i][j] = TRUE;

			pgm->pixels[i][j] = value;
		}
	}
	matrix_free((void **) expanded, pgm->row + conv_dim - 1);

	//Normalização
	for(i = 0; i < pgm->row; i++)
		for(j = 0; j < pgm->col; j++)
			if(check[i][j] != 0)
				pgm->pixels[i][j] = pgm->max*((pgm->pixels[i][j] - min_val)/(max_val - min_val));

	matrix_free((void **) check, pgm->row);
}

//Realiza a operação de floodfill no pgm dado.
//x e y são as coordenadas do pixel inicial.
//'col' é a cor com que se deseja preencher a imagem.
void PGM_floodfill(PGM_t *pgm, int x, int y, int col){
	static int base_col = -1;

	//condições de parada
	if(x < 0 || x >= pgm->col || y < 0 || y >= pgm->row) return;
	
	if(base_col == -1){
		base_col = (pgm->pixels)[y][x];
	}else{
		if((pgm->pixels)[y][x] != base_col ||
		   (pgm->pixels)[y][x] == col) return;
	}

	//ação principal
	//y representa linhas, x representa colunas
	(pgm->pixels)[y][x] = col;
	
	//passo recursivo
	PGM_floodfill(pgm, x+1, y, col);
	PGM_floodfill(pgm, x, y+1, col);
	PGM_floodfill(pgm, x-1, y, col);
	PGM_floodfill(pgm, x, y-1, col);
}
