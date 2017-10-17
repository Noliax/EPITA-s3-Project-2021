#include <stdio.h>
#include <math.h>
#include "net.h"

//NEURON

double sigma(double x)
{
    return 1.0/(1.0 - exp(-1.0 * x));
}

void Neuron_out(Neuron *n)
{ 
    n->output = sigma(dot(n->inputs, n->weights, n->nb_inputs));
}

double dot(double in[], double wei[], size_t s)
{
    double res = 0.0;
    for(size_t i = 0; i < s; i++)
        res += in[i] * wei[i];
    return res;
}

void new_Neuron(Neuron *_n,
                double _bias,
                double *_outputs,
                double *_weights,
                size_t _nb_inputs,
                size_t _nb_outputs)
{
    _n->outputs = _outputs;
    _n->nb_inputs = _nb_inputs;
    _n->nb_outputs = _nb_outputs;
    _n->bias = _bias;
    _n->weights = _weights;
    _n->inputs = malloc(_nb_inputs * sizeof(double));
    _n->nextInput = 0;
}

//NETWORK

void Net_access_neuron( size_t layer,
                        size_t number,
                        Network *net,
                        Neuron *ret_neuron)
{
    *ret_neuron = net->neurons[layer*net->nb_layer + number];
}

void Net_feed_forward(size_t layer, Network *net)
{
    for(size_t i = 0; i < net->layer_sizes[layer]; i++){
        Neuron *n;
        Net_access_neuron(layer, i, net, n);
        Neuron_out(n);
        if(layer < net->nb_layer-1)
        {
            Neuron next;
            Net_access_neuron(layer + 1, i, net, &next);
            next.inputs[n->nextInput++] = n->output;
        }
        n->nextInput = 0;
    }
}

void new_Network(Network *_n, 
                size_t _nb_layer,
                size_t _layer_sizes[],
                Neuron _neurons[])
{
    _n->nb_layer = _nb_layer;
    _n->layer_sizes = _layer_sizes;
    _n->neurons = _neurons;
}