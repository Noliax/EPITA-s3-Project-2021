#include <stdlib.h>
#include "./block.h"
#include "./pixel_operations.h"


Block *Block_new(int x, int y, int width, int height)
{
  Block *b = malloc(sizeof(Block));
  b->min_x = x;
  b->min_y = y;
  b->width = width;
  b->height = height;
  return b;
}

void DisplayBlock(Block *block, SDL_Surface *source)
{
  Uint32 color = SDL_MapRGB(source->format, 255, 0, 0);
  for (int x = block->min_x; x < block->min_x + block->width; x++)
  {
    putpixel(source, x, block->min_y, color);
    putpixel(source, x, block->min_y + block->height, color);
  }
  for (int y = block->min_y; y < block->min_y + block->height; y++)
  {
    putpixel(source, block->min_x, y, color);
    putpixel(source, block->min_x + block->width, y, color);
  }
}
