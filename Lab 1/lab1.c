#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    /* data */
    int boxId, upperLeftY, upperLeftX, height, width, numTopNeighbors, numBottomNeighbors, numLeftNeighbors, numRightNeighbors;
    double boxDSV;
    int neighborsID[20];
} box;

int main(int arc, char* argv[]) 
{
    /* Loop Counter Variable */
    int i;
    double affect_rate = atof(argv[1]);
    double epsilon = atof(argv[2]);
    /* Maximum and Minimum DSV's */
    double maxDSV;
    double minDSV;
    /* Time Variables */
    time_t timer1, timer2;
    clock_t t;
    int iterations = 0;

    /* Start Clocks */
    timer1 = time(NULL);
    t = clock();

    /* Get Number of Boxes, Rows, and Cols */
    int numGridBoxes, numCols, numRows;  
    scanf("%u %u %u\n", &numGridBoxes, &numRows, &numCols);

    /* Grids to Hold boxes */
    box current[numGridBoxes];
    double compute[numGridBoxes];

    /* Read In Data */
    while(1){
        int boxNum;
        scanf("%d", &boxNum);

        if(boxNum == -1){
            break;
        }
        current[boxNum].boxId = boxNum;

        /* Coordinates */
        scanf("%u %u %u %u", &current[boxNum].upperLeftY, &current[boxNum].upperLeftX, &current[boxNum].height, &current[boxNum].width);

        /* Top Neighbors */
        scanf("%u", &current[boxNum].numTopNeighbors);
        int topNeighborsID[current[boxNum].numTopNeighbors];
        for(i = 0; i < current[boxNum].numTopNeighbors; i++){
            scanf("%u", &topNeighborsID[i]);
        }

        /* Bottom Neighbors */
        scanf("%u", &current[boxNum].numBottomNeighbors);
        int bottomNeighborsID[current[boxNum].numBottomNeighbors];
        for(i = 0; i < current[boxNum].numBottomNeighbors; i++){
            scanf("%u", &bottomNeighborsID[i]);
        }

        /* Left Neighbors */
        scanf("%u", &current[boxNum].numLeftNeighbors);
        int leftNeighborsID[current[boxNum].numLeftNeighbors];
        for(i = 0; i < current[boxNum].numLeftNeighbors; i++){
            scanf("%u", &leftNeighborsID[i]);
        }

        /* Right Neighbors */
        scanf("%u", &current[boxNum].numRightNeighbors);
        int rightNeighborsID[current[boxNum].numRightNeighbors];       
        for(i = 0; i < current[boxNum].numRightNeighbors; i++){
            scanf("%u", &rightNeighborsID[i]);
        }

        /* Populate Struct Array */
        for(i = 0; i < current[boxNum].numTopNeighbors; i++){
            current[boxNum].neighborsID[i] = topNeighborsID[i];
        }
        for(i = 0; i < current[boxNum].numBottomNeighbors; i++){
            current[boxNum].neighborsID[i + current[boxNum].numTopNeighbors] = bottomNeighborsID[i];
        }
        for(i = 0; i < current[boxNum].numLeftNeighbors; i++){
            current[boxNum].neighborsID[i + current[boxNum].numTopNeighbors + current[boxNum].numBottomNeighbors] = leftNeighborsID[i];
        }
        for(i = 0; i < current[boxNum].numRightNeighbors; i++){
            current[boxNum].neighborsID[i + current[boxNum].numTopNeighbors + current[boxNum].numBottomNeighbors + current[boxNum].numLeftNeighbors] = rightNeighborsID[i];
        }

        /* DSV */
        scanf("%lf", &current[boxNum].boxDSV);
    }

    /* Iterate over Grid */
    do{
        iterations++;
        int j;
        
        /* Calculate new DSV's */
        for (i = 0; i < numGridBoxes; i++){
            int j;
            /* Calculate WAAT */
            double waat = 0;
            
            /* Top Neighbors */
            for(j = 0; j < current[i].width; j++){
                int k;
                for(k = 0; k < current[i].numTopNeighbors; k++){
                    // Check if Y is the right level
                    if(current[current[i].neighborsID[k]].upperLeftY + current[current[i].neighborsID[k]].height == current[i].upperLeftY){
                        // Check if X's are adjacent
                        if(current[current[i].neighborsID[k]].upperLeftX <= current[i].upperLeftX + j && current[current[i].neighborsID[k]].upperLeftX + current[current[i].neighborsID[k]].width - 1 >= current[i].upperLeftX + j){
                            waat += current[current[i].neighborsID[k]].boxDSV;
                        }
                    }
                }
            }

            /* Bottom Neighbors */
            for(j = 0; j < current[i].width; j++){
                int k;
                int offset = current[i].numTopNeighbors;
                for(k = 0; k < current[i].numBottomNeighbors; k++){
                    if(current[current[i].neighborsID[k + offset]].upperLeftY == current[i].upperLeftY + current[i].height){
                        if(current[current[i].neighborsID[k + offset]].upperLeftX <= current[i].upperLeftX + j && current[current[i].neighborsID[k + offset]].upperLeftX + current[current[i].neighborsID[k + offset]].width - 1 >= current[i].upperLeftX + j){
                            waat += current[current[i].neighborsID[k + offset]].boxDSV;
                        }
                    }
                }
            }

            /* Left Neighbors */
            for(j = 0; j < current[i].height; j++){
                int k;
                int offset = current[i].numTopNeighbors + current[i].numBottomNeighbors;
                for(k = 0; k < current[i].numLeftNeighbors; k++){
                    if(current[current[i].neighborsID[k + offset]].upperLeftX + current[current[i].neighborsID[k + offset]].width == current[i].upperLeftX){
                        if(current[current[i].neighborsID[k + offset]].upperLeftY <= current[i].upperLeftY + j && current[current[i].neighborsID[k + offset]].upperLeftY + current[current[i].neighborsID[k + offset]].height - 1 >= current[i].upperLeftY + j){
                            waat += current[current[i].neighborsID[k + offset]].boxDSV;
                        }
                    }
                }
            }

            /* Right Neighbors */
            for(j = 0; j < current[i].height; j++){
                int k;
                int offset = current[i].numTopNeighbors + current[i].numBottomNeighbors + current[i].numLeftNeighbors;
                for(k = 0; k < current[i].numRightNeighbors; k++){
                    if(current[current[i].neighborsID[k + offset]].upperLeftX == current[i].upperLeftX + current[i].width){
                        if(current[current[i].neighborsID[k + offset]].upperLeftY <= current[i].upperLeftY + j && current[current[i].neighborsID[k + offset]].upperLeftY + current[current[i].neighborsID[k + offset]].height - 1 >= current[i].upperLeftY + j){
                            waat += current[current[i].neighborsID[k + offset]].boxDSV;
                        }
                    }
                }
            }
            

            /* Add Current Box DSV to WAAT if it's on an edge */
            if(current[i].numTopNeighbors == 0){
                waat += current[i].boxDSV * current[i].width;
            }
            if(current[i].numBottomNeighbors == 0){
                waat += current[i].boxDSV * current[i].width;
            }
            if(current[i].numLeftNeighbors == 0){
                waat += current[i].boxDSV * current[i].height;
            }
            if(current[i].numRightNeighbors == 0){
                waat += current[i].boxDSV * current[i].height;
            }           

            /* Adjust WAAT by Perimeter */
            double perimeter = 2 * (current[i].height + current[i].width);
            waat = waat / perimeter;
            
            /* Compute new DSV */
            if(waat > current[i].boxDSV){
                compute[i] = current[i].boxDSV + (waat - current[i].boxDSV) * affect_rate;
                
            }else{
                compute[i] = current[i].boxDSV - (current[i].boxDSV - waat) * affect_rate;
            }
        }

        /* Find Max and Min DSV */
        maxDSV = compute[0];
        minDSV = compute[0];
        for(i = 1; i < numGridBoxes; i++){
            if(compute[i] > maxDSV){
                maxDSV = compute[i];
            }else if(compute[i] < minDSV){
                minDSV = compute[i];
            }
        }

        /* Update Boxes */
        for(i = 0; i < numGridBoxes; i++){
            current[i].boxDSV = compute[i];
        }

    }while((maxDSV - minDSV) > (epsilon * maxDSV));

    /* Stop Clocks */
    timer2 = time(NULL);
    t = clock() - t;

    /* Print out data */
    printf("***********************************************************************\n");
    printf("Converged in %u iterations, with: \n\tmaxDSV: %f\n\tminDSV %f\n\tAffect Rate: %.2f\n\tEpsilon: %.2f\n", iterations, maxDSV, minDSV, affect_rate, epsilon);
    printf("Elapsed Convergence Time(clock): %u\nElapsed Convergence Time(Time): %f\n", t, difftime(timer2, timer1));
    printf("***********************************************************************\n");
}
