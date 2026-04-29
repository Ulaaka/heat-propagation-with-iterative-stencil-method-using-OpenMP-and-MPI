#include <stdio.h>
#include <stdlib.h>

int read_dims(char *filename);
double *read_array(char *filename, int numOfValues);
void *write_to_output_file(char *filename, double *output, int numOfValues);
double get_temperature(int N, int maxIter, double radTemp);

int main(int argc, char *argv[]){
    int numOfValues = read_dims(argv[0]);
    double* array =read_array(argv[0], numOfValues);

    double radTemp = 100.00;
    double result = get_temperature(atoi(argv[1]), atoi(argv[2]), radTemp);
    //write_to_output_file(argv[0], &result, numOfValues);

    printf("%lf", result);
}