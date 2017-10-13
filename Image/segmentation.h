#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <stdlib.h>
#include <SDL/SDL.h>

// explained here http://crblpocr.blogspot.fr/2007/06/run-length-smoothing-algorithm-rlsa.html
struct block
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

SDL_Surface *HorizontalSmoothing(SDL_Surface *img, int treshold);
SDL_Surface *VerticalSmoothing(SDL_Surface *img,int treshold);


#endif
