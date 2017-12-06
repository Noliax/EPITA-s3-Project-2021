#include <stdlib.h>
#include <SDL/SDL.h>
#include "./pixel_operations.h"
#include "./block.h"

void BinarizeColors(SDL_Surface *img)
{
  Uint32 blackPixel = SDL_MapRGB(img->format, 0, 0, 0);
  Uint32 whitePixel = SDL_MapRGB(img->format, 255, 255, 255);
  for (int i = 0; i < img->w; i++)
  {
    for (int j = 0; j < img->h; j++)
    {
      Uint32 pixel = getpixel(img, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, img->format, &r, &g, &b);
      double average = (r + g + b) / 3;
      putpixel(img, i, j, average < 200 ? blackPixel : whitePixel);
    }
  }
}

SDL_Surface *newWhiteSurface(SDL_Surface *img)
{
  SDL_Surface *result = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);
  SDL_FillRect(result, NULL, SDL_MapRGB(result->format, 255, 255, 255));
  return result;
}

SDL_Surface *MergeSmoothings(SDL_Surface *une, SDL_Surface *deux)
{
  SDL_Surface *res = newWhiteSurface(une);

  Uint8 a;
  Uint8 b;
  Uint8 c;

  Uint8 d;
  Uint8 e;
  Uint8 f;

  Uint32 prem;
  Uint32 sec;
  Uint32 fin;
  for (int i = 0; i < une->w; i++)
  {
    for (int j = 0; j < une->h; j++)
    {
      prem = getpixel(une, i, j);
      sec = getpixel(deux, i, j);
      SDL_GetRGB(prem, une->format, &a, &b, &c);
      SDL_GetRGB(sec, deux->format, &d, &e, &f);
      if (a == 0 && b == 0 && c == 0 && d == 0 && f == 0 && e == 0)
      {
        fin = SDL_MapRGB(res->format, 0, 0, 0);
        putpixel(res, i, j, fin);
      }
    }
  }
  return res;
}

SDL_Surface *HorizontalSmoothing(SDL_Surface *img, int treshold)
{
  SDL_Surface *result = newWhiteSurface(img);

  int last_black = img->w;
  Uint32 blackPixel = SDL_MapRGB(img->format, 0, 0, 0);

  for (int j = 0; j < img->h; j++)
  {
    last_black = img->w;
    for (int i = 0; i < img->w; i++)
    {
      Uint32 pixel = getpixel(img, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, img->format, &r, &g, &b);

      if (r == 0 && g == 0 && b == 0)
      {
        int gap = i - last_black + 1;
        if (2 < gap && gap <= treshold)
        {
          for (int toFill = last_black + 1; toFill < i; toFill++)
            putpixel(result, toFill, j, blackPixel);
        }
        putpixel(result, i, j, blackPixel);
        last_black = i;
      }
    }
  }
  return result;
}

SDL_Surface *VerticalSmoothing(SDL_Surface *img, int treshold)
{
  SDL_Surface *result = newWhiteSurface(img);

  int last_black = img->h;
  Uint32 blackPixel = SDL_MapRGB(img->format, 0, 0, 0);

  for (int i = 0; i < img->w; i++)
  {
    last_black = img->h;
    for (int j = 0; j < img->h; j++)
    {
      Uint32 pixel = getpixel(img, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, img->format, &r, &g, &b);

      if (r == 0 && g == 0 && b == 0)
      {
        int gap = j - last_black + 1;
        if (2 < gap && gap <= treshold)
        {
          for (int toFill = last_black + 1; toFill < j; toFill++)
            putpixel(result, i, toFill, blackPixel);
        }
        putpixel(result, i, j, blackPixel);
        last_black = j;
      }
    }
  }
  return result;
}

void Scan_Surface(SDL_Surface *blocks, SDL_Surface *source)
{
  int last_white_line = -1;
  for (int j = 0; j < blocks->h; j++)
  {
    // Count the nb of black pixels (and stop when there is one)
    int blacks_found = 0;
    for (int i = 0; i < blocks->w && blacks_found == 0; i++)
    {
      Uint32 pixel = getpixel(blocks, i, j);
      if (pixel == 0)
        blacks_found++;
    }

    if (blacks_found == 0)
    {
      // We found two different blank lines
      // There is probably a block between j and last_white_line
      if (last_white_line > 0 && j - last_white_line > 1)
      {
        int last_white_col = -1;
        for (int i = 0; i < blocks->w; i++)
        {
          // Count the nb of black pixels
          int blacks_found_col = 0;
          for (int y = last_white_line + 1; y < j; y++)
          {
            Uint32 pixel = getpixel(blocks, i, y);
            if (pixel == 0)
              blacks_found_col++;
          }

          // Allow one pixel to separate blocks
          if (blacks_found_col < 2)
          {
            /*There is probably a block between i, last_white_col, j and last_white_line

              (lwc, lwl) ---------- (i, lwl)
                  |                    |
                  |                    |
               (lwc, j) ----------- (i, j)
            */
            if (last_white_col > 0 && i - last_white_col > 1)
            {
              Block *block = Block_new(
                  last_white_col,
                  last_white_line,
                  i - last_white_col,
                  j - last_white_line);
              DisplayBlock(block, source);
            }
            last_white_col = i;
          }
        }
      }
      last_white_line = j;
    }
  }
}
