#ifndef __myfunc1_h
#define __myfunc1_h

#include "readBMP.h"

short fmin_i(short arr[8]);
short fmax_i(short arr[8]);

void unfiltered_blur(unsigned char* image);
void filtered_blur(unsigned char* image);
void unfiltered_sharp(unsigned char* image);
void unfiltered_row_blur(unsigned char* image);
void unfiltered_row_sharpen(unsigned char* image);

#endif

