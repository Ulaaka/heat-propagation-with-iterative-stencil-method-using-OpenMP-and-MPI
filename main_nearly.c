#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int read_dims(char *filename);
double *read_array(char *filename, int numOfValues);
void *write_to_output_file(char *filename, double *output, int numOfValues);
double get_temperature(int N, int maxIter, double radTemp);

int main(int argc, char *argv[]){
    int numOfValues = read_dims(argv[3]);
    double* array_temp =read_array(argv[3], numOfValues);
    double result_array[numOfValues];

    // Begin timing
    double start = omp_get_wtime();
    // Loop through heats and store the result in result_array
    for (int i=0; i<numOfValues; i++){
        double result = get_temperature(atoi(argv[1]), atoi(argv[2]), array_temp[i]);
        result_array[i] = result;
    }

    double end = omp_get_wtime();
    // End Timing
    printf("Total Time = %f secs\n", end - start);

    // Write to the file
    write_to_output_file(argv[4], result_array, numOfValues);
}
