#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "net.h"
//

double Net_sigma(double x)
{
    return 2/(1 + exp(-x)) - 1;
}

double Net_amgis(double y)
{
    if(y > 0.99) y = 0.99;
    if(y < -0.99) y = -0.99;
    return -log(2/(y + 1) - 1);
}

double *Net_newMat(size_t size)
{
    double *res = malloc(size * sizeof(double));
    size--;
    for(;size != 0; --size)
        res[size] = 0;
    res[0] = 0;
    return res; 
}

double Net_abs_(double a)
{
    return a > 0 ? a : -a;
}

double Net_dif(double *m1, double *m2, size_t size)
{
    double sum = 0;
    for(size_t i = 0; i < size; i++)
        sum += Net_abs_((m1[i]) - (m2[i]));
    return sum;
}

void Net_add(double *letter, double *image, size_t size)
{
    double x, y;
    for(size_t i = 0; i < size; i++){
        x = Net_amgis(letter[i]);
        x += image[i];
        y = Net_sigma(x);
        letter[i] = y;
    }
}
/*
char Net_read(double *img, double **net, size_t net_size)
{
    double min = (Net_dif(net[0], img, net_size));
    double diff;
    size_t id = 0;
    for(size_t out = 0; out < net_size; out++)
    {
        diff = (Net_dif(net[out], img, net_size));
        if(diff < min)
        {
            min = diff;
            id = out;
        }
    }
    return Net_decode(id);
}
*/

char Net_read(double *img, double **net, size_t net_size)
{
    double diff, best;
    size_t bestID = 0;
    best = 10000;    
    for(size_t letter = 0; letter < net_size; letter++)
    {
        diff = 0;
        for(size_t i = 0; i < 1024; i++)
        {
            diff += Net_abs_(img[i] - net[letter][i]);
        }
        if(diff < best)
        {
            best = diff;
            bestID = letter;
        }
    }
    return Net_decode(bestID);
}

void Net_learn(double *img, double **net, char res)
{
    size_t letter = Net_encode(res);
    if(letter == 100) return;
    Net_add(net[letter], img, 32*32);
}

char Net_decode(size_t c)
{
    char table[26 + 26 + 10 + 7] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                                    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                                    '0','1','2','3','4','5','6','7','8','9','.',',',';',':','-','?','!'};
    return table[c];
}

size_t Net_encode(char c)
{
    if(c >= 'A' && c <= 'Z') return (size_t)(c - 'A');
    if(c >= 'a' && c <= 'z') return (size_t)(26 + c - 'a');
    if(c >= '0' && c <= '9') return (size_t)(52 + c - '0');
    if(c == '.') return 62;
    if(c == ',') return 63;
    if(c == ';') return 64;
    if(c == ':') return 65;
    if(c == '-') return 66;
    if(c == '?') return 67;
    if(c == '!') return 68;
    return 100;
}

//-----------

void Net_save(double **net, size_t size)
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

double Net_charTabToDouble(char *tab)
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

void Net_open(double **net, size_t size)
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
            net[i][j] = Net_charTabToDouble(tab);
        }
    }
}

