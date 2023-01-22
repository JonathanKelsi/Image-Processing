#include <stdbool.h> 
#include "myfunction1.h"
#include "myfunction.h"
#include "writeBMP.h"


// ================================ Compiler Flags ================================


#pragma GCC optimize("Ofast", "fast-math","unroll-loops", "omit-frame-pointer", "inline-functions", "no-stack-protector")
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")


// ================================ Image Processing Functions ================================


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* rowBlurRsltImgName, char* rowSharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {
	if (flag == '1') {	
		// blur image
		unfiltered_blur((unsigned char*) image->data);
		
		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		unfiltered_sharp((unsigned char*) image->data);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
    } else if (flag == '2') {	
		// blur image with row-blurring kernel
		unfiltered_row_blur((unsigned char*) image->data);

		// write result image to file
		writeBMP(image, srcImgpName, rowBlurRsltImgName);

		// sharpen the resulting image with row-sharpening kernel
		unfiltered_row_sharpen((unsigned char*) image->data);

		// write result image to file
		writeBMP(image, srcImgpName, rowSharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		filtered_blur((unsigned char*) image->data);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		unfiltered_sharp((unsigned char*) image->data);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

