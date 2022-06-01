#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[]){
    /* Work Variables */
    int i, j, rank, size;
    int iterations = 1000000;
    double pp_time, bandwidth, *A, *B;
    int msgSize[5] = {32, 256, 512, 1024, 2048};
    /* Clocks */
    clock_t clkbegin, clkend, tott;
    /* Setup MPI */
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Iterate through msgSize */
    for(i = 0; i < 5; i++){
        /* Malloc Arrays */
        A = (double*)malloc(msgSize[i] * sizeof(double));
        B = (double*)malloc(msgSize[i] * sizeof(double));
        /* Populate Array with Random Values */
        for(j = 0; j < msgSize[i]; j++){
            A[j] = (double)rand();
            A[j] = (double)rand();
        }

        /* Barrier */
        MPI_Barrier(MPI_COMM_WORLD);

        /* Start Clock */
        clkbegin = clock();

        for(j = 0; j < iterations; j++){
            if(rank == 0){
                MPI_Send(A, msgSize[i], MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(B, msgSize[i], MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
                MPI_Send(B, msgSize[i], MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(A, msgSize[i], MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
            }else{
                MPI_Recv(A, msgSize[i], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(A, msgSize[i], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Recv(B, msgSize[i], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(B, msgSize[i], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }

        /* Barrier */
        MPI_Barrier(MPI_COMM_WORLD);

        /* Stop Clock */
        clkend = clock();
        /* Get Bandwidth */
        tott = clkend - clkbegin;
        pp_time = (double) tott / (4 * iterations);
        bandwidth = (sizeof(double) * msgSize[i]) / pp_time;
        /* Print out */
        printf("Message Size: %d, Process: %d, Elapsed Time: %lf seconds, Bandwidth: %lf\n", msgSize[i], rank, pp_time, bandwidth);
        
        /* Free Memory */
        free(A);
        free(B);
    }
    MPI_Finalize();
}