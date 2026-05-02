#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#define MIN(a,b) ((a) < (b) ? (a): b) // macro for comparison
/*Gets the temperature of the point in the center of the room after final iteration has been performed */
double get_temperature( int N, int maxIter , double radTemp){
    double TOLERANCE = 1.0e-9;
    double NORMAL_TEMP = 10.0;

    // Implement 1D array for faster populating
    // Populate the room points with normal temperature
    double heat_room[N*N];
    for(int i=0; i<N*N; i++){
        heat_room[i] = NORMAL_TEMP;
    }

    // Populate the radiator points
    for(int i=floor(N*0.2); i<floor(N*0.8); i++){
        heat_room[(i+1)*N- 1] = radTemp;
    }

    double max_diff;
    int block_size; //  block size for tiling the outer maxIter loop
    bool flag = false; // flag to exit the loops

    // Block size can be adjusted
    block_size = 64;
    for (int iter2=0; iter2<maxIter && !flag; iter2+=block_size){
        // Macro MIN used here
        for (int iter = iter2; iter<MIN(iter2 + block_size, maxIter) && !flag; iter++){

            max_diff = 0.0;
            // Switch from 1st pass to 2nd
            for(int m=0; m<2; m++){
                #pragma omp parallel for reduction(max:max_diff) schedule(static) // optmisation
                for (int i=1; i<N-1;i++){
                    int index = ((i + m)%2) + 1; // Column indices for the cells of the color
                    for(int j=index; j<N-1; j+=2){
                        double prev = heat_room[N*i + j];

                        double down = heat_room[N*(i-1) + j];
                        double up =  heat_room[N*(i+1) + j];
                        double right = heat_room[N*i + (j+1)];
                        double left = heat_room[N*i + (j-1)];

                        // Set the temperature of the current point in the room
                        heat_room[N*i + j] = (down + up + right + left) * 0.25;
                        double difference = fabs(heat_room[N*i + j] - prev);
                        max_diff = fmax(max_diff, difference);
                    }
                }
            }

            // Exit early if converged
            if (max_diff < TOLERANCE){
                flag = true;
            }
        }
    }

    // Convert from 1D to 2D array for final output
    double (*t)[N] =(double (*)[N])heat_room;
    int pointx = floor((N-1)*0.5);
    int pointy = floor((N-1)*0.5);
    double result = t[pointx][pointy];
    return result;
}

