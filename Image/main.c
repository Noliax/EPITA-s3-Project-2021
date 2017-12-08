#include "./block.h"
#include "./segmentation.h"
#include <gtk/gtk.h>
#include <stdlib.h>

static void activate(GtkApplication *app,
                     __attribute__((unused)) gpointer user_data) {
  //---------------------------------------------------------------------------
  // Create a window with an image

  GtkWidget *window;
  GtkWidget *scroll;
  GtkWidget *img;
  char *path = "texte.png";

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "OCR Segmentation");
  gtk_window_set_default_size(GTK_WINDOW(window), 1400, 700);

  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(window), scroll);

  img = gtk_image_new_from_file(path);
  gtk_container_add(GTK_CONTAINER(scroll), img);

  gtk_widget_show_all(window);

  //---------------------------------------------------------------------------
  // SEGMENTATION

  // value for words
  int hsv = 8;
  int vsv = 30;
  int ahsv = 2;

  GdkPixbuf *buffer = gtk_image_get_pixbuf(GTK_IMAGE(img));
  struct BlockList *blocks = RLSA(buffer, hsv, vsv, ahsv);
  size_t blocks_len = blocks->size;

  // Display for the show
  struct BlockList *cursor = blocks;
  if (!BlockList_isempty(cursor)) {
    int i = 4;
    while (i > 0) {
      while (cursor->next != NULL &&
             (cursor->next->curr == 0 || cursor->next->curr == 1))
        cursor = cursor->next;
      if (cursor->next != NULL)
        DisplayBlock(buffer, cursor->next->curr);
      i--;
      cursor = cursor->next;
    }
  }

  int** matrices = BlocksToMat(buffer, blocks);

  for(size_t i = 0; i < blocks_len/2; i++)
  {
    int *tmp = matrices[i];
    matrices[i] = matrices[blocks_len-i-1];
    matrices[blocks_len-i-1] = tmp;
  }
  
  for(size_t i = 0; i < 5 && i < blocks_len; i++)
    print_mat(matrices[i]);

  BlockList_destroy(blocks);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("test.ocr.image", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
