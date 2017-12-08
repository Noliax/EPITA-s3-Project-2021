#ifndef BLOCK_H_
#define BLOCK_H_

#include <gtk/gtk.h>

// explained here http://crblpocr.blogspot.fr/2007/06/run-length-smoothing-algorithm-rlsa.html
struct Block 
{
    int min_x;
    int min_y;
    int width;
    int height;

    // ?????
    //int black_pixels;
    //int original_black_pixels;
    //int horizontal_white_transitions;
    //int eccentricity;
    //int s;
    //int mean_horizontal_length;
};

struct Block *Block_new(int x, int y, int width, int height);

struct BlockList
{
  struct Block *curr;
  struct BlockList *next;
};


void BlockList_init(struct BlockList *list);
int BlockList_isempty(struct BlockList *list);
void BlockList_push(struct BlockList *list, struct Block *elm);
struct Block *BlockList_pop(struct BlockList *list);
void BlockList_destroy(struct BlockList *list);
int* ProcessBlock(GdkPixbuf *source, struct Block *block);

#endif
