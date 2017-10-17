#include <stdio.h>
#include <stdlib.h>
#include "net.h"

int main()
{
    Neuron A, B, C;
    double outA[] = {0};
    double outB[] = {0}; 
    double *outC = NULL;
    double weiA[] = {1.0, 1.0}; 
    double weiB[] = {-1.0, -1.0}; 
    double weiC[] = {1.0, 1.0};
    new_Neuron(&A, 0.5, outA, weiA, 2, 1);
    new_Neuron(&B, -1.5, outA, weiA, 2, 1);
    new_Neuron(&C, 1.5, outA, weiA, 2, 1);
    Network net;
    Neuron neu[] = {A, B, C};
    size_t sizes[] = {2, 1};
    new_Network(&net, 2, sizes, neu);
    double inputs[] = {0, 1};
    A.inputs = inputs;
    B.inputs = inputs;
    Net_feed_forward(0, &net);
    printf("%d\n", C.outputs[0]);
    return 0;
}

