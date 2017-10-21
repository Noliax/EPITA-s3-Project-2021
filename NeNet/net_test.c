#include <stdio.h>
#include <stdlib.h>

#include "net.h"

int main(int argc, char *argv[])
{
    Network net;
    init_Network(&net, 3);
    input_Network(&net, 1.0, 0);
    input_Network(&net, 0.75, 1);
    print_Network(&net);
    feed_forward_Network(&net);
    print_Network(&net);
    printf("\n_____END_OF_EXECUTION_____\n");
    exit(0);
    return 0;
}
