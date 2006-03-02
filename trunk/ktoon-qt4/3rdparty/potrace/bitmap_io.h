/* Copyright (C) 2001-2005 Peter Selinger.
   This file is part of potrace. It is free software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

/* $Id: bitmap_io.h,v 1.3 2005/02/22 21:31:23 selinger Exp $ */
/* bitmap input/output functions */

#ifndef BITMAP_IO_H
#define BITMAP_IO_H

#include <stdio.h>
#include "bitmap.h"

/* Note that bitmaps are stored bottom to top, i.e., the first
   scanline is the bottom-most one */

extern char *bm_read_error;

int bm_read(FILE *f, double blacklevel, potrace_bitmap_t **bmp);
void bm_writepbm(FILE *f, potrace_bitmap_t *bm);
int bm_print(FILE *f, potrace_bitmap_t *bm);

#endif /* BITMAP_IO_H */

