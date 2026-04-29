#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

double get_temperature( int N, int maxIter , double radTemp){
    double start = omp_get_wtime();

    double tolerance = 1.0e-9;
    double heat_room[N*N];
    
    int below =floor(N*0.2);
    int upper = floor(N*0.8);

    for(int i=0; i<N*N; i++){
        heat_room[i] = 10;
    }

    for(int i=below; i<upper; i++){
        heat_room[(i+1)*N- 1] = radTemp;
    }

    double max_diff;
    for (int iter=0; iter<maxIter; iter++){
        max_diff = 0.0;

        #pragma omp parallel for simd reduction(max:max_diff) schedule(static) 
        for (int i=1; i<N-1;i++){
            int index = (i%2) + 1;
            for(int j=index; j<N-1; j+=2){
                double prev = heat_room[N*i + j];

                double down = heat_room[N*(i-1) + j];
                double up =  heat_room[N*(i+1) + j];
                double right = heat_room[N*i + (j+1)];
                double left = heat_room[N*i + (j-1)];

                heat_room[N*i + j] = (down + up + right + left) * 0.25;
                double difference = fabs(heat_room[N*i + j] - prev);
                max_diff = fmax(max_diff, difference);
            }
        }

        #pragma omp parallel for simd reduction(max:max_diff) schedule(static)
        for (int i=1; i<N-1;i++){
            int index = ((i+1)%2) + 1;
            for(int j=index; j<N-1; j+=2){
                double prev = heat_room[N*i + j];
                double down = heat_room[N*(i-1) + j];
                double up =  heat_room[N*(i+1) + j];
                double right = heat_room[N*i + (j+1)];
                double left = heat_room[N*i + (j-1)];

                heat_room[N*i + j] = (down + up + right + left) * 0.25;
                double difference = fabs(heat_room[N*i + j] - prev);
                max_diff = fmax(max_diff, difference);
            }
        }

        if (max_diff < tolerance){
            break;
        }
    }

    double (*t)[N] =(double (*)[N])heat_room;
    int pointx = floor((N-1)*0.5);
    int pointy = floor((N-1)*0.5);
    double result = t[pointx][pointy];
    double end = omp_get_wtime();
    printf("Total Time = %f secs\n", end - start);
    return result;
}