#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Function Definitions */
double getRandom(double min, double max);
__global__ void transpose(double *h_a, double *d_a, int dimA);

int main(int arc, char* argv[]){

    int i;       // Loop Counter
    double *h_a;    // Host Memory Pointer
    double *d_a;    // Device Memory Pointer
    int dimA = 4096 * 4096; // Size of Array
    size_t memSize; // Memory Size
    double gigaflops;   // Gigaflops Store
    /* Thread Hierarchy Variables */
    int nBlocks = 1;
    int tPB = 1024;
    /* Time Variables */
    time_t timer1, timer2;
    timer1 = time(NULL);

    /* Allocate Memory */
    memSize = dimA * sizeof(double);
    h_a = (double*) malloc(memSize);
    cudaMalloc((void**) &d_a, memSize);

    /* Populate Matrix */
    srand(time(NULL));
    for(i = 0; i < dimA; i++){
        h_a[i] = getRandom(1.0, 2.0);
    }

    /* Copy to Device */
    cudaMemcpy(d_a, h_a, memSize, cudaMemcpyHostToDevice);

    /* Launch Kernel */
    dim3 dimGrid(nBlocks);
    dim3 dimBlock(tPB);
    transpose<<<dimGrid,dimBlock>>>(d_a, h_a, dimA);

    /* Copy Result */
    cudaMemcpy(h_a, d_a, memSize, cudaMemcpyDeviceToHost);

    /* Stop Clocks */
    timer2 = time(NULL);

    gigaflops = ((dimA * 4960.0 * 6.0) / 1000000000.0) / difftime(timer2, timer1);
    printf("Estimated GFlops/sec: %lf\n", gigaflops);
}
    
double getRandom(double min, double max){
    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

__global__ void transpose(double *d_a, double *h_a, int dimA){
    int i, j, k;
    i = blockIdx.x;
    j = threadIdx.x;
    int spot = (i * dimA) + j;
    for (k = 0; k < dimA; k++){
        d_a[spot] += h_a[(i * dimA) + k] * h_a[(k * dimA) + j];
    }
}