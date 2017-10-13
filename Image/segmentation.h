#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <stdlib.h>
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
}
typedef struct Block Block;

// Perform smoothing in different directions
SDL_Surface *HorizontalSmoothing(SDL_Surface *img, int treshold);
SDL_Surface *VerticalSmoothing(SDL_Surface *img,int treshold);

SDL_Surface *MergeSmoothings(SDL_Surface *horizontal, SDL_Surface *vertical);

// Transform a surface with blacks blocks into struct
Block Scan_Surface(SDL_Surface *original_image, SDL_Surface *blocksSurface);
#endif
