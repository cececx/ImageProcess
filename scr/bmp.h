#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
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

	BYTE& operator [] (const int color) {
		switch (color)
		{
		case 0: return r;
		case 1: return g;
		case 2: return b;
		}
		return r;
	}
	tagBGR () : b(0), g(0), r(0) {}
	tagBGR (BYTE red, BYTE green, BYTE blue) : b(blue), g(green), r(red) {}

} BGR;





class BMP {
public:
	BMP () {}
	BMP(BMP&);
	BMP(LONG, LONG, vector<BGR>);
	BMP(LONG, LONG, vector<BYTE>, vector<BYTE>, vector<BYTE>);
	int openImage (char*);
	void readBmp (char*);
	void writeBmp (char*);
	
	LONG getWidth() { return width; }
	LONG getHeight() { return height; }
	vector<BGR> getPixel() { return pixels; }

	vector<BYTE> getChannel(int);
	vector<BYTE> getChannelR();
	vector<BYTE> getChannelG();
	vector<BYTE> getChannelB();



	int error(int opt);
	void printBmpInfo(BITMAPFILEHEADER, BITMAPINFOHEADER);

protected:
	LONG width;
	LONG height;
	vector<BGR> pixels;
};



#endif
