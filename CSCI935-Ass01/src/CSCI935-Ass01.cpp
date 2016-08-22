/*------------------------------------------------------
 Student's Name: Li Baoxing
 Student's number: 5083497
 Student's email address: bl251@uowmail.edu.au
 -------------------------------------------------------*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

using namespace std;

#pragma pack(push, 1)
/**** BMP file header structure ****/
typedef struct {
	unsigned short bfType; /* BM */
	unsigned int bfSize; /* Size of file */
	unsigned short bfReserved1; /* Reserved */
	unsigned short bfReserved2; /* ... */
	unsigned int bfOffBits; /* Offset to bitmap data */
} FILEHEADER;

/**** BMP file info structure ****/
typedef struct {
	unsigned int biSize; /* Size of info header */
	int width; /* Width of image */
	int height; /* Height of image */
	unsigned short planes; /* Number of color planes */
	unsigned short bitPix; /* Number of bits per pixel */
	unsigned int biCompression; /* Type of compression to use */
	unsigned int biSizeImage; /* Size of image data */
	int biXPelsPerMeter; /* X pixels per meter */
	int biYPelsPerMeter; /* Y pixels per meter */
	unsigned int biClrUsed; /* Number of colors used */
	unsigned int biClrImportant; /* Number of important colors */
} INFOHEADER;

/**** Colormap pixel structure ****/
typedef struct {
	unsigned char b; /* Blue value */
	unsigned char g; /* Green value */
	unsigned char r; /* Red value */
} PIX;

#pragma pop

/*-- Function prototypes --*/
unsigned char **alloc2D(int height, int width);
PIX **alloc2Drgb(int height, int width);
void free2D(unsigned char **mar);
void free2Drgb(PIX **mar);
bool processCFAImage(unsigned char **cfaImage, PIX **rgbImage, int width, int height);
void _CFAInterpolation(unsigned char **cfaImage, PIX **rgbImage, int width, int height);
void _ColourCorrection(PIX **rgbImage, int width, int height, float _ColorCorrectionMatrix[][3]);
void _GammaCorrection(PIX **rgbImage, int width, int height, float gamma);

#include <cstring>

int main(void) {
	unsigned char **cfaImage; /* 2D array of CFA image pixels */
	PIX **rgbImage; /* 2D array of RGB image pixels */

	char fileName[20];
	FILEHEADER header;
	INFOHEADER info;
	FILE *inFile, *outFile;
	bool errCode;
	int i;

	//printf("Input CFA file name:");
	//scanf("%s", fileName);
	strcpy(fileName, "test2.bmp");

	/*-- Open the BMP file with CFA samples --*/
	if ((inFile = fopen(fileName, "rb")) == NULL) {
		fprintf(stderr, "Error Opening BMP File");
		return -1;

	}
	/*-- read the file header --*/
	if (fread(&header, sizeof(FILEHEADER), 1, inFile) != 1) {
		fprintf(stderr, "Error reading File Header");
		fclose(inFile);
		return -1;
	}

	/*-- Check BM marker --*/
	if (header.bfType != 0x4D42) {
		fprintf(stderr, "Unsupported file format");
		fclose(inFile);
		return -1;
	}
	/*-- read info --*/
	if (fread(&info, sizeof(INFOHEADER), 1, inFile) != 1) {
		fprintf(stderr, "Error reading Info Header");
		fclose(inFile);
		return -1;
	}

	/* allocate memory for cfaImage, 1 byte per pixel */
	cfaImage = alloc2D(info.height, info.width);
	if (cfaImage == NULL) {
		fprintf( stderr, " Memory allocation error");
		return -1;
	}

	fseek(inFile, header.bfOffBits, SEEK_SET);
	for (i = 0; i < info.height; i++)
		if (fread(cfaImage[i], sizeof(unsigned char), info.width, inFile) != info.width) {
			fprintf(stderr, "Error reading Image data");
			fclose(inFile);
			return -1;
		}

	/* allocate memory for an RGB Image, 3 bytes per pixel */
	rgbImage = alloc2Drgb(info.height, info.width);
	if (rgbImage == NULL) {
		fprintf( stderr, " Memory allocation error");
		return -1;
	}

	/*----- produce an RGB image from a CFA image ----*/
	errCode = processCFAImage(cfaImage, rgbImage, info.width, info.height);
	if (errCode == false) {
		printf("Data processing error \n");
		return -1;
	}

	/*-------- Save RGB file --------*/
	//printf("RGB file name:");
	//scanf("%s", fileName);
	strcpy(fileName, "t2-03.bmp");
	/*-- Open the BMP file --*/
	if ((outFile = fopen(fileName, "wb")) == NULL) {
		fprintf(stderr, "Error Opening output BMP File");
		return -1;

	}
	/*-- write the file header --*/
	if (fwrite(&header, sizeof(FILEHEADER), 1, outFile) != 1) {
		fprintf(stderr, "Error writing File Header");
		fclose(outFile);
		return -1;
	}

	info.bitPix = 24; /* change to 24 bit/pix for true colour images*/
	/*-- write the info header --*/
	if (fwrite(&info, sizeof(INFOHEADER), 1, outFile) != 1) {
		fprintf(stderr, "Error writing Info Header");
		fclose(outFile);
		return -1;
	}

	/*-- write pixel data --*/
	fseek(outFile, header.bfOffBits, SEEK_SET);
	for (i = 0; i < info.height; i++)
		if (fwrite(rgbImage[i], sizeof(PIX), info.width, outFile) != info.width) {
			fprintf(stderr, "Error writing RGB Image data");
			fclose(outFile);
			return -1;
		}

	free2D(cfaImage);
	free2Drgb(rgbImage);
	fclose(inFile);
	fclose(outFile);

	return 0;
}

