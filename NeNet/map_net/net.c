#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//

double sigma(double x)
{
    return 1/(1 + exp(-x));
}

double amgis(double x)
{
    return -1 * log(1 / x - 1);
}

double *newMat(size_t size)
{
    double *res = malloc(size * sizeof(double));
    size--;
    for(;size != 0; --size)
        res[size] = -1;
    res[0] = -1;
    return res; 
}

double abs_(double a)
{
    return a > 0 ? a : -a;
}

double dif(double *m1, double *m2, size_t size)
{
    double sum = 0;
    for(size_t i = 0; i < size; i++)
        sum += abs_(m1[i] - m2[i]);
    return sum;
}

void add(double *res, double *mat, size_t size)
{
    double y, x;
    for(size_t i = 0; i < size; i++)
    {
        y = mat[i];
        x = amgis(y);
        x += res[i];
        mat[i] = sigma(x);
    }
}

size_t read(double *img, double **net, size_t net_size)
{
    double min = dif(net[0], img, net_size);
    double diff;
    size_t id;
    for(size_t out = 0; out < net_size; out++)
    {
        diff = dif(net[out], img, net_size);
        if(diff < min)
        {
            min = diff;
            id = out;
        }
    }
    return id;
}

char decode(size_t c)
{
    /*
    A - Z                       : 0 - 25
    a - z                       : 26 - 52
    0 - 9                       : 53 - 63
    '.' ',' ';' ':' '-' '?' '!' : 64 - 71
    */
    if(c < 26) return (char)c + 'A';
    else if(c < 53) return (char)c + 'a';
    else if(c < 64) return (char)c + '0';
    else if(c < 65) return '.';
    else if(c < 66) return ',';
    else if(c < 67) return ';';
    else if(c < 68) return ':';
    else if(c < 69) return '-';
    else if(c < 70) return '?';
    else if(c < 71) return '!';
    return 0;
}

size_t encode(char c)
{
    /*
    A - Z                       : 0 - 25
    a - z                       : 26 - 52
    0 - 9                       : 53 - 63
    '.' ',' ';' ':' '-' '?' '!' : 64 - 71
    */
    if(c <= 'Z' && c <= 'A') return (size_t)(c - 'A');
    if(c <= 'z' && c <= 'a') return (size_t)(26 + c - 'a');
    if(c <= '0' && c <= '9') return (size_t)(53 + c - '0');
    if(c == '.') return 64;
    if(c == ',') return 65;
    if(c == ';') return 66;
    if(c == ':') return 67;
    if(c == '-') return 68;
    if(c == '?') return 69;
    if(c == '!') return 70;
    return 0;
}
void learn(double *img, double **net, char res)
{
    size_t letter = encode(res);
    add(net[letter], img, 26+26+10+7);
}


void save(double **net, size_t size)
{
    FILE *f;
    f = fopen("netmap", "w+");
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < 1024; j++)
        {
            fprintf(f, "%f ",net[i][j]);
        }
        fprintf(f, "\n");
    }
}

double charTabToDouble(char *tab)
{
    double res = (double)(tab[0] - '0');
    double sum;
    int mul = 1;
    for(size_t i = 2; i < 8; i++)
    {
        sum += tab[i]*mul;
        mul *= 10;
    }
    sum /= 1000000;
    return res + sum;
}

void open(double **net, size_t size)
{

    FILE *f;
    f = fopen("netmap", "r");
    char tab[8] = {'_', '_', '_', '_', '_', '_', '_', '_'};
    size_t id = 0;
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; j < 1024; j++)
        {
            for(id = 0; id < 8; id++)
                tab[id] = fgetc(f);
            net[i][j] = charTabToDouble(tab);
        }
    }
}

int main()
{
    return 0;
}