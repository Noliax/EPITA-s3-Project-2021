#include <stdio.h>
#include <math.h>
#include "net.h"

//NEURON

size_t *seed = 2003;

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
    double t[_nb_inputs];
    _n->inputs = t;
    _n->nextInput = 0;
}

//NETWORK

size_t Net_neuron_id( size_t layer,
                        size_t number,
                        Network *net)
{  
    size_t sum = 0;
    for(size_t i = 0; i <= layer; i++)
        sum += net->layer_sizes[i];
    return sum + number;
}

void Neuron_print(Neuron *n){
    printf("\n**************\nInputs = ");
    double *beg = n->inputs;
    printf("*");
    for(; beg < n->inputs + n->nb_inputs; beg++)
        printf(", %4g", *beg);
    printf("\nOutputs = ");
    size_t *bg = n->outputs;
    for(; bg < n->outputs + n->nb_outputs; bg++)
        printf(", %zu", *bg);
    printf("\nBias = %4g\n");
}

void Net_feed_forward(size_t layer, Network *net)
{
    for(size_t i = 0; i < net->layer_sizes[layer]; i++){
        printf("Line 58\n");
        Neuron *n = net->neurons + Net_neuron_id(layer, i, net);
        Neuron_out(n);
        for(size_t i = 0; i < n->nb_outputs; i++){
            printf("Line 62\n");
            Neuron *next = &(net->neurons[Net_neuron_id(layer+1, n->outputs[i], net)]);
            printf("Line 64\n");
            Neuron_print(next);
            printf("nb_inputs= %zu\n", next->nb_inputs);
            printf("nextInput= %zu\n", next->nextInput);
            next->inputs[next->nextInput] = n->output;
            printf("Line 66\n");
            next->nextInput++;
        }
        n->nextInput = 0;
    }
}

void new_Network(Network *_n, 
                size_t _nb_layer,
                size_t _layer_sizes[],
                Neuron _neurons[])
{
    _n->nb_layer    = _nb_layer;
    _n->layer_sizes = _layer_sizes;
    _n->neurons = _neurons;
}

/*
void randomShuffle(double tab1[], double tab2[], size_t s){
    for(size_t i; i < s; i++){
        double t1 = tab1[i], t2 = tab2[i];
        tab1[i] = tab1[*seed%s];
        tab2[i] = tab2[*seed%s];
        tab1[*seed%s] = t1;
        tab2[*seed%s] = t2;
        *seed *= 73;
        *seed %= 20000;
    }
}

void SGD(double train_in[],
         double train_out[], 
         size_t train_size, 
         size_t epochs, 
         size_t mini_natch_size, 
         double eta)
{
    for(size_t i = 0; i < epochs; i++){
        randomShuffle(train_in, train_out, train_size);
        for(size_t j = 0; j < train_size; j++){
            
        }
    }
}
*/
