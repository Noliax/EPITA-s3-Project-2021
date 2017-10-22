#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "net.h"
#include <math.h>



double sigma(double x)
{
    return 1.0/(1.0 + exp(-x));
}

void print_array(double *arr, size_t size)
{
    printf("[%4g", arr[0]);
    for(size_t i = 1; i < size; i++)
        printf(", %4g", arr[i]);
    printf("]");
}

void print_Neuron(Neuron *n){
    printf("Neuron : ");
    printf("Output = %4g, Bias = %4g, Delta = %4g, Weights = ", n->output, n->bias, n->delta);
    print_array( n->weights, n->nb_in);
    printf("\n");
}

void print_Layer(Layer *l)
{
    printf("Layer : \n");
    for(size_t i = 0; i < l->size; i++){
        printf("        -> ");
        print_Neuron(&l->layer[i]);
    }
}

void print_Network(Network *n){
    printf("Network :\n");
    for(size_t i =0; i < n->nb_layer; i++)
        print_Layer(&n->net[i]);
    printf("---------------------\n");
}

void swap(double *a, double *b)
{
    double t = *a;
    *a = *b;
    *b = t;
}

void shuffle(double in[], double out[], size_t s)
{
    for(size_t i = 0;  i < s; i++)
    {
        int i = rand()%s;
        int j = rand()%s;
        swap(&in[i], &in[j]);
        swap(&out[i], &out[j]);
    }
}

void init_Neuron(Neuron *n, size_t nb_in){
    n->nb_in = nb_in;
    n->output = 0.0;
    n->delta = 0.0;
    n->bias = 1 - (double)(rand()%2000)/1000;
    n->weights = malloc(nb_in * sizeof(double));
    for(size_t i = 0; i < nb_in; i++)
        n->weights[i] = 1 - (double)(rand()%2000)/1000;
}

void init_Layer(Layer *l, size_t size)
{
    l->size = size;
    l->layer = malloc(size * sizeof(Neuron));
    for(size_t i = 0; i < size; i++)
        init_Neuron(&(l->layer[i]), 2);
}

void init_Network(Network *n, size_t nb_layer)
{
    n->nb_layer = nb_layer;
    n->net = malloc(nb_layer * sizeof(Layer));
    for(size_t i = 0; i < nb_layer; i++)
        init_Layer(&(n->net[i]), 2);
}

void input_Network(Network *n, double in, size_t id){
    n->net[0].layer[id].output = in;
}

void feed_forward_Network(Network *n)
{
    Layer *lay, *preclay;
    Neuron *neu;
    double sum, w, in;
    for(size_t l = 1; l < n->nb_layer; l++)
    {
        lay = &(n->net[l]);
        preclay = &(n->net[l-1]);
        for(size_t i = 0; i < lay->size; i++)
        {
            neu = &lay->layer[i];
            sum = 0;
            for(size_t j = 0; j < neu->nb_in; j++){
                w = neu->weights[j];
                in = preclay->layer[j].output;
                sum += w * in;
            }
        neu->output = sigma(sum - neu->bias);
        }
    }
}

void BWP_Network(Network *n, double out){
    Layer *lay, *nextlay;
    Neuron *curr;
    double newDelta, w, d;
    size_t depth = n->nb_layer - 1;
    lay = &(n->net[depth]);
    for(size_t i = 0; i < lay->size; i++)
        (&(lay->layer[i]))->delta = out - lay->layer[i].output;
    for(size_t l = 1; l <= depth-1; l++){
        lay = &(n->net[depth - l]);
        nextlay = &(n->net[depth - l + 1]);
        for(size_t i = 0; i < lay->size; i++){
            curr = &(lay->layer[i]);
            newDelta = 0.0;
            for(size_t j = 0; j < curr->nb_in; j++){
                d = nextlay->layer[j].delta;
                w = curr->weights[j];
                newDelta += d * w;
            }
            newDelta *= curr->output * (1.0 - curr->output);
            curr->delta = newDelta;
        }
    }
}

void weights_update_Network(Network *net, double rate){
    Layer *lay, *preclay;
    Neuron *neu;
    double w;
    for(size_t l = 1; l < net->nb_layer; l++)
    {
        lay = &(net->net[l]);
        preclay = &(net->net[l-1]);
        for(size_t i = 0; i < lay->size; i++)
        {
            neu = &lay->layer[i];
            for(size_t j = 0; j < neu->nb_in; j++){
                w = neu->weights[j];
                w += rate * preclay->layer[j].output * neu->delta;
                neu->weights[j] = w;
            }
        }
    }   
}
