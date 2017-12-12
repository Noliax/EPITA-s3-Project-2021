#ifndef _NET_H
#define _NET_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//

double Net_sigma(double x);

double Net_amgis(double y);

double *Net_newMat(size_t size);

double Net_abs_(double a);

double Net_dif(double *m1, double *m2, size_t size);

void Net_add(double *res, double *mat, size_t size);

char Net_read(double *img, double **net, size_t net_size);

char Net_decode(size_t c);

size_t Net_encode(char c);

void Net_learn(double *img, double **net, char res);

void Net_save(double **net, size_t size);

double Net_charTabToDouble(char *tab);

void Net_open(double **net, size_t size);

#endif