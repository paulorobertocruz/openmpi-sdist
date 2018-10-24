#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <vector>
#include <string>

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#define NUM_ROWS(a) ARRAYSIZE(a)
#define NUM_COLS(a) ARRAYSIZE(a[0])

#define MASTER_RANK 0

std::string serialize_col(std::vector<std::vector<float>>, int col)
{
    int cols, rows;
    rows = NUM_ROWS(m);
    cols = NUM_COLS(m);
    std::string output = "";

    if ( col >= cols | col < 0)
    {
        std::cout << "Não pode serializar coluna inexistente." << std::endl;
        return "";
    }
    for ( int i = 0; i < rows; i++)
    {
       output.append(std::to_string(m[i][col]));
    }
    std::cout << output << std::endl;
    return output;

}

std::string serialize_row(float m)
{

}

int main(int argc, char** argv) {

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    bool *nos_usados;
    int i;
    
    nos_usados = (bool *)malloc(world_size * sizeof(bool));
    for (i = 0; i < world_size; ++i) {
         nos_usados[i] = false;
    }

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // no caso deste no ser o master
    if ( world_rank == MASTER_RANK)
    {
        printf("PROCESSO MESTRE\n");

        float **ma;
        ma = 
        = {
            {1,2,3},
            {4,5,6}
        };

        float mb[3][2] = {
            {1,2},
            {2,3},
            {4,5}
        };

        int linhas_a, linhas_b, colunas_a, colunas_b;
        linhas_a = NUM_ROWS(ma);
        colunas_a = NUM_COLS(ma);
        linhas_b = NUM_ROWS(mb);
        colunas_b = NUM_COLS(mb);

        serialize_col(mb, 1);
        
        printf("linhas a: %d, colunas a: %d, linhas b: %d, colunas b: %d\n", linhas_a, colunas_a, linhas_b, colunas_b);
        
        //somente multiplica se o numero de colunas da matriz a é igual a quantidade de linhas da matriz b
        if( colunas_a == linhas_b)
        {
            std::vector< std::vector<float>> matriz_resultado(linhas_a, std::vector<float>(colunas_b, 0));

            printf("Multiplicação Possivel\n");

            int linhas_a_i, colunas_b_j;

            for (linhas_a_i = 0; linhas_a_i < linhas_a; linhas_a_i++)
            {
                for ( colunas_b_j = 0; colunas_b_j < colunas_b; colunas_b_j++)
                {
                    //multiplica linha e coluna
                    printf("%d:%d\n", linhas_a_i, colunas_b_j);
                }
            }
        }
        else
        {
            printf("Multiplicação Não possivel\n");
        }

    }
    else
    {

    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
