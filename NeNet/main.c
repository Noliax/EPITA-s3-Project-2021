#include "net.h"
#include "../Image/block.h"
#include "../Image/segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    char *path = "texte.png";

    size_t in_size = 32;
    size_t out_size = 26 + 26 + 10 + 7;
    double **network = malloc(out_size * sizeof(double*));
    
    for(size_t i = 0; i < out_size; i++)
    {
        network[i] = newMat(in_size * in_size);
    }
    
    GtkWidget *img = gtk_image_new_from_file(path);
    int hsv = 8;
    int vsv = 30;
    int ahsv = 2;
    GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));

    struct BlockList *blocks = RLSA(buffer, hsv, vsv, ahsv);
    int **mat = BlocksToMat(buffer, blocks);
    size_t mat_size = sizeof(mat)/sizeof(double);
    double **doubleMat = malloc(mat_size * sizeof(double*));
    double *charmap;
    
    for(size_t i = 0; i < mat_size; i++)
    {
        if(mat[i] == (void*)0 || mat[i] == (void*)1)
            doubleMat[i] = (double*)mat[i];
        else
        {
            charmap = newMat(in_size * in_size);
            for(size_t j = 0; j < in_size; j++)
                charmap[j] = mat[i][j];
            doubleMat[i] = charmap;
        }
    }

    char *text = "1 Au commencement, Dieu créa le ciel et la terre.\n2 La terre n'etait que chais et vide";
    
    for(size_t i = 0; i < 88; i++)
    {
        if(doubleMat[i] == (void*)0 || doubleMat[i] == (void*)1)
            continue;
        else
        {
            Net_learn(doubleMat[i], network, text[i]);
            printf("%c",text[i]);
        }
    }
    printf("\n-----\n");
    char *result = malloc(88 * sizeof(char));
    for(size_t i = 0; i < 88; ++i)
    {
        if(doubleMat[i] == (void*)0)
            result[i] = ' ';
        else if(doubleMat[i] == (void*)1)
            result[i] = '\n';
        else
        {
            result[i] = Net_read(doubleMat[i], network, out_size);
        }
        printf("%c", result[i]);
    }

    return 0;
}
