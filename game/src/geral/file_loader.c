#include "file_loader.h"

void limpa(FILE *entrada, char t);

/* Aloca uma matriz tridimensional de char. */
char ***alloc_tri_matrix(int x, int y, int z) {
    char ***M;
    int i, j;

    M = malloc(x * sizeof(char**));
    
    for(i = 0; i < x; i++) {
        M[i] = malloc(y * sizeof(char*));
        
        for(j = 0; j < y; j++) {
            M[i][j] = malloc(z * sizeof(char));
        }
    }
    return M;
}

/* Libera a alocação de memória para uma matriz tridimensional. */
void free_tri_matrix(char ***M, int x, int y) {
    int i, j;

    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            free(M[i][j]);
            M[i][j] = NULL;
        }

        free(M[i]);
        M[i] = NULL;
    }

    free(M);
    M = NULL;
}

/* limpa a entrada até encontrar o caracter t. */
void limpa(FILE *entrada, char t) {
    char c;
    do {
        // Verifica se o que foi lido é igual a -1, o que significa que acabou o file.
        if (fscanf(entrada, "%c", &c) == -1) {
            break;
        }
    } while(c != t);
}

/* Carrega os textos de um arquivo de texto e retorna como ponteiro para matriz tridimensional. 
Adiciona a quantidade de textos na matriz no ponteiro passado como parâmetro. */
char ***load_file(const char *path, int *n) {
    int i, j, k;
    char ***txt;
    char c;

    FILE *entrada = fopen(path, "r");
    if(!entrada) {
        fprintf(stderr, "Falha ao carregar o arquivo de texto.\n");
    }

    // Carrega o inteiro inicial que diz quantos textos serão alocados.
    fscanf(entrada, "%d", n);
    limpa(entrada, '\n');
    
    // Aloca os textos de estrutura fixa (n x 9 x 34).
    txt = alloc_tri_matrix(*n, 9, 34);
    
    for(i = 0; i < *n; i++) {
        for(j = 0; j < 9; j++) {
            for(k = 0; k < 34; k++) {
                // Lê caracter por caracter do file.
                fscanf(entrada, "%c", &c);
                
                if(c == '*'){
                    txt[i][j][k] = '\0';
                    break;
                } else {
                    txt[i][j][k] = c;
                }
            } 
            limpa(entrada, '\n');
        }
    }

    fclose(entrada);
    return txt;
}