#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

double get_temperature( int N, int maxIter , double radTemp){
    double tolerance = 1.0e-9;
    double heat_room[N*N];
    
    int below =floor(N*0.2);
    int upper = floor(N*0.8);

    //#pragma omp parallel for shared(heat_room) private(i)
    for(int i=0; i<N*N; i++){
        heat_room[i] = 10;
    }

    //#pragma omp parallel for shared(heat_room) private(i)
    for(int i=below; i<upper; i++){
        int index_room = (i+1)*N- 1;
        heat_room[index_room] = radTemp;
    }

    // Source - https://stackoverflow.com/a/9836551
    volatile bool flag=false;
    double max_diff;
    double start_time = omp_get_wtime();

    //#pragma omp parallel shared(max_diff, heat_room, flag)
    for (int iter=0; iter < maxIter && !flag; iter++){
        max_diff = 0;

        double difference, prev, down, up, right, left;

        //#pragma omp parallel for reduction(max:max_diff) schedule(static) private(difference, prev, down, up, right, left;)
        for (int i=1; i<N-1;i++){
            int index = (i%2) + 1;
            for(int j=index; j<N-1; j+=2){
                prev = heat_room[N*i + j];

                down = heat_room[N*(i-1) + j];
                up =  heat_room[N*(i+1) + j];
                right = heat_room[N*i + (j+1)];
                left = heat_room[N*i + (j-1)];

                heat_room[N*i + j] = (down + up + right + left) * 0.25;
                difference = fabs(heat_room[N*i + j] - prev);
                max_diff = fmax(max_diff, difference);
            }
        }

        //#pragma omp parallel for reduction(max:max_diff) schedule(static) private(difference, prev, down, up, right, left;)
        for (int i=1; i<N-1;i++){
            int index = (i+1)%2 + 1;
            for(int j=index; j<N-1; j+=2){
                prev = heat_room[N*i + j];
                down = heat_room[N*(i-1) + j];
                up =  heat_room[N*(i+1) + j];
                right = heat_room[N*i + (j+1)];
                left = heat_room[N*i + (j-1)];

                heat_room[N*i + j] = (down + up + right + left) * 0.25;
                difference = fabs(heat_room[N*i + j] - prev);
                max_diff = fmax(max_diff, difference);
            }
        }

        if (max_diff < tolerance){
            flag=true;
        }
    }
    double end_time = omp_get_wtime();
    printf("Total runtime = %f seconds\n", end_time - start_time);

    double (*t)[N] =(double (*)[N])heat_room;
    int pointx = floor((N-1)*0.5);
    int pointy = floor((N-1)*0.5);
    double result = t[pointx][pointy];

    return result;
}

