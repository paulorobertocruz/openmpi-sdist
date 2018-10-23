#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 10

int main(int* argc, char** argv)
{
    bool *nos;
    int i;
    
    nos = (bool *) malloc( N * sizeof(bool));
    for (i = 0; i < N; i++) {
         nos[i] = false;
    }

    nos[3] = true;

    for(i=0; i < N; i++){
        if (nos[i] == true)
        {
            printf("TRUE\n");
        }
        else
        {
            printf("FALSE\n");
        }
    }

    return 0;
}