#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <gtk/gtk.h>
#include "./block.h"

void BinarizeColors(GdkPixbuf *img);


// Perform smoothing in different directions
GdkPixbuf *HorizontalSmoothing(GdkPixbuf *img, int treshold);
GdkPixbuf *VerticalSmoothing(GdkPixbuf *img, int treshold);

GdkPixbuf *MergeSmoothings(GdkPixbuf *horizontal, GdkPixbuf *vertical);

// Transform a surface with blacks blocks into struct
GdkPixbuf *Scan_Surface(GdkPixbuf *source, GdkPixbuf *draw, struct BlockList *list);
#endif
