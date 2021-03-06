#include "../merge/block.h"
#include "../merge/net.h"
#include "../merge/segmentation.h"
#include <SDL.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

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

typedef struct {
  GtkBuilder *builder;
  gpointer user_data;
} SGlobalData;

static SGlobalData data;
static GtkWidget *fenetre_principale = NULL;
static GtkWidget *fenetre_secondaire = NULL;
static gchar *resultat = "Final Text";
static GError *error = NULL;
static gchar *filename = NULL;
static gchar *nomFichier = "resultat.txt";
static GtkTextBuffer *buffer;
static GtkTextIter iter;
static GtkTextBuffer *bufferpopup;
static GtkImage *image;
static GdkPixbuf *buf;
static char *result;

G_MODULE_EXPORT void on_MainWindow_destroy() { gtk_main_quit(); }

G_MODULE_EXPORT void on_BoutonQuit_clicked() { gtk_main_quit(); }

G_MODULE_EXPORT void Charger_selection_changed_cb() {}

G_MODULE_EXPORT void on_Charger_selection_changed() {
  resultat = "Final Text";
  gtk_text_buffer_set_text(buffer, resultat, -1);
  GtkFileChooser *file =
      GTK_FILE_CHOOSER(gtk_builder_get_object(data.builder, "Charger"));
  gchar *filename = gtk_file_chooser_get_filename(file);
  image = GTK_IMAGE(gtk_builder_get_object(data.builder, "Image2"));
  gtk_image_set_from_file(image, filename);

  buf = gtk_image_get_pixbuf(image);
  struct BlockList *blocks = RLSA(buf, 8, 30, 2);

  for (size_t i = 0; i < blocks->size; i++) {
    DisplayBlock(buf, blocks->data[i]); }

  // OCR
  int **mat = BlocksToMat(buf, blocks);
  size_t mat_size = blocks->size;
  double **doubleMat = IntToDoubleMat(mat, mat_size);

  size_t out_size = 26 + 26 + 10 + 7;
  double **network = malloc(out_size * sizeof(double*));
    for(size_t i = 0; i < out_size; i++)
    {
        network[i] = Net_newMat(1024);
    }
  Net_open(network, out_size, "../netmap");
   
  result = malloc(mat_size * sizeof(char) + sizeof(char));
  result[mat_size] = '\0';

  for (size_t i = 0; i < mat_size; ++i) {
    if (doubleMat[i] == (void *)1)
      result[i] = ' ';
    else if (doubleMat[i] == (void *)0)
      result[i] = '\n';
    else {
      result[i] = Net_read(doubleMat[i], network, out_size);
    }
  }

  gtk_text_buffer_set_text(buffer, result, -1);

  BlockList_destroy(blocks);
}

G_MODULE_EXPORT void on_BoutonSave_clicked() {
  gtk_widget_show(fenetre_secondaire);
}

G_MODULE_EXPORT void on_BoutonValide_clicked() {
  GtkEntry *entre =
      GTK_ENTRY(gtk_builder_get_object(data.builder, "EntreDeNom"));
  nomFichier = gtk_entry_get_text(entre);
  // gtk_text_buffer_insert(buffer, &iter, resultat, -1);
  if (nomFichier[0] == '\0') {
    gtk_text_buffer_set_text(bufferpopup, "Enter a correct name please", -1);
  } else {
    FILE *fichier = NULL;
    fichier = fopen(nomFichier, "w");
    fputs(result, fichier);
    fclose(fichier);
    gtk_widget_hide(fenetre_secondaire);
    gtk_widget_show(fenetre_principale);
  }
}

G_MODULE_EXPORT void on_BoutonTranslate_clicked() {
  // prendre la sortie de la fonction de thib
  resultat = "Text from the image\n";
  gtk_text_buffer_set_text(buffer, resultat, -1);
}

G_MODULE_EXPORT void on_BoutonTourner_clicked() {
  buf = gtk_image_get_pixbuf(image);
  buf = gdk_pixbuf_rotate_simple(buf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
  gtk_image_set_from_pixbuf(image, buf);
}

G_MODULE_EXPORT void on_BoutonTourner2_clicked() {
  buf = gtk_image_get_pixbuf(image);
  buf = gdk_pixbuf_rotate_simple(buf, GDK_PIXBUF_ROTATE_CLOCKWISE);
  gtk_image_set_from_pixbuf(image, buf);
}

G_MODULE_EXPORT void on_BoutonContour_clicked() {}

/*static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}


G_MODULE_EXPORT void on_BoutonBlackWhite_clicked()
{
  buf = gtk_image_get_pixbuf(image);
  printf("rrrrrrrrrrrr\n");
  SDL_Surface *img = image;
  printf("gggggggggg\n");
  Uint8 r = 0;
  Uint8 g = 0;
  Uint8 b = 0;
  Uint8 somme = 0;
  Uint32 pixel;
  int a = gdk_pixbuf_get_width (buf);
  int c = gdk_pixbuf_get_height (buf);
  printf("llllllllllllll\n");
  for(int i = 0; i < a; i++)
  {
    for(int j = 0; j < c; j++)
    {
      printf("aaaaaaaaaaaa\n");
      pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img->format, &r, &g, &b);
      somme = r * 0.3 + g * 0.59 + b * 0.11;
      r = somme;
      g = somme;
      b = somme;
      pixel = SDL_MapRGB(img->format, r, g, b);
      putpixel(img, i, j, pixel);
    }
  }
  printf("hhhhhhhhhh\n");
  image = img;
  printf("ddddd\n");
  gtk_image_set_from_pixbuf (image, buf);
  printf("tttttttttttt\n");
} */

// void callback_about (GtkMenuItem *menuitem, gpointer user_data);

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  data.builder = gtk_builder_new();

  filename = g_build_filename("test.glade", NULL);

  gtk_builder_add_from_file(data.builder, filename, &error);
  g_free(filename);
  if (error) {
    gint code = error->code;
    g_printerr("%s\n", error->message);
    g_error_free(error);
    return code;
  }

  gtk_builder_connect_signals(data.builder, &data);

  fenetre_principale =
      GTK_WIDGET(gtk_builder_get_object(data.builder, "MainWindow"));

  gtk_widget_show_all(fenetre_principale);

  fenetre_secondaire =
      GTK_WIDGET(gtk_builder_get_object(data.builder, "SecondWindow"));

  buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(data.builder, "Buffer"));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_insert(buffer, &iter, resultat, -1);

  bufferpopup =
      GTK_TEXT_BUFFER(gtk_builder_get_object(data.builder, "TextPopUp"));
  gtk_text_buffer_get_iter_at_offset(bufferpopup, &iter, 0);
  gtk_text_buffer_insert(bufferpopup, &iter, resultat, -1);
  gtk_text_buffer_set_text(bufferpopup, "Enter a name file", -1);

  gtk_main();
  exit(0);
  return 0;
}

void callback_about(GtkMenuItem *menuitem, gpointer user_data) {
  /* Transtypage du pointeur user_data pour récupérer nos données. */
  SGlobalData *data = (SGlobalData *)user_data;
  GtkWidget *dialog = NULL;

  /* Récupération de la fenêtre "AboutWindow". */
  dialog = GTK_WIDGET(gtk_builder_get_object(data->builder, "AboutWindow"));
  gtk_dialog_run(GTK_DIALOG(dialog));

  /* On cache la fenêtre de dialogue. Si on la détruisait le prochain appel */
  /* à ce callback provoquerait un segdefault! */
  gtk_widget_hide(dialog);
}
