#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>

#define MASTER_RANK 0

int main(int argc, char** argv) {

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    bool *nos_usados;
    int i;
    
    nos_usados = (bool *)malloc(world_rank * sizeof(bool));
    for (i = 0; i < world_rank; ++i) {
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
        char ma[3][256];
        char mb[5][256];

        strcpy(ma[0], "1 2 3 4 5");
        strcpy(ma[1], "6 7 8 9 10");
        strcpy(ma[2], "11 12 13 14 15");

        strcpy(mb[0], "1 2 3");
        strcpy(mb[1], "4 5 6");
        strcpy(mb[2], "7 8 9");
        strcpy(mb[3], "10 11 12");
        strcpy(mb[4], "13 14 15");


    }
    else
    {

    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
