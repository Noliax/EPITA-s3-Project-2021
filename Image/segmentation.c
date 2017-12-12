#include "./block.h"
#include <gtk/gtk.h>
#include <stdlib.h>

static guchar *getpixel(GdkPixbuf *buffer, int i, int j) {
  int row_offset = gdk_pixbuf_get_rowstride(buffer);
  int channels = gdk_pixbuf_get_n_channels(buffer);
  guchar *pixels = gdk_pixbuf_get_pixels(buffer);
  return pixels + j * row_offset + i * channels;
}

static void setpixel(guchar *pixel, guchar val) {
  pixel[0] = val;
  pixel[1] = val;
  pixel[2] = val;
}

static double GreyscaleAverage(GdkPixbuf *buffer) {
  double average = 0;

  guchar *pixels = gdk_pixbuf_get_pixels(buffer);
  gsize len = gdk_pixbuf_get_byte_length(buffer);
  guchar *end = pixels + len;

  int n_channels = gdk_pixbuf_get_n_channels(buffer);

  for (; pixels < end; pixels += n_channels) {
    double greyscale = 0.3 * pixels[0] + 0.59 * pixels[1] + 0.11 * pixels[2];
    setpixel(pixels, greyscale);
    average += greyscale;
  }
  average = average / len;

  return average;
}

static void BinarizeColors(GdkPixbuf *buffer) {
  int width = gdk_pixbuf_get_width(buffer);
  int height = gdk_pixbuf_get_height(buffer);

  // double -> int needs +0.5 because casting truncates decimal
  // e.g (int)(3.99999) gives 3
  double average = GreyscaleAverage(buffer);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      guchar *p = getpixel(buffer, i, j);
      guchar val = p[0] < 2.5 * average ? 0 : 255;
      setpixel(p, val);
    }
  }
}

static GdkPixbuf *MergeSmoothings(GdkPixbuf *a, GdkPixbuf *b) {
  int width = gdk_pixbuf_get_width(a);
  int height = gdk_pixbuf_get_height(a);
  GdkPixbuf *res = gdk_pixbuf_copy(a);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      guchar *pa = getpixel(a, i, j);
      guchar *pb = getpixel(b, i, j);
      guchar *res_pixel = getpixel(res, i, j);

      guchar value = *pa == 0 && *pb == 0 ? 0 : 255;
      setpixel(res_pixel, value);
    }
  }

  return res;
}

static GdkPixbuf *HorizontalSmoothing(GdkPixbuf *a, int treshold) {
  int width = gdk_pixbuf_get_width(a);
  int height = gdk_pixbuf_get_height(a);
  int n_channels = gdk_pixbuf_get_n_channels(a);

  GdkPixbuf *res = gdk_pixbuf_copy(a);

  int last_black;
  for (int j = 0; j < height; j++) {
    last_black = width;
    for (int i = 0; i < width; i++) {
      guchar *pixel = getpixel(a, i, j);

      if (pixel[0] == 0) {
        int gap = i - last_black + 1;
        if (2 < gap && gap <= treshold) {
          int toFill = last_black + 1;
          guchar *fill_pixel = getpixel(res, toFill, j);
          for (; toFill <= i; toFill++) {
            setpixel(fill_pixel, 0);
            fill_pixel += n_channels;
          }
        }
        last_black = i;
      }
    }
  }
  return res;
}

static GdkPixbuf *VerticalSmoothing(GdkPixbuf *a, int treshold) {
  int width = gdk_pixbuf_get_width(a);
  int height = gdk_pixbuf_get_height(a);
  int n_channels = gdk_pixbuf_get_n_channels(a);

  GdkPixbuf *res = gdk_pixbuf_copy(a);

  int last_black;
  for (int i = 0; i < width; i++) {
    last_black = height;
    for (int j = 0; j < height; j++) {
      guchar *pixel = getpixel(a, i, j);

      if (pixel[0] == 0) {
        int gap = j - last_black + 1;
        if (2 < gap && gap <= treshold) {
          int toFill = last_black + 1;
          guchar *fill_pixel = getpixel(res, i, toFill);
          for (; toFill <= j; toFill++) {
            setpixel(fill_pixel, 0);
            fill_pixel += width * n_channels;
          }
        }
        last_black = j;
      }
    }
  }
  return res;
}

