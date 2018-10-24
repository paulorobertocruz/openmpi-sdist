#include "sdist.h"

// utils
//---------------------------------------------------------
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

// vetores
//---------------------------------------------------------
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

int ** multply_matrix(int **matriz_a, int a_rows, int a_cols, int **matriz_b, int b_rows, int b_cols)
{
    int **matriz_b_transposta, **matriz_resposta;

    matriz_resposta = get_2d_matrix(a_rows, b_cols);
    matriz_b_transposta = get_transpose_2d_matrix(matriz_b, b_rows, b_cols);

    for(int i = 0; i < a_rows; i++)
    {
        for (int j = 0; j < b_cols; j++)
        {
            matriz_resposta[i][j] = dot_product(matriz_a[i], matriz_b_transposta[j], a_cols);
        }
    }

    return matriz_resposta;
}

// processamento de imagem 
//---------------------------------------------------------
int image_kernel_filter(int *image, int *kernel, int length)
{
    int soma;
    soma = 0;

    if (length == 0)
    {
        return 0;
    }
    
    for (int i = 0; i < length; i++)
    {
        soma+= image[i] * kernel[i];
    }

    return soma/length;
}