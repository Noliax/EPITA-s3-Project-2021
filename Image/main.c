#include <stdlib.h>
#include "./utils.h"
#include "./segmentation.h"

int main()
{
  init_sdl();

  // value for words in my text
  int hsv = 7;
  int vsv = 35;
  int ahsv = 6;

  // Double horizontal to get lines
  // hsv *= 2;
  // ahsv *= 2;

  SDL_Surface *img = load_image("texte.png");
  SDL_Surface *copy = newWhiteSurface(img);

  img = display_image(img);

  // Set image colors to white or black
  BinarizeColors(img);
  WaitKeyToUpdate(img);

  // Save the white/black image to copy
  SDL_BlitSurface(img, NULL, copy, NULL);

  SDL_Surface *horizontal = HorizontalSmoothing(copy, hsv);
  SDL_BlitSurface(horizontal, NULL, img, NULL);
  WaitKeyToUpdate(img);

  SDL_Surface *vertical = VerticalSmoothing(copy, vsv);
  SDL_BlitSurface(vertical, NULL, img, NULL);
  WaitKeyToUpdate(img);

  SDL_Surface *and = MergeSmoothings(vertical, horizontal);
  SDL_BlitSurface(and, NULL, img, NULL);
  WaitKeyToUpdate(img);

  horizontal = HorizontalSmoothing(img, ahsv);
  SDL_BlitSurface(horizontal, NULL, img, NULL);
  WaitKeyToUpdate(img);

  Scan_Surface(img, copy);
  SDL_BlitSurface(copy, NULL, img, NULL);
  WaitKeyToUpdate(img);

  SDL_FreeSurface(img);
  SDL_FreeSurface(vertical);
  SDL_FreeSurface(copy);
  SDL_FreeSurface(horizontal);
  return 0;
}
