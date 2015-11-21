#ifndef BMP_H_
#define BMP_H_

#include <vector>
#include <fstream>
#include <algorithm>
#include <memory>

#include "common.h"

enum COLOR {
	RED,
	GREEN,
	BLUE
};

enum BMPERROR {
	BMP_OK = 0,
	BMP_ERR_OPENING_FILE = 1,
	BMP_ERR_CREATING_FILE = 2,
	BMP_ERR_READING_FILE = 3,
	BMP_ERR_WRITING_FILE = 4,
	BMP_ERR_FILE_NOT_SUPPORTED = 5,
	BMP_ERR_IMAGE_TOO_LARGE = 6,
	BMP_VALUE_NOT_SET = 7
};

typedef struct tagBITMAPFILEHEAADER {
	WORD 	bfType;				// file type "BM"
	DWORD 	bfSize;				// file size
	WORD 	bfReserved1;
	WORD 	bfReserved2;
	DWORD 	bfOffBits;			// data offset bits
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD 	biSize;				// info header size
	LONG 	biWidth;			// width
	LONG 	biHeight;			// height
	WORD 	biPlanes;			// (1)
	DWORD 	biCompression;		// compression type (1, 4, 8, 24)
	DWORD 	biSizeImage;		// image data size
	LONG 	biXPelsPerMeter;
	LONG 	beYPelsPerMeter;
	DWORD 	biClrUsed;
	DWORD 	biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE 	rgbBlue;
	BYTE 	rgbGreen;
	BYTE 	rgbRed;
	BYTE 	rgbReserved;
} RGBQUAD;

typedef struct tagRGB
{
	BYTE r;
	BYTE g;
	BYTE b;

	BYTE& operator [] (const COLOR& color) {
		switch (color) {
			case RED: return r;
			case GREEN: return g;
			case BLUE: return b;
		}
		// throw ERROR_OUT_OF_BOUNDS;
	}

	RGB ()
	 : r(0), g(0), b(0) {}
	RGB (const BYTE* rgb)
	 : r(rgb[0]), g(rgb[1]), b(rgb[2]) {}
	RGB (BYTE red, BYTE green, BYTE blue)
	 : r(red), g(green), b(blue) {}

} RGB;

class BMPImage {

public:
	BMPImage (void);
	BMPImage (std::string);
	BMPImage (std::string, LONG, LONG);

	~BMPImage (void);

	/* copy constructors */
	BMPImage (const BMPImage&);
	BMPImage (BMPImage&);

	/* assignment operators */
	BMPImage& operator = (const BMPImage&);
	BMPImage& operator = (BMPImage&);

	int OpenImage (void);
	int CreateImage (void);
	int ReadImageHeader (void);
	int ReadImagePixels (void);
	int WriteImageHeader (void);
	int WriteImagePixels (void);

	RGB& index (LONG, LONG);
	DWORD size (void);

	/* get and set */
	std::string file_name (void);
	LONG width (void);
	LONG height (void);
	std::vector<RGB> pixels (void);
	void set_file_name (std::string);
	void set_width (LONG);
	void set_height (LONG);
	void set_pixels (std::vector<RGB>);

private:
	struct MyImage {
		std::string 		file_name;
		std::fstream 		working_file;
		BITMAPFILEHEADER 	*file_header;
		BITMAPINFOHEADER 	*info_header;
		BYTE 				ref_count;
		LONG 				width;
		LONG 				height;
		std::vector<RGB> 	pixels;
		MyImage (WORD ref_count, LONG width, LONG height)
		 : ref_count(ref_count), width(width), height(height) {}
		MyImage (std::string file_name, WORD ref_count, LONG width, LONG height)
		 : file_name(file_name), ref_count(ref_count), width(width), height(height) {}
	};
	std::shared_ptr<MyImage> _image;
	// std::shared_ptr<MyImage> MyImageData (BMPErrors);
	void MyImageCheck (void);
};

#endif