static void SearchHorizontalBlocks(GdkPixbuf *source, struct BlockList *list,
                                   int col1, int col2, int line1, int line2) {
  int height = line2 - line1 + 1;
  int last_white_col = -1;

  for (int i = col1; i <= col2; i++) {
    // Search white column
    int nb_black = 0;
    for (int y = line1 + 1; y < line2; y++) {
      guchar *pixel = getpixel(source, i, y);
      if (*pixel == 0)
        nb_black++;
    }

    if (nb_black == 0) {
      // we found two differents white columns
      if (last_white_col > 0 && i - last_white_col > 1) {
        struct Block *block =
            Block_new(last_white_col, line1, i - last_white_col + 1, height);

        CropWhite(source, block);
        BlockList_push(list, block);
      }
      last_white_col = i;
    }
  }
}

static struct BlockList *SearchLines(GdkPixbuf *source) {
  struct BlockList *list = BlockList_new(100);

  int width = gdk_pixbuf_get_width(source);
  int height = gdk_pixbuf_get_height(source);

  int last_white_line = -1;
  for (int j = 0; j < height; j++) {
    // Count the nb of black pixels per line
    int blacks_found = 0;
    for (int i = 0; i < width && blacks_found == 0; i++) {
      guchar *pixel = getpixel(source, i, j);
      if (*pixel == 0)
        blacks_found++;
    }

    if (blacks_found == 0) {
      // We found two different blank lines
      // There is probably one or multiple block between these lines.
      if (last_white_line > 0 && j - last_white_line > 1) {
        SearchHorizontalBlocks(source, list, 0, width, last_white_line, j);
      }
      last_white_line = j;
    }
  }
  return list;
}

struct BlockList *RLSA(GdkPixbuf *buffer, int hsv, int vsv, int ahsv) {

  // Should be easier with only 2 colors
  BinarizeColors(buffer);

  // LINES
  GdkPixbuf *horizontal = HorizontalSmoothing(buffer, hsv * 2);
  GdkPixbuf *vertical = VerticalSmoothing(buffer, vsv);
  GdkPixbuf *both = MergeSmoothings(horizontal, vertical);
  GdkPixbuf *final = HorizontalSmoothing(both, ahsv * 10);
  final = VerticalSmoothing(final, ahsv * 5);

  struct BlockList *lines = SearchLines(final);

  // WORDS
  horizontal = HorizontalSmoothing(buffer, hsv * 2);
  vertical = VerticalSmoothing(buffer, vsv);
  both = MergeSmoothings(horizontal, vertical);
  final = HorizontalSmoothing(both, ahsv * 4);

  // Push all words then push 0 to mark the end of line
  struct BlockList *words = BlockList_new(100);
  for(size_t i = 0; i < lines->size; i++)
  {
    struct Block *line = lines->data[i];
    int line1 = line->min_y;
    int line2 = line1 + line->height;
    int col1 = line->min_x;
    int col2 = col1 + line->width;

    SearchHorizontalBlocks(final, words, col1, col2, line1, line2);
    BlockList_push(words, (struct Block *)0);
  }

  // CHARACTERS
  horizontal = HorizontalSmoothing(buffer, hsv * 2);
  vertical = VerticalSmoothing(buffer, vsv);
  both = MergeSmoothings(horizontal, vertical);
  final = HorizontalSmoothing(both, ahsv);

  // Push all chars then push 1 to mark the end of a word
  struct BlockList *chars = BlockList_new(100);
  for(size_t i = 0; i < words->size; i++)
  {
    struct Block *word = words->data[i];

    // word can be an end of line !
    if (word != 0) {
      int line1 = word->min_y;
      int line2 = line1 + word->height;
      int col1 = word->min_x;
      int col2 = col1 + word->width;

      SearchHorizontalBlocks(final, chars, col1, col2, line1, line2);
      BlockList_push(chars, (struct Block *)1);
    }
    else {
      BlockList_push(chars, (struct Block*)0);
    }
  }

  for(size_t i = 0; i < words->size; i++)
  {
    DisplayBlock(buffer, words->data[i]);
  }

  // CLEAN THIS MESS
  BlockList_destroy(words);
  BlockList_destroy(lines);

  g_object_unref(horizontal);
  g_object_unref(vertical);
  g_object_unref(both);
  return chars;
}
