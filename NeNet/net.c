#include <stdio.h>
#include <stdlib.h>
#include "net.h"

//NEURON

void Neuron_init(Neuron *n, size_t _in, size_t _out, double _bias)
{
    n->nb_in = _in;
    n->nb_out = _out;
    n->bias = _bias;
    n->inputs = malloc(_in * sizeof(double));
    n->out_id = malloc(_out * sizeof(size_t));
    n->weights = malloc(_in * sizeof(double));
    n->nb_wei = _in;
}

double sigma(double x)
{
    return 1/(1 - exp(-x));
}

double dot(double w[], double i[], size_t l)
{
    double sum = 0;
    for(size_t j = 0; j < l; j++)
        sum += w[j] * i[j];
    return sum;
}

void Neuron_in(Neuron *n, double in)
{
    n->inputs[n->next_in] = in;
    n->next_in++;
}

void Neuron_out(Neuron *n)
{
n->output = sigma(
                    dot(
                        n->weights,
                        n->inputs,
                        n-> nb_in)
                    - n->bias);    
}

//NETWORK

void Net_init(Network *net,
              size_t _nb_layer,
              size_t *_layer_sizes)
{
    printf("\n*****-----*****\n");
    printf("<-<-<-***->->->\nnb_layer = %zu\n", net->nb_layer);
    net->nb_layer = _nb_layer;
    printf("<-<-<-***->->->\n");
    for(size_t i = 0; i < _nb_layer; i++)
        net->layer_sizes[i] = _layer_sizes[i];    
    //net->layer_sizes = _layer_sizes;
    size_t sum = 0;
    for(size_t i = 0; i < _nb_layer; i++)
        sum += _layer_sizes[i];
    //Neuron mat[sum];
    //net->mat = mat;
}

void Net_place(Network *net, Neuron *n, size_t layer, size_t i)
{
    *Net_access(net, layer, i) = *n;
}

Neuron* Net_access(Network *net, size_t layer, size_t i)
{
    size_t id = 0;
    for(size_t j = 0; j < layer; j++)
        id += net->layer_sizes[j];
    id += i;
    return &(net->mat[id]);
}

void Net_fire(Network *net, size_t layer)
{
    for(size_t i = 0; i < net->layer_sizes[layer]; i++)
    {
        Neuron *n = Net_access(net, layer, i);
        Neuron_out(n);
        for(size_t j= 0; j < n->nb_out; j++)
            Neuron_in(
                    Net_access(
                                net,
                                layer+1,
                                n->out_id[j]),
                    n->output);
        n->next_in = 0;
    }
}

void Net_feed_forward(Network *net)
{
    for(size_t i = 0; i < net->nb_layer; i++)
        Net_fire(net, i);
}












