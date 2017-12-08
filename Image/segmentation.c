#include <stdlib.h>
#include <gtk/gtk.h>
#include "./block.h"

guchar *getpixel(GdkPixbuf *buffer, int i, int j)
{
  int row_offset = gdk_pixbuf_get_rowstride(buffer);
  int channels = gdk_pixbuf_get_n_channels(buffer);
  guchar *pixels = gdk_pixbuf_get_pixels(buffer);
  return pixels + j * row_offset + i * channels;
}

void setpixelrgb(guchar *pixel, guchar r, guchar g, guchar b)
{
  pixel[0] = r;
  pixel[1] = g;
  pixel[2] = b;
}

void setpixel(guchar *pixel, guchar val)
{
  pixel[0] = val;
  pixel[1] = val;
  pixel[2] = val;
}

double GreyscaleAverage(GdkPixbuf *buffer)
{
  double average = 0;
  
  guchar *pixels = gdk_pixbuf_get_pixels(buffer);
  gsize len = gdk_pixbuf_get_byte_length(buffer);
  guchar *end = pixels + len;

  int n_channels = gdk_pixbuf_get_n_channels(buffer);

  for(; pixels < end; pixels+=n_channels)
  {
    double greyscale = 0.3*pixels[0] + 0.59*pixels[1] + 0.11*pixels[2];
    setpixel(pixels, greyscale);
    average += greyscale;
  }
  average = average/len;

  return average;
}

void BinarizeColors(GdkPixbuf *buffer)
{
  int width = gdk_pixbuf_get_width(buffer);
  int height = gdk_pixbuf_get_height(buffer);

  // double -> int needs +0.5 because casting truncates decimal
  // e.g (int)(3.99999) gives 3
  double average = GreyscaleAverage(buffer);

  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      guchar *p = getpixel(buffer, i, j);
      guchar val = p[0] < 2.5*average  ? 0 : 255;
      setpixel(p, val);
    }
  }
}

GdkPixbuf *MergeSmoothings(GdkPixbuf *a, GdkPixbuf *b)
{
  int width = gdk_pixbuf_get_width(a);
  int height = gdk_pixbuf_get_height(a);
  GdkPixbuf *res = gdk_pixbuf_copy(a);
                      
  
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      guchar *pa = getpixel(a, i, j);
      guchar *pb = getpixel(b, i, j);
      guchar *res_pixel = getpixel(res, i, j);

      guchar value = *pa == 0 && *pb == 0 ? 0 : 255;
      setpixel(res_pixel, value);
    }
  }
  
  return res;
}

GdkPixbuf *HorizontalSmoothing(GdkPixbuf *a, int treshold)
{
  int width = gdk_pixbuf_get_width(a);
  int height = gdk_pixbuf_get_height(a);
  int n_channels = gdk_pixbuf_get_n_channels(a);

  GdkPixbuf *res = gdk_pixbuf_copy(a);   

  int last_black;
  for (int j = 0; j < height; j++)
  {
    last_black = width;
    for (int i = 0; i < width; i++)
    {
      guchar *pixel = getpixel(a, i, j);

      if (pixel[0] == 0)
      {
        int gap = i - last_black + 1;
        if (2 < gap && gap <= treshold)
        {
          int toFill = last_black + 1;
          guchar *fill_pixel = getpixel(res, toFill, j);
          for (; toFill <= i; toFill++) {
            setpixel(fill_pixel, 0);
            fill_pixel += n_channels;
          }
        }
        last_black = i;
      }
    }
  }
  return res;
}
GdkPixbuf *VerticalSmoothing(GdkPixbuf *a, int treshold)
{
  int width = gdk_pixbuf_get_width(a);
  int height = gdk_pixbuf_get_height(a);
  int n_channels = gdk_pixbuf_get_n_channels(a);

  GdkPixbuf *res = gdk_pixbuf_copy(a);   

  int last_black;
  for (int i = 0; i < width; i++)
  {
    last_black = height;
    for (int j = 0; j < height; j++)
    {
      guchar *pixel = getpixel(a, i, j);

      if (pixel[0] == 0)
      {
        int gap = j - last_black + 1;
        if (2 < gap && gap <= treshold)
        {
          int toFill = last_black + 1;
          guchar *fill_pixel = getpixel(res, i, toFill);
          for (; toFill <= j; toFill++) {
            setpixel(fill_pixel, 0);
            fill_pixel += width*n_channels;
          }
        }
        last_black = j;
      }
    }
  }
  return res;
}

