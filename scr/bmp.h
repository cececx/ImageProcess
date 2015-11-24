#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

enum BmpError {
	BMP_ERROR_OPEN_FILE = 0,
	BMP_ERROR_FORMAT = 1
};

typedef struct tagBITMAPFILEHEADER {
	DWORD	bfSize;				// file size
	WORD	bfReserved1 = 0;
	WORD	bfReserved2 = 0;
	DWORD	bfOffBits = 54;		// data offset bits
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD 	biSize = 40;		// info header size
	LONG 	biWidth;			// width
	LONG 	biHeight;			// height
	WORD 	biPlanes = 1;		// (1)
	WORD	biBitCount = 24;	// Bits per pixel (1,4,8,16,24,32)
	DWORD 	biCompression = 0;	// compression type
	DWORD 	biSizeImage;		// image data size
	LONG 	biXPelsPerMeter = 0;
	LONG 	biYPelsPerMeter = 0;
	DWORD 	biClrUsed = 0;
	DWORD 	biClrImportant = 0;
} BITMAPINFOHEADER;

typedef struct tagBGR {
	BYTE b;
	BYTE g;
	BYTE r;
} BGR;

class BMP {
public:
	BMP () {}
	BMP(BMP& bitmap);
	BMP(LONG w, LONG h, vector<BGR> p);
	
	int openImage (char* path);
	void readBmp (char* path);
	void writeBmp (char* path);

	LONG getWidth() { return width; }
	LONG getHeight() { return height; }
	vector<BGR> getPixel() { return pixels; }

	int error(int opt);
	void printBmpInfo(BITMAPFILEHEADER, BITMAPINFOHEADER);

protected:
	LONG width;
	LONG height;
	vector<BGR> pixels;
};

#endif
