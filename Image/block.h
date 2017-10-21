#ifndef BLOCK_H_
#define BLOCK_H_

#include <SDL/SDL.h>

// explained here http://crblpocr.blogspot.fr/2007/06/run-length-smoothing-algorithm-rlsa.html
struct Block 
{
    int black_pixels;
    int min_x;
    int min_y;
    int width;
    int height;
    int original_black_pixels;
    int horizontal_white_transitions;

    int eccentricity;
    int s;
    int mean_horizontal_length;
};
typedef struct Block Block;

Block *Block_new(int x, int y, int width, int height);
void DisplayBlock(Block *block, SDL_Surface *source);

#endif