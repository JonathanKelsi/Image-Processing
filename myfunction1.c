#include <string.h>
#include <immintrin.h>
#include "myfunction1.h"
#include "showBMP.h"


// ================================ Compiler Flags ================================


#pragma GCC optimize("Ofast", "fast-math","unroll-loops", "omit-frame-pointer", "inline-functions", "no-stack-protector")
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target ("avx2")


// ================================ Image Processing Functions ================================


/**
 * Apply a blur kernel to a given image.
 * The kernel is a 3x3 matrix with the following values:
 * 						[1, 1, 1]
 * 				  1/9 * [1, 1, 1]
 * 						[1, 1, 1]
 */
inline void unfiltered_blur(unsigned char* image) {
	// Create a backup of the given image
    int n3 = n*3, len = n*n3;
	unsigned char* backup = malloc(len);
	memcpy(backup, image, len);

	// Setup
	int middle, up, down, in3;
	short i, j, size = n-1;
	unsigned char* backup_down_ptr, * backup_middle_ptr, * backup_up_ptr, * image_ptr;

	// For each pixel in the image that has a big enough neighborhood, apply the kernel
	for (i = 1 ; i < size; ++i) {
        in3 = i*n3;

		for (j =  1 ; j < size; ++j) {
			// Calculate the locations of the pixel in the neighborhood of [i,j]
            middle = in3 + j*3;
			up = middle - n3;
            down = middle + n3;

			image_ptr = image + middle;
			backup_down_ptr = backup + down;
			backup_middle_ptr = backup + middle;
			backup_up_ptr = backup + up;

			// Calculate the convolution for the current pixel
			image_ptr[0] = (backup_middle_ptr[-3] + backup_middle_ptr[0] + backup_middle_ptr[3] + backup_up_ptr[-3] + backup_up_ptr[0] + backup_up_ptr[3] + backup_down_ptr[-3] + backup_down_ptr[0] + backup_down_ptr[3]) / 9;
			image_ptr[1] = (backup_middle_ptr[-2] + backup_middle_ptr[1] + backup_middle_ptr[4] + backup_up_ptr[-2] + backup_up_ptr[1] + backup_up_ptr[4] + backup_down_ptr[-2] + backup_down_ptr[1] + backup_down_ptr[4]) / 9;
			image_ptr[2] = (backup_middle_ptr[-1] + backup_middle_ptr[2] + backup_middle_ptr[5] + backup_up_ptr[-1] + backup_up_ptr[2] + backup_up_ptr[5] + backup_down_ptr[-1] + backup_down_ptr[2] + backup_down_ptr[5]) / 9;
		}
	}

	free(backup);
}

/**
 * Apply a sharp kernel to a given image.
 * The kernel is a 3x3 matrix with the following values:
 * 					[-1, -1, -1]
 * 				    [-1,  9, -1]
 * 					[-1, -1, -1]
 */
inline void unfiltered_sharp(unsigned char* image) {
    // Create a backup of the given image
    int n3 = n*3, len = n*n3;
	unsigned char* backup = malloc(len);
	memcpy(backup, image, len);

	// Setup
	int middle, up, down, in3;
	short i, j, size = n-1, conv, max_conv0;
	unsigned char* backup_down_ptr, * backup_middle_ptr, * backup_up_ptr, * image_ptr;

	// For each pixel in the image that has a big enough neighborhood, apply the kernel
	for (i = 1 ; i < size; ++i) {
        in3 = i*n3;

		for (j =  1 ; j < size; ++j) {
			// Calculate the locations of the pixel in the neighborhood of [i,j]
            middle = in3 + j*3;
			up = middle - n3;
            down = middle + n3;

			image_ptr = image + middle;
			backup_down_ptr = backup + down;
			backup_middle_ptr = backup + middle;
			backup_up_ptr = backup + up;

			// Calculate the convolution for the current pixel
			conv = 9*backup_middle_ptr[0] - (backup_middle_ptr[-3] + backup_middle_ptr[3] + backup_up_ptr[-3] + backup_up_ptr[0] + backup_up_ptr[3] + backup_down_ptr[-3] + backup_down_ptr[0] + backup_down_ptr[3]);
			max_conv0 = conv & -(0 < conv);
			image_ptr[0] = 255 ^ ((max_conv0 ^ 255) & -(max_conv0 < 255));

			conv = 9*backup_middle_ptr[1] - (backup_middle_ptr[-2] + backup_middle_ptr[4] + backup_up_ptr[-2] + backup_up_ptr[1] + backup_up_ptr[4] + backup_down_ptr[-2] + backup_down_ptr[1] + backup_down_ptr[4]);
			max_conv0 = conv & -(0 < conv);
			image_ptr[1] = 255 ^ ((max_conv0 ^ 255) & -(max_conv0 < 255));

			conv = 9*backup_middle_ptr[2] - (backup_middle_ptr[-1] + backup_middle_ptr[5] + backup_up_ptr[-1] + backup_up_ptr[2] + backup_up_ptr[5] + backup_down_ptr[-1] + backup_down_ptr[2] + backup_down_ptr[5]);
			max_conv0 = conv & -(0 < conv);
			image_ptr[2] = 255 ^ ((max_conv0 ^ 255) & -(max_conv0 < 255));
		}
	}

	free(backup);
}

/**
 * Apply a row blur kernel to a given image.
 * The kernel is a 3x3 matrix with the following values:
 * 					[0, 0, 0]
 * 				    [1, 2, 1]
 * 					[0, 0, 0]
 */
