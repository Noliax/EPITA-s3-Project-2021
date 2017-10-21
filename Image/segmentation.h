#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <SDL/SDL.h>

void BinarizeColors(SDL_Surface *img);
SDL_Surface *newWhiteSurface(SDL_Surface *img);


// Perform smoothing in different directions
SDL_Surface *HorizontalSmoothing(SDL_Surface *img, int treshold);
SDL_Surface *VerticalSmoothing(SDL_Surface *img, int treshold);

SDL_Surface *MergeSmoothings(SDL_Surface *horizontal, SDL_Surface *vertical);

// Transform a surface with blacks blocks into struct
void Scan_Surface(SDL_Surface *blocks, SDL_Surface *source);
#endif