GdkPixbuf *Scan_Surface(GdkPixbuf *source, GdkPixbuf *draw, struct BlockList* list)
{
  GdkPixbuf *res = gdk_pixbuf_copy(draw);
  int width = gdk_pixbuf_get_width(source);
  int height = gdk_pixbuf_get_height(source);

  int last_white_line = -1;
  for (int j = 0; j < height; j++)
  {
    // Count the nb of black pixels (and stop when there is one)
    int blacks_found = 0;
    for (int i = 0; i < width && blacks_found == 0; i++)
    {
      guchar *pixel = getpixel(source, i, j);
      if (*pixel == 0)
        blacks_found++;
    }

    if (blacks_found == 0)
    {
      // We found two different blank lines
      // There is probably a block between j and last_white_line
      if (last_white_line > 0 && j - last_white_line > 1)
      {
        int last_white_col = -1;
        for (int i = 0; i < width; i++)
        {
          // Count the nb of black pixels
          int col_lum = 0;
          for (int y = last_white_line + 1; y < j; y++)
          {
            guchar *pixel = getpixel(source, i, y);
            col_lum += pixel[0];
          }
          col_lum = col_lum / (j - last_white_line);

          // Allow one pixel to separate blocks
          if (col_lum > 240)
          {
            /*
            There is probably a block between i, last_white_col, j and last_white_line

              (lwc, lwl) ---------- (i, lwl)
                  |                    |
                  |                    |
               (lwc, j) ----------- (i, j)
            */ 
            if (last_white_col > 0 && i - last_white_col > 1)
            {
              struct Block *block = Block_new(
                  last_white_col,
                  last_white_line,
                  i - last_white_col,
                  j - last_white_line);

              // Draw the block
              guchar *p1;
              guchar *p2; 

              // Crop the white on the top and bottom of the block
              for(int dy = 0; dy < block->height; dy++)
              {
                int is_white = 1;
                for(int dx = 0; dx < block->width && is_white; dx++)
                {
                  p1 = getpixel(source, block->min_x + dx, block->min_y + dy);
                  if(*p1 == 0)
                    is_white = 0; 
                }
                if(!is_white)
                {
                  block->min_y += dy - 1;
                  block->height -= dy -1;
                  break;
                }
              }
              for(int dy = block->height - 1; dy >= 0; dy--)
              {
                int is_white = 1;
                for(int dx = 0; dx < block->width && is_white; dx++)
                {
                  p1 = getpixel(source, block->min_x + dx, block->min_y + dy);
                  if(*p1 == 0)
                    is_white = 0; 
                }
                if(!is_white)
                {
                  block->height -= block->height - 1 - dy;
                  break;
                }
              }
              
              //Display
              for(int dy = 0; dy < block->height; dy++)
              {
                p1 = getpixel(res, block->min_x, block->min_y + dy); 
                p2 = getpixel(res, block->min_x + block->width, block->min_y + dy); 
                setpixelrgb(p1, 255, 0, 0);
                setpixelrgb(p2, 255, 0, 0);
              } 
              for(int dx = 0; dx < block->width; dx++)
              {
                p1 = getpixel(res, block->min_x + dx, block->min_y); 
                p2 = getpixel(res, block->min_x + dx, block->min_y + block->height); 
                setpixelrgb(p1, 255, 0, 0);
                setpixelrgb(p2, 255, 0, 0);
              }

              BlockList_push(list, block);
            }
            last_white_col = i;
          }
        }
      }
      last_white_line = j;
    }
  }
  return res;
}

