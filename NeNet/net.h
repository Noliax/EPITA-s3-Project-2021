#include <stdio.h>
#include <stdlib.h>

struct Network{
    int[] sizes;
    double[] biases;
    double[] weights;
}
typedef struc Network Network;

Network newNetwork(int[] sizes);


size_t length(int arr[]){
    return sizeof(arr)/sizeof(int);
}
double sigma(double x);
