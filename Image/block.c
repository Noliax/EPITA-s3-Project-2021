#include <stdlib.h>
#include <gtk/gtk.h>
#include "./block.h"


struct Block *Block_new(int x, int y, int width, int height)
{
  struct Block *b = malloc(sizeof(struct Block));
  b->min_x = x;
  b->min_y = y;
  b->width = width;
  b->height = height;
  return b;
}

void BlockList_init(struct BlockList *list)
{
  list->next = NULL;
  list->curr = NULL; 
  list->size = 0;
}

int BlockList_isempty(struct BlockList *list)
{
  return list->next == NULL;
}

void BlockList_push(struct BlockList *list, struct Block *elm)
{
  struct BlockList *tmp = list->next;
  struct BlockList *new = malloc(sizeof(struct BlockList));
  new->curr = elm;
  new->next = tmp;
  list->next = new;
  list->size += 1;
}

struct Block *BlockList_pop(struct BlockList *list)
{
  if(BlockList_isempty(list))
    return NULL;

  struct BlockList *elm = list->next;
  list->next = elm->next;
  struct Block *ret = elm->curr;
  free(elm);
  list->size -= 1;
  return ret; 
}

void BlockList_destroy(struct BlockList *list)
{
  if(BlockList_isempty(list))
    return;

  while(list->next->next != NULL)
  {
    struct BlockList *tmp = list->next->next;
    free(BlockList_pop(list));
    list->next = tmp; 
  }
  free(BlockList_pop(list));
  free(list);
}

int* ProcessBlock(GdkPixbuf *source, struct Block *block)
{
  GdkPixbuf *blockBuf = gdk_pixbuf_new_subpixbuf(source, block->min_x, block->min_y, block->width, block->height);

  // Resize block
  int new_width = 32;
  int new_height = 32;
  if(block->width != block->height)
  {
    // new_width / new_height = old_width / old_height
    if(block->height > block->width)
      new_width = new_height * block->width / block->height;
    else
      new_height =  new_width * block->height / block->width;
  }
  printf("Resizing block to %ix%i\n", new_width, new_height);
  GdkPixbuf* resized = gdk_pixbuf_scale_simple(blockBuf, new_width, new_height, GDK_INTERP_BILINEAR);

  const guchar *pixels = gdk_pixbuf_read_pixels(resized);
  int n_channels = gdk_pixbuf_get_n_channels(resized);
  int rowstride = gdk_pixbuf_get_rowstride(resized);
  int *mat = calloc(32*32, sizeof(int));
   
  for(int i = 0; i < 32; i++)
  {
    for(int j = 0; j < 32; j++)
    {
      if(i < new_width && j < new_height)
      {
        guchar p = *(pixels + j*rowstride + i*n_channels);
        if(p == 0)
          mat[i + 32*j] = 1;
      }
      else
      {
        mat[i + 32*j] = 0;
      }
    }
  }
  
  return mat;
}

int **BlocksToMat(GdkPixbuf *final, struct BlockList *blocks)
{
  size_t count = blocks->size;
  int **block_matrices = malloc(count * sizeof(int*));
  int **tmp = block_matrices;

  while(!BlockList_isempty(blocks))
  {
    struct Block *block = BlockList_pop(blocks);
    *(tmp++) = ProcessBlock(final, block);
    free(block);
  }

  return block_matrices;
}
