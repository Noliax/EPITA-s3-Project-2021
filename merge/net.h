#ifndef _NET_H
#define _NET_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//

double sigma(double x);

double amgis(double x);

double *newMat(size_t size);

double abs_(double a);

double dif(double *m1, double *m2, size_t size);

void add(double *res, double *mat, size_t size);

size_t Net_read(double *img, double **net, size_t net_size);

char decode(size_t c);

size_t encode(char c);

void Net_learn(double *img, double **net, char res);

void save(double **net, size_t size);

double charTabToDouble(char *tab);

void open(double **net, size_t size);

#endif
