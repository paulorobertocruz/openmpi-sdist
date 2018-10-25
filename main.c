#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sdist.h"

#define MASTER_RANK 0
#define TAG_OPERATION 0

int main(int argc, char** argv) {

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);

    if(MASTER_RANK == world_rank)
    {
        //master

        int **imagem, imagem_width, imagem_height, pixels;
        int **imagem_resultante;
        int *kernel, kernel_size, kernel_length;
        
        kernel_size = 3;
        kernel_length = kernel_size * kernel_size;

        int offset = 1;
        int pixels_por_no, pixels_sobra;

        pixels = imagem_width * imagem_height;
        pixels_por_no = pixels / (world_size - offset);
        pixels_sobra = pixels % (world_size - offset);

        imagem = get_2d_matrix(imagem_width, imagem_height);
        imagem_resultante = get_2d_matrix(imagem_width, imagem_height);
        kernel = get_array(kernel_length);

        int contador_pixels_global = 0;

        int total_pixels, pixel, x, y, send_array_size, *send_array, contador_send_array;

        int *pixels;

        int coeficient, contador_pixels;

        pixels = get_array(kernel_length);

        //não coloca o master na lista de nos a receber tarefas
        for( int i = offset; i < world_size, i++)
        {
            total_pixels = pixels_por_no;

            if( i == offset)
            {
                //coloca a sobra para o primeiro no da lista
                total_pixels += pixels_sobra;
            }

            send_array_size = (total_pixels * kernel_length + 3) + kernel_length + 2;
            send_array = get_array(send_array_size);
            send_array[0] = total_pixels;
            send_array[1] = kernel_length;
            contador_send_array = 2;

            for( int c = 0; c < kernel_length; c++)
            {
                send_array[c + contador_send_array] = kernel[c];
            }
            contador_send_array += kernel_length;

            for (int j = 0; j < total_pixels; j ++ )
            {
                pixel = j + contador_pixels_global;
                x = (pixel / imagem_width) - 1;
                y = (pixel / imagem_height) - 1;

                //armazena x, y
                send_array[contador_send_array] = x;
                contador_send_array ++;
                send_array[contador_send_array] = y; 
                contador_send_array ++;

                coeficient = 0;
                contador_pixels = 0;
                //pega os valores para aplicar o filtro
                for (int k = x - 1; k < x - 1 + kernel_size; k++)
                {
                    for ( int w = y-1; w < y - 1 + kernel_size; w++)
                    {
                        //verifica se o pixel existe
                        if( k >= 0 && w >= 0 && k < imagem_width && w < imagem_height)
                        {
                            coeficient++;
                            pixels[contador_pixels] = imagem[k][w];
                        }
                        else
                        {
                            pixels[contador_pixels] = 0;
                        }
                        contador_pixels++;
                    }
                }

                //armazena coeficiente para divisão
                send_array[contador_send_array] = coeficient;
                contador_send_array++;
                //armazena pixels e kernel para filtro
                for ( int f = 0; f < kernel_length; f ++)
                {
                    send_array[f + contador_send_array] = pixels[f];
                }
                contador_send_array += kernel_length;
            }

            contador_pixels_global += total_pixels;


            //manda o tamanho do array
            MPI_Send(&send_array_size, 1, MPI_INT, i, TAG_OPERATION, MPI_COMM_WORLD);

            //manda o array
            MPI_Send(&send_array, send_array_size, MPI_INT, i, TAG_OPERATION, MPI_COMM_WORLD);

        }

        // recebe respostas
        for( int i = offset; i < world_size, i++)
        {
            int recive_array_size, *recive_array;

            MPI_Recv(&recive_array_size, 1, MPI_INT, RANK_MASTER, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
            recive_array = get_array(recive_array_size);

            MPI_Recv(&recive_array[0], recive_array_size, MPI_INT, RANK_MASTER, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            

            int x, y, novo_pixel;

            for ( int j = 0; j < recive_array_size / 3; j++)
            {
                x = recive_array[j * 3];
                y = recive_array[(j * 3) + 1];
                novo_pixel = recive_array[(j * 3) + 2];
                imagem_resultante[x][y] = novo_pixel;
            }

        }

        //resultado
        print_matrix(imagem, imagem_width, imagem_height);

        print_matrix(imagem_resultante, imagem_width, imagem_height);

    }
    else
    {
        //slave

        int recive_array_size, *recive_array, *send_back_array, send_back_array_length, kernel_length, *kernel, total_pixels;

        MPI_Recv(&recive_array_size, 1, MPI_INT, RANK_MASTER, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        recive_array = get_array(recive_array_size);

        MPI_Recv(&recive_array[0], recive_array_size, MPI_INT, RANK_MASTER, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        total_pixels = recive_array[0];
        kernel_length = recive_array[1];
        kernel = get_array(kernel_length);

        //array de resposta
        send_back_array_length = 3 * total_pixels;
        send_back_array = get_array(send_back_array_length);

        for (int i = 0; i < kernel_length; i ++)
        {
            kernel[i] = recive_array[i + 1];
        }

        int contador_pixels, coeficiente, *pixels, contador_resposta, x, y;
        
        pixels = get_array(kernel_length);

        contador_pixels = kernel_length + 2;
        contador_resposta = 0;

        while( contador_pixels < recive_array_size)
        {
            x = recive_array[contador_pixels];
            y = recive_array[contador_pixels + 1];

            coeficiente = recive_array[contador_pixels + 2];
            for (int i = 0; i < kernel_length; i ++)
            {
                pixels[i] = recive_array[i + contador_pixels + 3];
            }
            //pixels, kernel, kernel_length, coeficiente

            int novo_pixel;
            novo_pixel = pixel_kernel_filter(pixels, kernel, kernel_length, coeficiente)

            send_back_array[contador_resposta] = x;
            send_back_array[contador_resposta] = y;
            send_back_array[contador_resposta] = novo_pixel;
            
            //incrementa contadores
            contador_resposta += 3;
            contador_pixels = kernel_length + 3;

        }

        //manda o tamanho do array
        MPI_Send(&send_back_array_length, 1, MPI_INT, MASTER_RANK, TAG_OPERATION, MPI_COMM_WORLD);

        //manda o array
        MPI_Send(&send_back_array, send_back_array_length, MPI_INT, MASTER_RANK, TAG_OPERATION, MPI_COMM_WORLD);

    }


    printf("Tem que existir no minimo 2 processos!");

    // Finalize the MPI environment.
    MPI_Finalize();
}
