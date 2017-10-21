#ifndef BLOCK_H_
#define BLOCK_H_

#include <SDL/SDL.h>

void WaitKeyToUpdate(SDL_Surface *img);
void wait_for_keypressed(void);
void init_sdl(void);
SDL_Surface *load_image(char *path);
SDL_Surface *display_image(SDL_Surface *img);

#endif