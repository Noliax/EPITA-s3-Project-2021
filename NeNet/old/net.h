#ifndef NET_H
#define NET_H 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _Neuron{
    size_t nb_inputs, nb_outputs, nextInput;
    double  output, bias;
    size_t *outputs;
    double *inputs, *weights;
}Neuron;

double sigma(double x);

void Neuron_out(Neuron *neuron);

void new_Neuron(Neuron *_n,
                double _bias,
                double *_outputs,
                double *_weights,
                size_t _nb_inputs,
                size_t _nb_outputs);
;

double dot(double in[], double out[], size_t s);



typedef struct _Network{
    size_t nb_layer;
    size_t *layer_sizes;
    Neuron *neurons;
}Network;

void Net_access_neuron( size_t layer,
                        size_t number,
                        Network *net,
                        Neuron *ret_neuron);

void Net_feed_forward(size_t layer, Network *net);

void randomShuffle(double tab1[], double tab2[], size_t s);

void new_Network(Network *n, 
                size_t _nb_layer,
                size_t _layer_sizes[],
                Neuron _neurons[]);

void SGD(double train_in[],
         double train_out[], 
         size_t train_size, 
         size_t epochs, 
         size_t mini_natch_size, 
         double eta);
#endif
