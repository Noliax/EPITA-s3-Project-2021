#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _Neuron{
    size_t nb_in;
    double output;
    double bias;
    double delta;
    double *weights;
}Neuron;

typedef struct _Layer{
    size_t size;
    Neuron *layer;
}Layer;

typedef struct _Network{
    size_t nb_layer;
    Layer *net;
}Network;

double sigma(double x);

//Neuron

void init_Neuron(Neuron *n, size_t nb_in);

void print_array(double *arr, size_t size);

void print_Neuron(Neuron *n);

//Layer

void init_Layer(Layer *l, size_t size);

void print_Layer(Layer *l);

//Network

void init_Network(Network *net, size_t nb_layer);

void input_Network(Network *n, double in, size_t id);

void feed_forward_Network(Network *net);

void BWP_Network(Network *n, double out);

void print_Network(Network *n);

void weights_update_Network(Network *net, double rate);
#endif