unsigned char **alloc2D(int height, int width) {
	int i;
	unsigned int nelem;
	unsigned char *p, **pp;

	nelem = height * width;
	p = (unsigned char*) calloc(nelem, 1);
	if (p == NULL)
		return NULL;
	pp = (unsigned char**) calloc(height, sizeof(unsigned char*));
	if (pp == NULL) {
		free(p);
		return NULL;
	}
	for (i = 0; i < height; i++)
		pp[i] = p + i * width;

	return pp;
}

void free2D(unsigned char **mar) {
	if (mar != NULL)
		free((unsigned char*) *mar);
	free((unsigned char**) mar);

	return;
}

PIX **alloc2Drgb(int height, int width) {
	int i;
	unsigned int nelem;
	PIX *p, **pp;

	nelem = height * width;
	p = (PIX*) calloc(nelem, 3);
	if (p == NULL)
		return NULL;
	pp = (PIX**) calloc(height, sizeof(PIX*));
	if (pp == NULL) {
		free(p);
		return NULL;
	}
	for (i = 0; i < height; i++)
		pp[i] = p + i * width;

	return pp;
}

void free2Drgb(PIX **mar) {
	if (mar != NULL)
		free((PIX*) *mar);
	free((PIX**) mar);

	return;
}

/*================ Your code starts here ========================*/

bool processCFAImage(unsigned char **cfaImage, PIX **rgbImage, int width, int height) {
	int row, col;

	/* an example that shows how to access pixels and r,g,b components */
	for (row = 0; row < height; row++)
		for (col = 0; col < width; col++) {
			// r,g and b, they are the same value??
			rgbImage[row][col].r = cfaImage[row][col];
			rgbImage[row][col].g = cfaImage[row][col];
			rgbImage[row][col].b = cfaImage[row][col];
		}
	_CFAInterpolation(cfaImage, rgbImage, width, height);
	float _ColorCorrectionMatrix[3][3] = { { 1.18, -0.09, -0.13 }, { -0.24, 1.29, -0.05 }, { -0.18, -0.44, 1.71 } };
	_ColourCorrection(rgbImage, width, height, _ColorCorrectionMatrix);
	float gamma = 0.5;
	_GammaCorrection(rgbImage, width, height, gamma);
	return true;
}

