#include <stdio.h>
#include "sdist.h"

int main()
{
    printf("Filtro de imagem!\n");

    int **imagem, imagem_width, imagem_height, **imagem_filtrada, *kernel, kernel_size, kernel_length;

    kernel_size = 3;
    kernel_length = kernel_size * kernel_size;

    kernel = get_array(kernel_length);

    for ( int i = 0; i < kernel_length; i++)
    {
        kernel[i] = i;
    }

    imagem_width = 12;
    imagem_height = 12;

    print_array(kernel, kernel_length);
    imagem = get_2d_matrix(imagem_width, imagem_height);
    
    for ( int i = 0; i < imagem_width; i++)
    {
        for ( int j = 0; j < imagem_height; j ++)
        {
            imagem[i][j] = 1;
        }
    }

    print_matrix(imagem, imagem_width, imagem_height);
    
    imagem_filtrada = image_kernel_filter(imagem, imagem_width, imagem_height, kernel, kernel_size);

    print_matrix(imagem_filtrada, imagem_width, imagem_height);

    return 0;
}