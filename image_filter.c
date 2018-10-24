#include <stdio.h>
#include "sdist.h"

int main()
{
    printf("Filtro de imagem!\n");

    int **imagem, imagem_width, imagem_height, **imagem_filtrada, *kernel, kernel_size, kernel_length;

    kernel_size = 3;
    kernel_length = kernel_size * kernel_size;

    kernel = get_array(kernel_length);

    imagem_width = 24;
    imagem_height = 24;

    imagem = get_2d_matrix(imagem_width, imagem_height);

    print_matrix(imagem, imagem_width, imagem_height);
    
    imagem_filtrada = image_kernel_filter(imagem, imagem_width, imagem_height, kernel, kernel_size);

    print_matrix(imagem_filtrada, imagem_width, imagem_height);

    return 0;
}