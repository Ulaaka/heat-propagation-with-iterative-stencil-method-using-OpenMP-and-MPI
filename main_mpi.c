#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#include <mpi.h>
#include<time.h>

int read_dims(char *filename);
double *read_array(char *filename, int numOfValues);
void *write_to_output_file(char *filename, double *output, int numOfValues);
double get_temperature(int N, int maxIter, double radTemp);

int main(int argc, char *argv[]){

    int commSz, myRank;
    int root = 0;
    char *name = argv[3];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &commSz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Check the command line inputs
    if (argc != 5){
        printf("Command hand input size does not match");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int numOfValues;
    // Assign numOfValues first 
    if (myRank == root) {
        numOfValues = read_dims(name);
    }

    // Broadcast numOfValues to all ranks
    MPI_Bcast(&numOfValues, 1, MPI_INT, root, MPI_COMM_WORLD);

    // Declare 1D matrices
    double* array_temp = (double*)malloc(numOfValues * sizeof(double));
    double *result_array = (double*)malloc(numOfValues * sizeof(double));

    // Rank 0 sets up the problem by finding the batch size per process and assign array_temp
    int valuesPerProcess;
    if (myRank == root) {
        valuesPerProcess = numOfValues / commSz;

        if (numOfValues % commSz != 0) {
            valuesPerProcess++;
        }
        srand(time(NULL));
        array_temp =read_array(name, numOfValues);
    }

    // Broadcast valuesPerProcess to all ranks
    MPI_Bcast(&valuesPerProcess, 1, MPI_INT, root, MPI_COMM_WORLD);

    // Create local matrices
    double *local_array_temp = (double*)malloc(valuesPerProcess *  sizeof(double));
    double *local_result = (double*)malloc(valuesPerProcess *  sizeof(double));

    // Scatter the initialised matrices from rank 0 to all other ranks.
    MPI_Scatter(array_temp, valuesPerProcess, MPI_DOUBLE, local_array_temp,
    valuesPerProcess, MPI_DOUBLE, root, MPI_COMM_WORLD);

    int currV = 0;
    // Begin timing for distributed matrices from each rank
    double start_time = MPI_Wtime();
    while(currV < valuesPerProcess){
        double result = get_temperature(atoi(argv[1]), atoi(argv[2]), local_array_temp[currV]);
        local_result[currV] = result;
        currV++;
    }

    double end_time = MPI_Wtime();
    // End timing
    // Gather the results back
    MPI_Gather(local_result, valuesPerProcess, MPI_DOUBLE, result_array, valuesPerProcess, MPI_DOUBLE, root,MPI_COMM_WORLD);

    // Once the result is back
    if(myRank == root){
        printf("Total Time = %f secs\n", end_time - start_time);
        write_to_output_file(argv[4], result_array, numOfValues);
    }

    MPI_Finalize();
}

