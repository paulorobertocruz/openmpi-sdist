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
    //printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);

    if(MASTER_RANK == world_rank)
    {
        //master

        int escravos;
        escravos = world_size - 1;

        printf("ESCRAVOS: %d\n", escravos);

        int **imagem;

        int imagem_width;
        imagem_width = 12;
        
        int imagem_height;
        imagem_height = 12;

        printf("WIDTH: %d HEIGHT: %d\n", imagem_width, imagem_height);

        imagem = get_2d_matrix(imagem_width, imagem_height);

        for ( int c = 0; c < imagem_width; c++)
        {
            for ( int v = 0; v < imagem_height; v++)
            {
                imagem[c][v] = 1;
            }
        }

        int **imagem_resultante;
        imagem_resultante = get_2d_matrix(imagem_width, imagem_height);

        printf("IMAGEM RESULTANTE ANTES:\n");
        print_matrix(imagem_resultante, imagem_width, imagem_height);

        int kernel_size, kernel_length;
        kernel_size = 3;
        kernel_length = kernel_size * kernel_size;

        int *kernel;
        kernel = get_array(kernel_length);

        for ( int c = 0; c < kernel_length; c++)
        {
            kernel[c] = c;
        }
        /*
        kernel[0] = 1;
        kernel[1] = 1;
        kernel[2] = 1;
        kernel[3] = 1;
        kernel[4] = 9;
        kernel[5] = 1;
        kernel[6] = 1;
        kernel[7] = 1;
        kernel[8] = 1;
        */
        printf("KERNEL:\n");
        print_array(kernel, kernel_length);

        int offset = 1;

        int pixels_da_imagem;
        pixels_da_imagem = imagem_width * imagem_height;

        printf("PIXELS: %d\n", pixels_da_imagem);
        
        int pixels_por_no;
        pixels_por_no = pixels_da_imagem / escravos;
        printf("PIXELS por NO: %d\n", pixels_por_no);
        
        int pixels_sobra;
        pixels_sobra = pixels_da_imagem % escravos;
        printf("PIXELS SOBRA: %d\n", pixels_sobra);

        int contador_pixels_global;
        contador_pixels_global = 0;

        int total_pixels, x, y, send_array_size, *send_array, contador_send_array;

        int *pixels;

        int coeficient, contador_pixels;

        pixels = get_array(kernel_length);

        //não coloca o master na lista de nos a receber tarefas
        for( int i = offset; i < world_size; i++)
        {
            total_pixels = pixels_por_no;

            if( i == offset)
            {
                //coloca a sobra para o primeiro no da lista
                total_pixels += pixels_sobra;
            }

            send_array_size = (total_pixels * (kernel_length + 3)) + kernel_length + 2;

            send_array = get_array(send_array_size);
            
            send_array[0] = total_pixels;
            send_array[1] = kernel_length;

            contador_send_array = 2;

            //carrega kernel no array
            for( int c = 0; c < kernel_length; c++)
            {
                send_array[c + contador_send_array] = kernel[c];
            }
            contador_send_array += kernel_length;

            for (int j = 0; j < total_pixels; j ++ )
            {
                int pixel;
                pixel = j + contador_pixels_global;

                x = (pixel / imagem_width);
                y = (pixel % imagem_height);

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
            MPI_Send(send_array, send_array_size, MPI_INT, i, TAG_OPERATION, MPI_COMM_WORLD);

        }

        // recebe respostas
        for( int i = offset; i < world_size; i++)
        {
            int recive_array_size, *recive_array;

            MPI_Recv(&recive_array_size, 1, MPI_INT, i, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
            recive_array = get_array(recive_array_size);

            MPI_Recv(&recive_array[0], recive_array_size, MPI_INT, i, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            

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
        printf("RESULTADOS:\n");
        
        printf("Imagem Original:\n");
        print_matrix(imagem, imagem_width, imagem_height);

        printf("Imagem Resultante:\n");
        print_matrix(imagem_resultante, imagem_width, imagem_height);

    }
    else
    {
        //slave
        int recive_array_size, *recive_array, *send_back_array, send_back_array_length, kernel_length, *kernel, total_pixels;

        MPI_Recv(&recive_array_size, 1, MPI_INT, MASTER_RANK, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        recive_array = get_array(recive_array_size);

        MPI_Recv(&recive_array[0], recive_array_size, MPI_INT, MASTER_RANK, TAG_OPERATION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        
        total_pixels = recive_array[0];

        kernel_length = recive_array[1];

        kernel = get_array(kernel_length);

        int pixel_offset;
        pixel_offset = kernel_length + 2; // 1 para total de pixels + 1 para tamanho do kernel + kernel_length para o kernel 

        int pixel_multiple;
        pixel_multiple = kernel_length + 3; // 1 para x + 1 para y + 1 para coeficiente + kernel_length para pixels

        //array de resposta
        send_back_array_length = 3 * total_pixels;
        send_back_array = get_array(send_back_array_length);

        for (int i = 0; i < kernel_length; i ++)
        {
            kernel[i] = recive_array[i + 2];
        }
        
        int coeficiente, *pixels, x, y;
        
        pixels = get_array(kernel_length);

        int local_offset = 0;

        for ( int cp = 0; cp < total_pixels; cp++)
        {
            local_offset = pixel_offset + (pixel_multiple * cp);
            x = recive_array[local_offset];
            y = recive_array[local_offset + 1];

            coeficiente = recive_array[local_offset + 2];

            for (int i = 0; i < kernel_length; i ++)
            {
                pixels[i] = recive_array[i + local_offset + 3];
            }
            //pixels, kernel, kernel_length, coeficiente
            int novo_pixel;
            novo_pixel = pixel_kernel_filter(pixels, kernel, kernel_length, coeficiente);
            send_back_array[(cp * 3)] = x;
            send_back_array[(cp * 3) + 1] = y;
            send_back_array[(cp * 3) + 2] = novo_pixel;
        }

        //manda o tamanho do array
        MPI_Send(&send_back_array_length, 1, MPI_INT, MASTER_RANK, TAG_OPERATION, MPI_COMM_WORLD);

        //manda o array
        MPI_Send(send_back_array, send_back_array_length, MPI_INT, MASTER_RANK, TAG_OPERATION, MPI_COMM_WORLD);

    }
    // Finalize the MPI environment.
    MPI_Finalize();
}
