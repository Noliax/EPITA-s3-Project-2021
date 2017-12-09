#include "./block.h"
#include <gtk/gtk.h>
#include <stdlib.h>

static guchar *getpixel(GdkPixbuf *buffer, int i, int j) {
  int row_offset = gdk_pixbuf_get_rowstride(buffer);
  int channels = gdk_pixbuf_get_n_channels(buffer);
  guchar *pixels = gdk_pixbuf_get_pixels(buffer);
  return pixels + j * row_offset + i * channels;
}

static void setpixelrgb(guchar *pixel, guchar r, guchar g, guchar b) {
  pixel[0] = r;
  pixel[1] = g;
  pixel[2] = b;
}

static void setpixel(guchar *pixel, guchar val) {
  pixel[0] = val;
  pixel[1] = val;
  pixel[2] = val;
}

void print_mat(int *mat) {
  if (mat == 0) {
    printf("End of line\n");
  } else if (mat == 1) {
    printf("Space\n");
  } else {
    for (int j = 0; j < 32; j++) {
      for (int i = 0; i < 31; i++) {
        printf("%02i ", mat[i + 32 * j]);
      }
      printf("%02i\n", mat[31 + 32 * j]);
    }
  }
}

struct Block *Block_new(int x, int y, int width, int height) {
  struct Block *b = malloc(sizeof(struct Block));
  b->min_x = x;
  b->min_y = y;
  b->width = width;
  b->height = height;
  return b;
}

// returns false if something goes wrong (realloc fails)
static int double_size(struct BlockList *list)
{
  struct Block *tmp = realloc(list->data, 2 * list->capacity * sizeof (struct Block*));
  list->data = tmp;
  list->capacity *= 2;
  return 1;
}


struct BlockList *BlockList_new(size_t capacity)
{
  struct BlockList *list = malloc(sizeof(struct BlockList));
  list->size = 0;
  list->capacity = capacity;
  list->data = malloc(capacity * sizeof(struct Block*));
  return list;
}

int BlockList_push(struct BlockList *list, struct Block *elm)
{
  if(list->size < list->capacity ||
    (list->size == list->capacity && double_size(list)))
  {
    struct Block **end = list->data + list->size;
    *end = elm;
    list->size++;
    return 1;
  }
  return 0;
}

void BlockList_destroy(struct BlockList *list) {
  for(size_t i = 0; i < list->size; i++)
  {
    struct Block *block = list->data[i];
    if(block != 0 && block != 1)
      free(block);
  }
  free(list->data);
  free(list);
}

int *ProcessBlock(GdkPixbuf *source, struct Block *block) {
  if (block == (struct Block *)0)
    return (int *)0;
  if (block == (struct Block *)1)
    return (int *)1;

  GdkPixbuf *blockBuf = gdk_pixbuf_new_subpixbuf(
      source, block->min_x, block->min_y, block->width, block->height);

  // Resize block
  int new_width = 32;
  int new_height = 32;
  if (block->width != block->height) {
    // new_width / new_height = old_width / old_height
    if (block->height > block->width)
      new_width = new_height * block->width / block->height;
    else
      new_height = new_width * block->height / block->width;
  }
  GdkPixbuf *resized = gdk_pixbuf_scale_simple(blockBuf, new_width, new_height,
                                               GDK_INTERP_BILINEAR);

  const guchar *pixels = gdk_pixbuf_read_pixels(resized);
  int n_channels = gdk_pixbuf_get_n_channels(resized);
  int rowstride = gdk_pixbuf_get_rowstride(resized);
  int *mat = calloc(32 * 32, sizeof(int));

  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      mat[i + 32 * j] = -1;

      if (i < new_width && j < new_height) {
        guchar p = *(pixels + j * rowstride + i * n_channels);
        if (p == 0)
          mat[i + 32 * j] = 1;
      }
    }
  }

  return mat;
}

int **BlocksToMat(GdkPixbuf *source, struct BlockList *blocks) {
  int **block_matrices = malloc(blocks->size* sizeof(int *));

  size_t i = 0;
  for(size_t i = 0; i < blocks->size; i++)
  {
    block_matrices[i] = ProcessBlock(source, blocks->data[i]);
  }

  return block_matrices;
}

void DisplayBlockRGB(GdkPixbuf *buffer, struct Block *block, int r, int g, int b) {
  if(block == 0 || block == 1)
    return;

  int end_x = block->min_x + block->width;
  int end_y = block->min_y + block->height;
  guchar *p1, *p2;
  for (int dx = block->min_x; dx < end_x; dx++) {
    p1 = getpixel(buffer, dx, block->min_y);
    p2 = getpixel(buffer, dx, end_y);
    setpixelrgb(p1, r, g, b);
    setpixelrgb(p2, r, g, b);
  }
  for (int dy = block->min_y; dy < end_y; dy++) {
    p1 = getpixel(buffer, block->min_x, dy);
    p2 = getpixel(buffer, end_x, dy);
    setpixelrgb(p1, r, g, b);
    setpixelrgb(p2, r, g, b);
  }
}

void DisplayBlock(GdkPixbuf *buffer, struct Block *block) {
  DisplayBlockRGB(buffer, block, 255, 0, 0);
}

void CropWhite(GdkPixbuf *source, struct Block *block) {
  guchar *p1;

  // Crop the white on the top
  for (int dy = 0; dy < block->height; dy++) {
    int is_white = 1;
    for (int dx = 0; dx < block->width && is_white; dx++) {
      p1 = getpixel(source, block->min_x + dx, block->min_y + dy);
      if (*p1 == 0)
        is_white = 0;
    }
    if (!is_white) {
      block->min_y += dy - 1;
      block->height -= dy - 1;
      break;
    }
  }

  // Crop the white at the bottom
  for (int dy = block->height - 1; dy >= 0; dy--) {
    int is_white = 1;
    for (int dx = 0; dx < block->width && is_white; dx++) {
      p1 = getpixel(source, block->min_x + dx, block->min_y + dy);
      if (*p1 == 0)
        is_white = 0;
    }
    if (!is_white) {
      block->height -= block->height - 1 - dy;
      break;
    }
  }
}
