#include <stdlib.h>
#include <stdio.h>
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

void print_array(int * array, int size)
{
    for ( int i = 0; i < size; i ++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

// vetores
//---------------------------------------------------------

int * get_array(int size)
{
    int *array;

    array = (int *) malloc(size * sizeof(int));
    for ( int i = 0; i < size; i++)
    {
        array[i] = 0;
    }

    return array;
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
int pixel_kernel_filter(int *pixels, int *kernel, int length, int coeficient)
{
    int soma;
    soma = 0;

    if (length == 0)
    {
        return 0;
    }
    
    for (int i = 0; i < length; i++)
    {
        soma += pixels[i] * kernel[i];
    }

    if ( soma == 0)
    {
        return 0;
    }
    return soma/coeficient;
}


int ** image_kernel_filter(int **image, int image_width, int image_height, int *kernel, int kernel_size)
{
    // o tamanho do kernel deve ser igual a kernel_size * kernel_size
    // kernel_size deve ser um numero impar >= 3

    int ** imagem_resposta;
    int *pixels;

    int coeficient, contador_pixels;

    pixels = (int *)malloc(kernel_size * kernel_size * sizeof(int));
    
    //inicializa matriz resposta
    imagem_resposta = get_2d_matrix(image_width, image_height);
    
    for (int i = 0; i < image_width; i++)
    {
        for ( int j = 0; j < image_height; j++)
        {
            coeficient = 0;
            contador_pixels = 0;
            //pega os valores para aplicar o filtro
            for (int k = i - 1; k < i - 1 + kernel_size; k++)
            {
                for ( int w = j-1; w < j - 1 + kernel_size; w++)
                {
                    //verifica se o pixel existe
                    if( k >= 0 && w >= 0 && k < image_width && w < image_height)
                    {
                        coeficient ++;
                        pixels[contador_pixels] = image[k][w];
                    }
                    else
                    {
                        pixels[contador_pixels] = 0;
                    }
                    contador_pixels++;
                }
            }

            //chama filtro no pixel
            int resposta = pixel_kernel_filter(pixels, kernel, kernel_size * kernel_size, coeficient);
            imagem_resposta[i][j] = resposta;

        }
    }

    return imagem_resposta;

}