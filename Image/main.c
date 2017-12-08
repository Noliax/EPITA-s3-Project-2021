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
}

static
void activate (GtkApplication* app, __attribute__ ((unused)) gpointer user_data)
{
  //---------------------------------------------------------------------------
  //Create a window with an image

  GtkWidget *window;
  GtkWidget *scroll;
  GtkWidget *img;
  char *path = "texte.png";

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "OCR Segmentation");
  gtk_window_set_default_size (GTK_WINDOW (window), 1400, 700);

  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(window), scroll);

  img = gtk_image_new_from_file(path);
  gtk_container_add(GTK_CONTAINER(scroll), img);
  
  gtk_widget_show_all(window);

  //---------------------------------------------------------------------------
  //SEGMENTATION

  // value for words
  int hsv = 8;
  int vsv = 30;
  int ahsv = 2;

  // Double horizontal to get lines
  // hsv *= 2;
  // ahsv *= 2;

  GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));

  // RLSA
  BinarizeColors(buffer);
  GdkPixbuf *horizontal = HorizontalSmoothing(buffer, hsv);
  GdkPixbuf *vertical = VerticalSmoothing(buffer, vsv);
  GdkPixbuf *both = MergeSmoothings(horizontal, vertical);
  GdkPixbuf *final = HorizontalSmoothing(both, ahsv);
  
  // Blocks detection
  struct BlockList *blocks = malloc(sizeof(struct BlockList));
  BlockList_init(blocks);
  GdkPixbuf *withBlocks = Scan_Surface(final, buffer, blocks);
  size_t count = blocks->size;
  int **mat = BlocksToMat(final, blocks);

  for(size_t i = 0; i < count; i++)
  {
    print_mat(mat[i]);
  }
  printf("%zu blocks !\n", count);

  BlockList_destroy(blocks);

  g_object_unref(horizontal);
  g_object_unref(vertical);
  g_object_unref(both);
  g_object_unref(final);

  // Display result
  gtk_image_clear(GTK_IMAGE(img));
  gtk_image_set_from_pixbuf(GTK_IMAGE(img), withBlocks);

  g_object_unref(withBlocks);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("test.ocr.image", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
