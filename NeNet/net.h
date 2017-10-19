#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//NEURON
typedef struct _Neuron{
    double *inputs, output, *weights, bias;
    size_t nb_in, nb_out, nb_wei, *out_id, next_in;
} Neuron;

double sigma(double x);

double dot(double w[], double i[], size_t l);

void Neuron_init(Neuron *n, size_t _in, size_t _out, double _bias);

void Neuron_in(Neuron *n, double in);

void Neuron_out(Neuron *n);

//NETWORK

typedef struct _Network{
    size_t nb_layer;
    size_t layer_sizes[15];
    Neuron mat[10000];
} Network;

void Net_init(Network *net, size_t _nb_layer, size_t *_layer_sizes);

Neuron* Net_access(Network *net, size_t layer, size_t i);

void Net_fire(Network *net, size_t layer);

void Net_feed_forward(Network *net);

#endif
