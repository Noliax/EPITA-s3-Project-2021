
#include <stdio.h>
#include <math.h>
#include "net.h"


void learn(&exemple, char ans){
	//FIXME
}

void shuffle(int *arr[]){
}

char read(&image){
	//FIXME
	return 'c';
}

double sigma(double x){
	return 1.0/(1.0*exp(-x));
}

Network init(){
    //FIXME
}

double feedForward(Network* n, double a){
    for(size_t i = 0; i < length(n->biases) && i < length(n->weights); i++){
        a = sigma((double)(n->weights[i]) * a + (double)(n->biases[i]));
    }
    return a;
}

void SGD(Network* n, training_data, int epochs, int mini_batch_size, int eta){
    int n = training_data.length;
    for(;epoches > 0; epoches--){
        shuffle(training_data);
        mini_batches
    }
}


