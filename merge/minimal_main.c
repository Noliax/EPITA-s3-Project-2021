#include "./block.h"
#include "./segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  gtk_init(&argc, &argv);

  //---------------------------------------------------------------------------
  // SEGMENTATION

  char *path = "texte.png";
  GtkWidget *img = gtk_image_new_from_file(path);

  // value for words
  int hsv = 8;
  int vsv = 30;
  int ahsv = 2;

  GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));


  // Blocks detection to int mat
  struct BlockList *blocks = RLSA(buffer, hsv, vsv, ahsv);
  int **mat = BlocksToMat(buffer, blocks);

  // Prints for debug
  for (size_t i = 0; i < 3; i++) {
    print_mat(mat[i]);
  }
  printf("%zu blocks !\n", blocks->size);

  // Clean
  for(size_t i = 0; i < 32; i++)
  {
    int *m = mat[i];
    if(m != 0 && m != 1)
      free(m);
  }
  free(mat);
  BlockList_destroy(blocks);
  return 0;
}
