#include "net.h"
#include "../Image/block.h"
#include "../Image/segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printDoubleMat(double *mat)
{
    for(size_t i = 0; i < 1024; i++)
        printf("%.5f%c", mat[i], i%32==0 ? '\n' : ' ');
    printf("\n");
}

double **IntToDoubleMat(int **mat, size_t mat_size)
{
  // Convert int **mat to double **doubleMat
  double **doubleMat = malloc(mat_size * sizeof(double*));
  double *charmap;
  for(size_t i = 0; i < mat_size; i++)
  {
      if((size_t)(mat[i]) == 0 || (size_t)(mat[i]) == 1)
          doubleMat[i] = (double*)mat[i];
      else
      {
          charmap = Net_newMat(1024);
          for(size_t j = 0; j < 1024; j++)
              charmap[j] = (double)(mat[i][j]);
          doubleMat[i] = charmap;
      }
  }
  return doubleMat;
}

void learn(double **network, GdkPixbuf *buffer, char *text)
{
  struct BlockList *blocks = RLSA(buffer, 8, 32, 2);
  int **mat = BlocksToMat(buffer, blocks);
  size_t mat_size = blocks->size;
  double **doubleMat = IntToDoubleMat(mat, mat_size);
  size_t text_len = strlen(text);

  for(size_t i = 0; i < mat_size; i++)
  {
    if((size_t)(doubleMat[i]) == 0 || (size_t)(doubleMat[i]) == 1)
      continue;
    else
    {
      Net_learn(doubleMat[i], network, text[i%text_len]);
      printf("%c %zu \n", text[i%text_len], i);
      print_mat(mat[i]);
    }
  }

}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    // Create neural networkd
    size_t out_size = 26 + 26 + 10 + 7;
    double **network = malloc(out_size * sizeof(double*));
    for(size_t i = 0; i < out_size; i++)
    {
        network[i] = Net_newMat(1024);
    }
    
    // Load image
    char *path = "texte1.png";
    GtkWidget *img = gtk_image_new_from_file(path);
    GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));

    char *path2 = "texte2.png";
    GtkWidget *img2 = gtk_image_new_from_file(path2);
    GdkPixbuf *buffer2 = gtk_image_get_pixbuf(GTK_IMAGE(img2));


    char *alph_path = "texte4.png";
    GtkWidget *alph_img = gtk_image_new_from_file(alph_path);
    GdkPixbuf *alph_buffer = gtk_image_get_pixbuf(GTK_IMAGE(alph_img));


    char *text = 
      "1 Au commencement, Dieu crea le ciel et la terre. \n"
      "2 La terre n' etait que chaos et vide. Il y avait des tenebres a la surface de l' abime et l'Esprit \n"
      "de Dieu planait au-dessus de l' eau. \n"
      "3 Dieu dit : \" Qu'il y ait de la lumiere ! \" et il y eut de la lumiere. \n"
      "4 Dieu vit que la lumiere etait bonne, et il separa la lumiere des tenebres. \n"
      "5 Dieu appela la lumiere jour , et les tenebres nuit. Il y eut un soir et il y eut un matin. Ce fut \n"
      "le premier jour . \n"
      "6 Dieu dit : \" Qu'il y ait une etendue entre les eaux pour les separer les unes des autres ! \" \n"
      "7 Dieu fit l' etendue et separa ainsi l' eau qui est au-dessous de l' etendue de celle qui est \n"
      "audessus. Cela se passa ainsi. \n"
      "8 Dieu appela l' etendue ciel. Il y eut un soir et il y eut un matin. Ce fut le deuxieme jour . \n"
      "9 Dieu dit : \" Que les eaux qui sont au-dessous du ciel se rassemblent a un seul endroit et \n"
      "que le sec apparaisse ! \" Et cela se passa ainsi. \n"
      "10 Dieu appela le sec terre, et la masse des eaux mers. Dieu vit que c' etait bon. \n"
      "11 Puis Dieu dit : \" Que la terre produise de la verdure, de l'herbe a graine, des arbres \n"
      "fruitiers qui donnent du fruit selon leur espece et qui contiennent leur semence sur la terre ! \n"
      "\" Et cela se passa ainsi : \n"
      "12 la terre produisit de la verdure, de l'herbe a graine selon son espece et des arbres qui \n"
      "donnent du fruit et contiennent leur semence selon leur espece. Dieu vit que c' etait bon. \n"
      "13 Il y eut un soir et il y eut un matin. Ce fut le troisieme jour . \n"
      "14 Dieu dit : \" Qu'il y ait des luminaires dans l' etendue du ciel pour separer le jour de la nuit \n"
      "! Ils serviront de signes pour marquer les epoques, les jours et les annees, \n"
      "15 ainsi que de luminaires dans l' etendue du ciel pour eclairer la terre. \" Et cela se passa \n"
      "ainsi : \n"
      "16 Dieu fit les deux grands luminaires, le plus grand pour presider au jour et le plus petit \n"
      "pour presider a la nuit. Il fit aussi les etoiles. \n"
      "17 Dieu les placa dans l' etendue du ciel pour eclairer la terre, \n"
      "18 pour dominer sur le jour et la nuit et pour separer la lumiere des tenebres. Dieu vit que \n"
      "c' etait bon. \n"
      "19 Il y eut un soir et il y eut un matin. Ce fut le quatrieme jour . \n"
      "20 Dieu dit : \" Que l' eau pullule d' animaux vivants et que des oiseaux volent dans le ciel \n"
      "audessus de la terre ! \" \n";

    char *text2 =
      "21 Dieu crea les grands poissons et tous les animaux vivants capables de se deplacer : l' eau \n"
      "en pullula selon leur espece. Il crea aussi tous les oiseaux selon leur espece. Dieu vit que \n"
      "c' etait bon, \n"
      "22 et il les benit en disant : \" Reproduisez-vous, devenez nombreux et remplissez les mers, \n"
      "et que les oiseaux se multiplient sur la terre ! \" \n"
      "23 Il y eut un soir et il y eut un matin. Ce fut le cinquieme jour . \n"
      "24 Dieu dit : \" Que la terre produise des animaux vivants selon leur espece : du betail, des \n"
      "reptiles et des animaux terrestres selon leur espece. \" Et cela se passa ainsi. \n"
      "25 Dieu fit les animaux terrestres selon leur espece, le betail selon son espece et tous les \n"
      "reptiles de la terre selon leur espece. Dieu vit que c' etait bon. \n"
      "26 Puis Dieu dit : \" Faisons l'homme a notre image, a notre ressemblance ! Qu'il domine sur \n"
      "les poissons de la mer , sur les oiseaux du ciel, sur le betail, sur toute la terre et sur tous les \n"
      "reptiles qui rampent sur la terre. \" \n"
      "27 Dieu crea l'homme a son image, il le crea a l'image de Dieu. Il crea l'homme et la femme. \n"
      "28 Dieu les benit et leur dit : \" Reproduisez-vous, devenez nombreux, remplissez la terre et \n"
      "soumettez-la ! Dominez sur les poissons de la mer , sur les oiseaux du ciel et sur tout animal \n"
      "qui se deplace sur la terre ! \" \n"
      "29 Dieu dit aussi : \" Je vous donne toute herbe a graine sur toute la surface de la terre, ainsi \n"
      "que tout arbre portant des fruits avec pepins ou noyau : ce sera votre nourriture. \n"
      "30 A tout animal de la terre, a tout oiseau du ciel et a tout ce qui se deplace sur la terre, a ce \n"
      "qui est anime de vie, je donne toute herbe verte pour nourriture. \" Et cela se passa ainsi. \n"
      "31 Dieu regarda tout ce qu'il avait fait, et il constata que c' etait tres bon. Il y eut un soir et il \n"
      "y eut un matin. Ce fut le sixieme jour . \n"
      "Genese 2 \n"
      "1 C' est ainsi que furent termines le ciel et la terre et toute leur armee. \n"
      "2 Le septieme jour , Dieu mit un terme a son travail de creation. Il se reposa de toute son \n"
      "activite le septieme jour . \n"
      "3 Dieu benit le septieme jour et en fit un jour saint, parce que ce jour -la il se reposa de toute \n"
      "son activite, de tout ce qu'il avait cree. \n"
      "4 T elle fut l'histoire du ciel et de la terre quand ils furent crees. \n"
      "5 Lorsque l'Eternel Dieu fit la terre et le ciel, il n'y avait encore aucun arbuste des champs \n"
      "sur la terre et aucune herbe des champs ne poussait encore, car l'Eternel Dieu n' avait pas \n"
      "fait pleuvoir sur la terre et il n'y avait pas d'homme pour cultiver le sol. \n"
      "6 Cependant, une vapeur montait de la terre et arrosait toute la surface du sol. \n";

    char *text3 =
      "0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P \n"
      "Q R S T U V W X Y Z ? ! , ; . : \n"
      "- \n";


    learn(network, buffer, text);
    learn(network, buffer2, text2);
    learn(network, alph_buffer, text3);

    // OCR
    struct BlockList *blocks = RLSA(buffer, 8, 32, 2);
    int **mat = BlocksToMat(buffer, blocks);
    size_t mat_size = blocks->size;
    double **doubleMat = IntToDoubleMat(mat, mat_size);

    char *result = malloc(mat_size * sizeof(char) + sizeof(char));
    result[mat_size] = '\0';

    for(size_t i = 0; i < mat_size; ++i)
    {
        if(doubleMat[i] == (void*)1)
            result[i] = ' ';
        else 
        if(doubleMat[i] == (void*)0)
            result[i] = '\n';
        else
        {
            result[i] = Net_read(doubleMat[i], network, out_size);
        }
    }
    printf("\n%s\n",result);
    
    exit(0);
    return 0;
}