// Bayer Pattern CFA
// the first row:  G R G R G R ...
// the second row: B G B G B G ...
// it means that:
// the odd row:    G R G R G R ...
// the even row:   B G B G B G ...
// in this function, It implement bilinear algorithm
// to do interpolation.
void _CFAInterpolation(unsigned char **cfaImage, PIX **rgbImage, int width, int height) {
	int row, col;
	unsigned char x, y;
	for (row = 1; row < height - 1; row++) {
		for (col = 1; col < width - 1; col++) {
			// ignore the border (1px)
			// I guest the Bayer pattern is:
			// BGBGBGBGBG
			// GRGRGRGRGR
			// BGBGBGBGBG
			// GRGRGRGRGR
			if ((row + col) % 2 != 0) {
				//GBG        GRG
				//RgR   or   BgB
				//GBG        GRG
				rgbImage[row][col].g = cfaImage[row][col];
				x = (cfaImage[row][col - 1] + cfaImage[row][col + 1]) / 2;				// left/right
				y = (cfaImage[row - 1][col] + cfaImage[row + 1][col]) / 2;				// up/down
				if (row % 2 != 0) {
					rgbImage[row][col].r = x;
					rgbImage[row][col].b = y;
				} else {
					rgbImage[row][col].r = y;
					rgbImage[row][col].b = x;
				}
			} else {
				// BGB                  RGR
				// GrG (col odd )   or  GbG (col even)
				// BGB                  RGR
				x = (cfaImage[row - 1][col] + cfaImage[row][col + 1] + cfaImage[row + 1][col] + cfaImage[row][col - 1])
						/ 4;
				rgbImage[row][col].g = x;
				y = (cfaImage[row - 1][col - 1] + cfaImage[row - 1][col + 1] + cfaImage[row + 1][col + 1]
						+ cfaImage[row + 1][col - 1]) / 4;
				if (row % 2 != 0) {
					rgbImage[row][col].b = y;
					rgbImage[row][col].r = cfaImage[row][col];
				} else {
					rgbImage[row][col].r = y;
					rgbImage[row][col].b = cfaImage[row][col];
				}
			}

		}
	}
}

void _ColourCorrection(PIX **rgbImage, int width, int height, float _ColorCorrectionMatrix[][3]) {
	int row, col;
	int r, g, b;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			r = rgbImage[row][col].r;
			g = rgbImage[row][col].g;
			b = rgbImage[row][col].b;
			r = _ColorCorrectionMatrix[0][0] * r + +_ColorCorrectionMatrix[0][1] * g + _ColorCorrectionMatrix[0][2] * b;
			g = _ColorCorrectionMatrix[1][0] * r + +_ColorCorrectionMatrix[1][1] * g + _ColorCorrectionMatrix[1][2] * b;
			b = _ColorCorrectionMatrix[2][0] * r + +_ColorCorrectionMatrix[2][1] * g + _ColorCorrectionMatrix[2][2] * b;
			// overflow
			if (r > 255)
				r = 255;
			if (r < 0)
				r = 0;
			if (g > 255)
				g = 255;
			if (g < 0)
				g = 0;
			if (b > 255)
				b = 255;
			if (b < 0)
				b = 0;
			// save
			rgbImage[row][col].r = r;
			rgbImage[row][col].g = g;
			rgbImage[row][col].b = b;
		}
	}
}

#include <cmath>

void _GammaCorrection(PIX **rgbImage, int width, int height, float gamma) {
	unsigned char LUT[256];
	for (int i = 0; i < 256; ++i) {
		LUT[i] = pow(i / 255.0, gamma) * 255.0 +0.5f; // or 1/gamma ??
	}
	int row, col;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			rgbImage[row][col].r = LUT[rgbImage[row][col].r];
			rgbImage[row][col].g = LUT[rgbImage[row][col].g];
			rgbImage[row][col].b = LUT[rgbImage[row][col].b];
		}
	}
}

