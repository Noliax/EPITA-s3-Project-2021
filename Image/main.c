#include <stdlib.h>
#include "./utils.h"
#include "./segmentation.h"

void RLSA(SDL_Surface *img, int hsv, int vsv, int ahsv)
{
  SDL_Surface *copy = newWhiteSurface(img);

  img = display_image(img);

  // Set image colors to white or black
  BinarizeColors(img);

  // Save the white/black image to copy
  SDL_BlitSurface(img, NULL, copy, NULL);

  // Perfom RLSA
  SDL_Surface *horizontal = HorizontalSmoothing(copy, hsv);
  SDL_Surface *vertical = VerticalSmoothing(copy, vsv);
  SDL_Surface *and = MergeSmoothings(vertical, horizontal);
  horizontal = HorizontalSmoothing(and, ahsv);

  // Display final RLSA output
  SDL_BlitSurface(horizontal, NULL, img, NULL);
  WaitKeyToUpdate(img);

  // Show blocks
  Scan_Surface(img, copy);
  SDL_BlitSurface(copy, NULL, img, NULL);
  WaitKeyToUpdate(img);

  // Free surfaces
  SDL_FreeSurface(vertical);
  SDL_FreeSurface(copy);
  SDL_FreeSurface(horizontal);
 
}

int main()
{
  init_sdl();

  // value for words in my text
  int hsv = 8;
  int vsv = 30;
  int ahsv = 7;

  // Double horizontal to get lines
  // hsv *= 2;
  // ahsv *= 2;

  SDL_Surface *img = load_image("texte.png");

  RLSA(img, hsv*2, vsv, ahsv*3);
  RLSA(img, hsv, vsv, ahsv);
  RLSA(img, hsv, vsv, 2);

  SDL_FreeSurface(img);
 return 0;
}
