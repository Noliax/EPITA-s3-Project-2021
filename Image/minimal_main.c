#include <stdlib.h>
#include <gtk/gtk.h>
#include "./segmentation.h"
#include "./block.h"

static
void print_mat(int* mat)
{
  for(int j = 0; j < 32; j++)
  {
    for(int i = 0; i < 31; i++)
    {
      printf("%i ", mat[i + 32*j]); 
    }
    printf("%i\n", mat[31 + 32*j]);
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  gtk_init(&argc, &argv);

  //---------------------------------------------------------------------------
  //SEGMENTATION

  char *path = "texte.png";
  GtkWidget *img = gtk_image_new_from_file(path);

  // value for words
  int hsv = 8;
  int vsv = 30;
  int ahsv = 2;

  // Double horizontal to get lines
  // hsv *= 2;
  // ahsv *= 2;

  GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));

  // RLSA algorithm to detect blocks
  BinarizeColors(buffer);
  GdkPixbuf *horizontal = HorizontalSmoothing(buffer, hsv);
  GdkPixbuf *vertical = VerticalSmoothing(buffer, vsv);
  GdkPixbuf *both = MergeSmoothings(horizontal, vertical);
  GdkPixbuf *final = HorizontalSmoothing(both, ahsv);
  
  // Blocks detection to int mat
  struct BlockList *blocks = malloc(sizeof(struct BlockList));
  BlockList_init(blocks);
  GdkPixbuf *withBlocks = Scan_Surface(final, buffer, blocks);
  size_t count = blocks->size;
  int **mat = BlocksToMat(final, blocks);

  // Prints for debug
  for(size_t i = 0; i < count; i++)
  {
    print_mat(mat[i]);
  }
  printf("%zu blocks !\n", count);

  // Clean
  // TODO: free int **mat
  BlockList_destroy(blocks);
  g_object_unref(horizontal);
  g_object_unref(vertical);
  g_object_unref(both);
  g_object_unref(final);
  g_object_unref(withBlocks);
  
  return 0;
}
