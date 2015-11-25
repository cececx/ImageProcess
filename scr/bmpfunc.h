#ifndef BMPFUNC_H
#define BMPFUNC_H

#include "bmp.h"


BMP::BMP(BMP& bitmap) {
	width = bitmap.getWidth();
	height = bitmap.getHeight();
	pixels = bitmap.getPixel();
}

BMP::BMP(LONG w, LONG h, vector<BGR> p) {
	width = w;
	height = h;
	pixels = p;
}

BMP::BMP(LONG w, LONG h, vector<BYTE> r, vector<BYTE> g, vector<BYTE> b) {
	width = w;
	height = h;
	int n = w * h;
	if (r.size() < n) n = r.size();
	if (g.size() < n) n = g.size();
	if (b.size() < n) n = b.size();
	for (int i = 0; i < n; ++i) {
		BGR p(b[i], g[i], r[i]);
		pixels.push_back(p);
	}
	for (int i = n; i < w * h; ++i) {
		BGR p(0,0,0);
		pixels.push_back(p);
	}
}

int BMP::openImage(char* path) {
	ifstream file(path, ios::binary);
	if (!file.is_open())
		return error(BMP_ERROR_OPEN_FILE);

	WORD tmp;
	file.read((char*)&tmp, sizeof(WORD));
	if (tmp != 0x4D42)
		return error(BMP_ERROR_FORMAT);

	file.seekg(28, ios::beg);
	file.read((char*)&tmp, sizeof(WORD));
	if (tmp != 24)
		return error(BMP_ERROR_FORMAT);

	file.close();
	return 1;
}

void BMP::readBmp(char* path) {
	BITMAPINFOHEADER info_head;
	ifstream file(path, ios::binary);

	// read header
	file.seekg(18, ios::beg);
	file.read((char*)&width, sizeof(LONG));
	file.read((char*)&height, sizeof(LONG));

	// read data
	LONG row = 4 * ((width * 24 + 31) / 32);
	BYTE padding = (BYTE)(row - width * 3);

	file.seekg(54, ios::beg);

	if (!pixels.empty())
		pixels.clear();
	BGR tmp;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			file.read((char*)&tmp, sizeof(BGR));
			pixels.push_back(tmp);
		}
		file.seekg(padding, ios::cur);
	}
	file.close();
}

void BMP::writeBmp(char* path) {
	LONG row = 4 * ((width * 24 + 31) / 32);

	// write header

	BITMAPFILEHEADER file_head;
	BITMAPINFOHEADER info_head;

	info_head.biWidth = width;
	info_head.biHeight = height;
	info_head.biSizeImage = row * height;
	file_head.bfSize = 54 + info_head.biSizeImage;

	ofstream file(path, ios::binary | ios::trunc);
	// file.clear();
	file.write((char*) "BM", 2);
	file.write((char*)&file_head, sizeof(BITMAPFILEHEADER));
	file.write((char*)&info_head, sizeof(BITMAPINFOHEADER));

	// write data
	BYTE padding_amount = (BYTE)(row - width * 3);
	BYTE * padding = new BYTE[padding_amount];
	// for (int i = height - 1; i >= 0; --i) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			file.write((char*)&pixels[i*width + j], sizeof(BGR));
		file.write((char*)padding, padding_amount);
	}
	delete[] padding;

	file.close();
}

vector<BYTE> BMP::getChannel(int index) {
	vector<BYTE> channel;
	int n = width * height;
	for (int i = 0; i < n; ++i) {
		channel.push_back(pixels[i][index]);
	}
	return channel;
}


void BMP::printBmpInfo(BITMAPFILEHEADER file_head, BITMAPINFOHEADER info_head) {
	cout << endl
		<< "       bfSize:      " << file_head.bfSize << endl
		<< "       bfReserved1: " << file_head.bfReserved1 << endl
		<< "       bfReserved2: " << file_head.bfReserved2 << endl
		<< "       bfOffBits:   " << file_head.bfOffBits << endl << endl;

	cout << "       biSize:          " << info_head.biSize << endl
		<< "       biWidth:         " << info_head.biWidth << endl
		<< "       biHeight:        " << info_head.biHeight << endl
		<< "       biPlanes:        " << info_head.biPlanes << endl
		<< "       biBitCount:      " << info_head.biBitCount << endl
		<< "       biCompression:   " << info_head.biCompression << endl
		<< "       biSizeImage:     " << info_head.biSizeImage << endl
		<< "       biXPelsPerMeter: " << info_head.biXPelsPerMeter << endl
		<< "       biYPelsPerMeter: " << info_head.biYPelsPerMeter << endl
		<< "       biClrUsed:       " << info_head.biClrUsed << endl
		<< "       biClrImportant:  " << info_head.biClrImportant << endl << endl;
}

int BMP::error(int opt) {
	switch (opt) {
	case 0: cout << "  >> Cannot find file" << endl; break;
	case 1: cout << "  >> Wrong Format" << endl; break;
	default: break;
	}
	return 0;
}



#endif
