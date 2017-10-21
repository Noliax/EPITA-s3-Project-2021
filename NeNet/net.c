#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "net.h"
#include <math.h>



double sigma(double x)
{
    return 1.0/(1.0 + exp(-x));
}

double *doubleArray(size_t sx, double val)
{
    double *mat = malloc(sx * sizeof(double));
    for(size_t i = 0; i < sx; i++)
        mat[i] = val;
    return mat;
}

double *Array_random(size_t sx)
{
    double *ret = malloc(sx * sizeof(double));
    for(size_t i = 0; i < sx; i++)
        ret[i] = 1-(double)(rand()%1000)/1000*2;
    return ret;
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
    printf("Output = %4g, Bias = %4g, Weights =", n->output, n->bias);
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
    //print_Neuron(n);
}

void init_Layer(Layer *l, size_t size)
{
    l->size = size;
    l->layer = malloc(size * sizeof(Neuron));
    for(size_t i = 0; i < size; i++)
        init_Neuron(&(l->layer[i]), 2);
    //print_Layer(l);
}

void init_Network(Network *n, size_t nb_layer)
{
    n->nb_layer = nb_layer;
    n->net = malloc(nb_layer * sizeof(Layer));
    for(size_t i = 0; i < nb_layer; i++)
        init_Layer(&(n->net[i]), 2);
    //print_Network(n);

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



/*
void update_mini_batch(size_t begin,
                       size_t end,
                       double train_in[],
                       double train_out[])
{
    
}

void SGD(double train_in[], double train_out[], size_t train_size
        ,size_t epochs, size_t mini_batch_size, double eta)
{
    for(size_t j = 0; j < epochs; j++){
        shuffle(train_in, train_out, train_size);
        for(size_t k = 0; k < train_size; k+= mini_batch_size)
            continue;
    }
}
*/
