#include <stdio.h>
#include <stdlib.h>

#include "net.h"

int main(int argc, char *argv[])
{
    Network *net;
    size_t sizes[] = {2, 1};
    Net_init(net, (size_t)2, sizes);
    return 0;
}
