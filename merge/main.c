#include "net.h"
#include "../Image/block.h"
#include "../Image/segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void printDoubleMat(double *mat)
{
    for(size_t i = 0; i < 1024; i++)
        printf("%d%c", mat[i]==-1, i%32==0 ? '\n' : ' ');
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
//    printf("\n%zu\n", mat_size);
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
//            printDoubleMat(doubleMat[i]);
        }
    }
    
    char *text = "C general-purpose, imperative computer programming language, supporting structured programming, lexical variable scope and recursion, while a static type system prevents many unintended operations. By design, C provides constructs that map efficiently to typical machine instructions, and therefore it has found lasting use in applications that had formerly been coded in assembly language, including operating systems, as well as various application software for computers ranging from supercomputers embedded systems. \nC was originally developed by Dennis Ritchie between 1969 and 1973 at Bell Labs, and used to re-implement the Unix operating system. It has since become one of the most widely used programming languages of all time, with C compilers from various vendors available for the majority of existing computer architectures and operating systems. C has been standardized by the American National Standards Institute since 1989 and subsequently by the International Organization for Standardization. \nC is an imperative procedural language. It was designed to be compiled using a relatively straightforward compiler, to provide low-level access to memory, to provide language constructs that map efficiently to machine instructions, and to require minimal run-time support. Despite its low-level capabilities, the language was designed to encourage cross-platform programming. A standards-compliant and portably written C program can be compiled for a very wide variety of computer platforms and operating systems with few changes to its source code. The language has become available on a very wide range of platforms, from embedded microcontrollers to supercomputers.";
    printf("%s\n",text);
    for(size_t i = 0; i < mat_size; i++)
    char *text = "1 Au commencement, Dieu créa le ciel et la terre. \n2 La terre n’était que chaos et vide. Il y avait des ténèbres à la surface de l'abîme et l'Esprit de Dieu planait au-dessus de l’eau. \n3 Dieu dit : « Qu’il y ait de la lumière ! » et il y eut de la lumière. \n4 Dieu vit que la lumière était bonne, et il sépara la lumière des ténèbres. \n5 Dieu appela la lumière jour, et les ténèbres nuit. Il y eut un soir et il y eut un matin. Ce fut le premier jour. \n6 Dieu dit : « Qu'il y ait une étendue entre les eaux pour les séparer les unes des autres ! » \n7 Dieu fit l'étendue et sépara ainsi l’eau qui est au-dessous de l'étendue de celle qui est audessus. Cela se passa ainsi. \n8 Dieu appela l'étendue ciel. Il y eut un soir et il y eut un matin. Ce fut le deuxième jour. \n9 Dieu dit : « Que les eaux qui sont au-dessous du ciel se rassemblent à un seul endroit et que le sec apparaisse ! » Et cela se passa ainsi. \n10 Dieu appela le sec terre, et la masse des eaux mers. Dieu vit que c'était bon. \n11 Puis Dieu dit : « Que la terre produise de la verdure, de l'herbe à graine, des arbres fruitiers qui donnent du fruit selon leur espèce et qui contiennent leur semence sur la terre ! » Et cela se passa ainsi : \n12 la terre produisit de la verdure, de l'herbe à graine selon son espèce et des arbres qui donnent du fruit et contiennent leur semence selon leur espèce. Dieu vit que c'était bon. \n13 Il y eut un soir et il y eut un matin. Ce fut le troisième jour. \n14 Dieu dit : « Qu'il y ait des luminaires dans l'étendue du ciel pour séparer le jour de la nuit ! Ils serviront de signes pour marquer les époques, les jours et les années, \n15 ainsi que de luminaires dans l'étendue du ciel pour éclairer la terre. » Et cela se passa ainsi : \n16 Dieu fit les deux grands luminaires, le plus grand pour présider au jour et le plus petit pour présider à la nuit. Il fit aussi les étoiles. \n17 Dieu les plaça dans l'étendue du ciel pour éclairer la terre, \n18 pour dominer sur le jour et la nuit et pour séparer la lumière des ténèbres. Dieu vit que c'était bon. \n19 Il y eut un soir et il y eut un matin. Ce fut le quatrième jour. \n20 Dieu dit : « Que l’eau pullule d'animaux vivants et que des oiseaux volent dans le ciel audessus de la terre ! » \n";
    {
        if((size_t)(doubleMat[i]) == 0 || (size_t)(doubleMat[i]) == 1)
            continue;
        else
        {
            Net_learn(doubleMat[i], network, text[i]);
            printf("\n< %c >--------\n", text[i]);
            printDoubleMat(doubleMat[i]);
//            printf("%c", Net_read(doubleMat[i], network, out_size));
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
