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

struct BlockList
{
  size_t capacity, size;
  struct Block **data;
};



// Block
struct Block *Block_new(int x, int y, int width, int height);
int* ProcessBlock(GdkPixbuf *source, struct Block *block);
int **BlocksToMat(GdkPixbuf *final, struct BlockList *blocks);
void DisplayBlockRGB(GdkPixbuf *buffer, struct Block *block, int r, int g, int b);
void DisplayBlock(GdkPixbuf *buffer, struct Block *block);
void CropWhite(GdkPixbuf *source, struct Block *block);


// Block List
struct BlockList *BlockList_new(size_t capacity);
int BlockList_push(struct BlockList *list, struct Block *elm);
void BlockList_destroy(struct BlockList *list);

void print_mat(int* mat);
#endif
