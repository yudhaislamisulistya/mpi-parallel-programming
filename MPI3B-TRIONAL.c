#include "/opt/homebrew/Cellar/open-mpi/4.1.4_2/include/mpi.h"
#include <stdio.h>
#include <stdlib.h>
int size, rank;
int entered_number;
int total;

void get_user_information(){
    printf("enter number \n");
    scanf("%d",&entered_number);
}

int sum(int first, int count) {
    int i, total = 0;
    for (i = 0; i < count; i++) total += first+i;
    return total;
}

void communication(){
    
    int j;
    
    if (rank == 0) { // Master process will send the data to all processes including itself
        get_user_information();
        
        int count = entered_number/size;
        for (j = 0; j < size; j++) MPI_Send(&count, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
    
    }
    
    int count = 0;
    MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int first = count * rank + 1;
    int total = sum(first, count);
    MPI_Send(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    
    if (rank == 0) { // Master process will gather all results
        for (j = 1; j < size; j++) {
            int subtotal = 0;
            MPI_Recv(&subtotal, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += subtotal;
        }
        printf("total => %d\n",total);
    }
}

int main(int argc, char **argv) {
    
   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    communication();
   
    MPI_Finalize();
   // return 0;
}