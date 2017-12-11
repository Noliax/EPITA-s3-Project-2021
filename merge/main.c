#include "net.h"
#include "../Image/block.h"
#include "../Image/segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void printDoubleMat(double *mat)
{
    for(size_t i = 0; i < 1024; i++)
        printf("%c%c", mat[i] == -1 ? '.' : '#', i%32==0 ? '\n' : ' ');
    printf("\n");
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    char *path = "text2.png";

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
    printf("\n%zu\n", mat_size);
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
            printDoubleMat(doubleMat[i]);
        }
    }
    
    char *text = "C general-purpose, imperative computer programming language, supporting structured programming, lexical variable scope and recursion, while a static type system prevents many unintended operations. By design, C provides constructs that map efficiently to typical machine instructions, and therefore it has found lasting use in applications that had formerly been coded in assembly language, including operating systems, as well as various application software for computers ranging from supercomputers embedded systems.\nC was originally developed by Dennis Ritchie between 1969 and 1973 at Bell Labs, and used to re-implement the Unix operating system. It has since become one of the most widely used programming languages of all time, with C compilers from various vendors available for the majority of existing computer architectures and operating systems. C has been standardized by the American National Standards Institute since 1989 and subsequently by the International Organization for Standardization.\nC is an imperative procedural language. It was designed to be compiled using a relatively straightforward compiler, to provide low-level access to memory, to provide language constructs that map efficiently to machine instructions, and to require minimal run-time support. Despite its low-level capabilities, the language was designed to encourage cross-platform programming. A standards-compliant and portably written C program can be compiled for a very wide variety of computer platforms and operating systems with few changes to its source code. The language has become available on a very wide range of platforms, from embedded microcontrollers to supercomputers.";
    printf("%s\n",text);
    for(size_t i = 0; i < mat_size; i++)
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
    char *result = malloc(mat_size * sizeof(char));
    for(size_t i = 0; i < mat_size; ++i)
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
