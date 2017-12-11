#include "net.h"
#include "../Image/block.h"
#include "../Image/segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void printDoubleMat(double *mat)
{
    for(size_t i = 0; i < 1024; i++)
        printf("%2g%c", mat[i], i%32==0 ? '\0' : ' ');
    printf("\n");
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    char *path = "texte.png";

    size_t in_size = 32;
    size_t out_size = 26 + 26 + 10 + 7;
    double **network = malloc(out_size * sizeof(double*));
    
    for(size_t i = 0; i < out_size; i++)
    {
        network[i] = Net_newMat(in_size * in_size);
//        printDoubleMat(network[i]);
    }
    
    GtkWidget *img = gtk_image_new_from_file(path);
    int hsv = 8;
    int vsv = 30;
    int ahsv = 2;
    GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));

    struct BlockList *blocks = RLSA(buffer, hsv, vsv, ahsv);
    int **mat = BlocksToMat(buffer, blocks);
    size_t mat_size = blocks->size;
    double **doubleMat = malloc(mat_size * sizeof(double*));
    double *charmap;
    

    for(size_t i = 0; i < mat_size; i++)
    {
        if((size_t)(mat[i]) == 0 || (size_t)(mat[i]) == 1)
            doubleMat[i] = (double*)mat[i];
        else
        {
            charmap = Net_newMat(in_size * in_size);
            for(size_t j = 0; j < in_size * in_size; j++)
                charmap[j] = (double)mat[i][j];
            doubleMat[i] = charmap;
        }
    }

    
    char *text = "1 Au commencement, Dieu crea le ciel et la terre.\n2 La terre n'etait que chaos et vide";
    printf("%s\n",text);    
    for(size_t i = 0; i < 86; i++)
    {
        if((size_t)(doubleMat[i]) == 0 || (size_t)(doubleMat[i]) == 1)
            continue;
        else
        {
            Net_learn(doubleMat[i], network, text[i]);
            printf("%c", Net_read(doubleMat[i], network, out_size));
        }

    }
    printf("\n");
    char *result = malloc(86 * sizeof(char));
    for(size_t i = 0; i < 86; ++i)
    {
        if(doubleMat[i] == (void*)1)
            result[i] = ' ';
        else if(doubleMat[i] == (void*)0)
            result[i] = '\n';
        else
        {
            result[i] = Net_read(doubleMat[i], network, out_size);
        }
        printf("%c", result[i]);
    }
    printf("\n");
    exit(0);
    return 0;
}
