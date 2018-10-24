#include <stdio.h>
#include "sdist.h"
int main(int *argc, char **argv)
{

    int **matriz_a, **matriz_b, **matriz_resposta;
    int row_a, row_b, col_a, col_b;
    int contador;

    row_a = 2;
    col_a = 3;
    row_b = 3;
    col_b = 2;

    matriz_a = get_2d_matrix(row_a, col_a);
    matriz_b = get_2d_matrix(row_b, col_b);

    contador = 0;
    for(int i = 0; i < row_a; i++)
    {
        for (int j = 0; j < col_a; j++)
        {
            matriz_a[i][j] = contador;
            contador ++;
        }
    }

    contador = 0;
    for(int i = 0; i < row_b; i++)
    {
        for (int j = 0; j < col_b; j++)
        {
            matriz_b[i][j] = contador;
            contador ++;
        }
    }

    print_matrix(matriz_a, row_a, col_a);
    print_matrix(matriz_b, row_b, col_b);

    if (col_a != row_b)
    {
        printf("Não da para multiplicar matriz, dimensões erradas\n");
        return 0;
    }

    matriz_resposta = multply_matrix(matriz_a, row_a, col_a, matriz_b, row_b, col_b);
    print_matrix(matriz_resposta, row_a, col_b);
    return 0;
}