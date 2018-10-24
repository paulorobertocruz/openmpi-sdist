#include <stdlib.h>
#include <stdio.h>

typedef struct Int2DMatrix{
    int rows, cols;
    int **elements;
} Int2DMatrix;


int ** get_2d_matrix(int a, int b);
int ** get_transpose_2d_matrix(int **m, int r, int c);
void print_matrix(int **m, int r, int c);
int dot_product(int *a, int *b, int size);

int main(int *argc, char **argv)
{
    Int2DMatrix int_matriz_a;

    int **matriz_a, **matriz_b, **matriz_b_transposta, **matriz_resposta;
    int row_a, row_b, col_a, col_b;
    int contador;

    row_a = 2;
    col_a = 3;
    row_b = 3;
    col_b = 2;

    matriz_resposta = get_2d_matrix(row_a, col_b);
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

    matriz_b_transposta = get_transpose_2d_matrix(matriz_b, row_b, col_b);

    print_matrix(matriz_a, row_a, col_a);
    print_matrix(matriz_b, row_b, col_b);
    print_matrix(matriz_b_transposta, col_b, row_b);

    if (col_a != row_b)
    {
        printf("Não da para multiplicar matriz, dimensões erradas\n");
        return 0;
    }

    //multiplicar
    for(int i = 0; i < row_a; i++)
    {
        for (int j = 0; j < col_b; j++)
        {
            printf("%d:%d\n", i, j);
            matriz_resposta[i][j] = dot_product(matriz_a[i], matriz_b_transposta[j], col_a);
        }
    }

    print_matrix(matriz_resposta, row_a, col_b);
    return 0;
}


int ** get_2d_matrix(int a, int b)
{
    int **m;

    m = (int **) malloc( a * sizeof(int *) );
        
    for (int i = 0; i < a; i++)
    {
        m[i] = (int *) malloc(b * sizeof(int));

        for (int j = 0; j < b; j++)
        {
            m[i][j] = 0;
        }
    }

    return m;
}

int ** get_transpose_2d_matrix(int **m, int r, int c)
{
    int **matriz_transposta;
    matriz_transposta = get_2d_matrix(c, r);
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            matriz_transposta[j][i] = m[i][j];
        }
    }
    return matriz_transposta;
}

int dot_product(int *a, int *b, int size)
{
    int resultado;
    resultado = 0;
    for (int i = 0; i < size; i++)
    {
        resultado+= a[i] * b[i];
    }
    return resultado;
}

void print_matrix(int **m, int r, int c)
{
    printf("matrix:\n");
    for( int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}
