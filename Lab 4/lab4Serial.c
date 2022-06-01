#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 4096

/* Function Definitions */
double getRandom(double min, double max);
void transpose(double A[SIZE][SIZE], double C[SIZE][SIZE]);

int main(int arc, char* argv[]){

    int i, j;
    double A[SIZE][SIZE], C[SIZE][SIZE];
    double gigaflops;
    /* Time Variables */
    time_t timer1, timer2;
    timer1 = time(NULL);

    /* Populate Matrix */
    srand(time(NULL));
    for(i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++){
            A[i][j] = getRandom(1.0, 2.0);
        }
    }

    /* Transpose */
    transpose(A, C);

    /* Stop Clocks */
    timer2 = time(NULL);

    gigaflops = ((double) SIZE * SIZE * SIZE * 2.0 / 1000000000.0) / difftime(timer2, timer1);
    printf("Gigaflops: %lf\n", gigaflops);
}
    
double getRandom(double min, double max){
    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

void transpose(double A[SIZE][SIZE], double C[SIZE][SIZE]){
    int i, j, k;
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++){
            for (k = 0; k < SIZE; k++){
                C[ i ][ j ] += A[ k ][ i ] * A[ k ][ j ];
            }
        }
    }
}