inline void unfiltered_row_blur(unsigned char* image) {
    // Create a backup of the given image
    int n3 = n*3, len = n*n3;
	unsigned char* backup = malloc(len);
	memcpy(backup, image, len);

	// Setup
	int index, in3;
	short i, j, size = n-1;
	unsigned char* backup_ptr, * image_ptr;

	// For each pixel in the image that has a big enough neighborhood, apply the kernel
	for (i = 1; i < size; ++i) {
        in3 = i*n3;

		for (j =  1; j < size; ++j) {
			// Calculate the location of the pixel at [i,j]
			index = in3 + 3*j;
			image_ptr = image + index;
			backup_ptr = backup + index;

			// Calculate the convolution for the current pixel
			image_ptr[0] = (2*backup_ptr[0] + backup_ptr[-3] + backup_ptr[3]) / 4;
			image_ptr[1] = (2*backup_ptr[1] + backup_ptr[-2] + backup_ptr[4]) / 4;
			image_ptr[2] = (2*backup_ptr[2] + backup_ptr[-1] + backup_ptr[5]) / 4;
		}	
	}

	free(backup);
}

/**
 * Apply a row blur kernel to a given image.
 * The kernel is a 3x3 matrix with the following values:
 * 					[0,  0,  0]
 * 		      1/2 * [-2, 6, -2]
 * 					[0,  0,  0]
 */
inline void unfiltered_row_sharpen(unsigned char* image) {
    // Create a backup of the given image
    int n3 = n*3, len = n*n3;
	unsigned char* backup = malloc(len);
	memcpy(backup, image, len);

	// Setup
	int index, in3;
	short i, j, size = n-1, conv, max_conv0;
	unsigned char* backup_ptr, * image_ptr;

	// For each pixel in the image that has a big enough neighborhood, apply the kernel
	for (i = 1; i < size; ++i) {
        in3 = i*n3;

		for (j =  1; j < size; ++j) {
			// Calculate the location of the pixel at [i,j]
			index = in3 + 3*j;
			image_ptr = image + index;
			backup_ptr = backup + index;

			// Calculate the convolution for the current pixel
			conv = 3*backup_ptr[0] - (backup_ptr[-3] + backup_ptr[3]);
			max_conv0 = conv & -(0 < conv);
			image_ptr[0] = 255 ^ ((max_conv0 ^ 255) & -(max_conv0 < 255));

			conv = 3*backup_ptr[1] - (backup_ptr[-2] + backup_ptr[4]);
			max_conv0 = conv & -(0 < conv);
			image_ptr[1] = 255 ^ ((max_conv0 ^ 255) & -(max_conv0 < 255));

			conv = 3*backup_ptr[2] - (backup_ptr[-1] + backup_ptr[5]);
			max_conv0 = conv & -(0 < conv);
			image_ptr[2] = 255 ^ ((max_conv0 ^ 255) & -(max_conv0 < 255));
		}	
	}

	free(backup);
}

/**
 * Apply the same kernel as 'unfiltered_blur', but filter out the most and least dominate pixels.
 */
inline void filtered_blur(unsigned char* image) {
	// Create a backup of the given image
	int n3 = n*3, len = n*n3, in3;
	unsigned char* backup = malloc(len);
	memcpy(backup, image, len);

	// Setup
	short i, j, size = n - 1, min, max, temp;
	unsigned short conv_r, conv_g, conv_b;
	unsigned char max_r,max_g, max_b, min_r, min_g, min_b, * image_ptr, * backup_ptr, * window_ptr;

	// For each pixel (with a big enough neighborhood), find the most and least dominate pixels in the neighborhood
	// while simultaneously calculating the convolution for the current pixel
	for (i = 1; i < size; ++i) {
		// Initialize the pointers to the current row
		in3 = i*n3;
		image_ptr = image + in3 + 3;
		backup_ptr = backup + in3 + 3;

		for (j = 1 ; j < size; ++j) {
			// Initialize the window pointer to the top left pixel in the neighborhood		
			window_ptr = backup_ptr -(n3 + 3);

			// Calculate conv. and min/max for the top row
			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			min = max = temp;
			conv_r = backup_ptr[-n3-3]; conv_g =  backup_ptr[-n3-2]; conv_b =  backup_ptr[-n3-1];
			max_r = conv_r; max_g = conv_g; max_b = conv_b;
			min_r = conv_r; min_g = conv_g; min_b = conv_b;

			window_ptr += 3;

			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];
			
			window_ptr += 3;

			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			window_ptr = backup_ptr - 3;

			// Calculate conv. and min/max for the middle row
			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			window_ptr += 3;

			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			window_ptr += 3;

			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			window_ptr = backup_ptr + n3 - 3;

			//  Calculate conv. and min/max for the bottom row
			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			window_ptr += 3;

			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min = temp; min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max = temp; max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			window_ptr += 3;

			temp = window_ptr[0] + window_ptr[1] + window_ptr[2];
			if (min >= temp) { min_r = window_ptr[0]; min_g = window_ptr[1]; min_b = window_ptr[2]; }
			if (temp > max) { max_r = window_ptr[0]; max_g = window_ptr[1]; max_b = window_ptr[2]; }
			conv_r += window_ptr[0]; conv_g += window_ptr[1]; conv_b += window_ptr[2];

			// Subtract the weight of the filtered elements from the convolution, and normalize the result
			image_ptr[0] = (conv_r - (max_r + min_r)) / 7;
			image_ptr[1] = (conv_g - (max_g + min_g)) / 7;
			image_ptr[2] = (conv_b - (max_b + min_b)) / 7;

			// Move the image and backup pointers to the next pixel
			image_ptr += 3;
			backup_ptr += 3;
		}
	}

	free(backup);
}