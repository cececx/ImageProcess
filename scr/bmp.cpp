#include "bmp.h"

BMP::BMP (BMP& bitmap) {
	width = bitmap.getWidth();
	height = bitmap.getHeight();
	pixels = bitmap.getPixel();
}

BMP::BMP(LONG w, LONG h, vector<BGR> p) {
	width = w;
	height = h;
	pixels = p;
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
	ofstream file(path, ios::binary | ios::trunc);
	
	LONG row = 4 * ((width * 24 + 31) / 32);
	
	// write header
	BITMAPFILEHEADER file_head;
	BITMAPINFOHEADER info_head;

	info_head.biWidth = width;
	info_head.biHeight = height;
	info_head.biSizeImage = row * height;
	file_head.bfSize = 54 + info_head.biSizeImage;

	file.write((char*) "BM", 2);
	file.write((char*)&file_head, sizeof(BITMAPFILEHEADER));
	file.write((char*)&info_head, sizeof(BITMAPINFOHEADER));

	// write data
	BYTE padding_amount = (BYTE)(row - width * 3);
	BYTE * padding = new BYTE[padding_amount];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			file.write((char*)&pixels[i*width + j], sizeof(BGR));
		file.write((char*)padding, padding_amount);
	}
	delete[] padding;

	file.close();
}

int BMP::error(int opt) {
	switch (opt) {
	case 0: cout << "  >> Cannot find file" << endl; break;
	case 1: cout << "  >> Wrong Format" << endl; break;
	default: break;
	}
	return 0;
}
