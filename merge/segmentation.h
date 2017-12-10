#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include <gtk/gtk.h>
#include "./block.h"

struct BlockList *RLSA(GdkPixbuf *buffer, int hsv, int vsv, int ahsv);

#endif
