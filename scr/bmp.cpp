#include "bmp.h"

BMPImage::BMPImage (void) {
	_image.reset();
}

BMPImage::BMPImage (std::string file_name) {
	_image.reset (new MyImage(file_name, 1, 0, 0));
}

BMPImage::BMPImage (std::string file_name, LONG width, LONG height) {
	_image.reset (new MyImage(file_name, 1, width, height));
}

BMPImage::~BMPImage (void) {
	_image->working_file.close();
	delete _image->file_header;
	delete _image->info_header;
}

BMPImage::BMPImage (const BMPImage &bmpimage) {
	*this = const_cast<BMPImage&> (bmpimage);
}

BMPImage::BMPImage (BMPImage &bmpimage) {
	*this = bmpimage;
}
/*
BMPImage &BMPImage::operator = (const BMPImage &bmpimage) {
	return *this = const_cast<BMPImage> (bmpimage);
}
*/

BMPImage &BMPImage::operator = (BMPImage &bmpimage) {
	if (&bmpimage != this) {
		_image.swap(bmpimage._image);
		_image->ref_count++;
	}
}

int BMPImage::OpenImage (void) {
	MyImageCheck();
	std::cout << "[+] Opening BMP file \"" << _image->file_name << "\"...";
	_image->working_file.open(_image->file_name.c_str(), std::fstream::in | std::fstream::binary);
	if (!_image->working_file.is_open())
		return BMP_ERR_OPENING_FILE;
	return BMP_OK;
}

int BMPImage::CreateImage (void) {
	MyImageCheck();
	std::cout << "[+] Creating BMP file \"" << _image->file_name  << "\"...";
	if (!_image->working_file.is_open())
		return BMP_ERR_CREATING_FILE;
	return BMP_OK;
}

int BMPImage::ReadImageHeader (void) {
	MyImageCheck();

	std::cout << "[+] Reading image headers from \"" << _image->file_name << "\"...";
	if (!_image->working_file.is_open())
		return BMP_ERR_READING_FILE;

	BYTE* _data[2];

	_image->file_header = new BITMAPFILEHEADER();
	_image->info_header = new BITMAPINFOHEADER();

	_data[0] = new BYTE[sizeof(BITMAPFILEHEADER)];
	_data[1] = new BYTE[sizeof(BITMAPINFOHEADER)];

	_image->working_file.read (reinterpret_cast<char*> (_data[0]), sizeof(BITMAPFILEHEADER));
	_image->working_file.read (reinterpret_cast<char*> (_data[1]), sizeof(BITMAPINFOHEADER));

	if (_image->file_header->bfType != 0x4D42 ||
		_image->info_header->biCompression != 24)
		return BMP_ERR_FILE_NOT_SUPPORTED;

	_image->width = _image->info_header->biWidth;
	_image->height = _image->info_header->biHeight;

	delete[] _data[0];
	delete[] _data[1];

	return BMP_OK;
}

int BMPImage::ReadImagePixels (void) {
	MyImageCheck();
	std::cout << "[+] Reading image pixels from \"" << _image->file_name << "\"...";
	if (!_image->working_file.is_open())
		return BMP_ERR_READING_FILE;

	LONG width = _image->width;
	LONG height = _image->height;
	LONG buffer = width * height;
	BYTE pad_offset = ( ((width*3+3) & (~3)) - (width*3) ) * sizeof(BYTE);
	RGB temp;

	// skip header
	_image->working_file.seekg (_image->file_header->bfOffBits);
	for (int i=0; i<height; ++i) {
		for (int j=0; j<width; ++j) {
			_image->working_file.read(reinterpret_cast<char*> (&temp), sizeof(RGB));
			_image->pixels.push_back(temp);
		}
		// skip data offset
		_image->working_file.seekg (pad_offset, std::ios::cur);
	}

	return BMP_OK;
}

int BMPImage::WriteImageHeader (void) {
	MyImageCheck();
	std::cout << "[+] Writing image header to \"" << _image->file_name << "\"...";
	if (!_image->working_file.is_open())
		return BMP_ERR_WRITING_FILE;

	_image->file_header = new BITMAPFILEHEADER();
	_image->info_header = new BITMAPINFOHEADER();

	LONG height = _image->height;
	LONG width = _image->width;
	LONG width_padded = (width*3+3) & (~3);
	DWORD img_size = width_padded * height;

	BYTE fileheader_data[] = {"\x42\x4D"           //signature
                              "\x00\x00\x00\x00"   //file size
                              "\x00\x00\x00\x00"   //reserved
                              "\x36\x00\x00\x00"}; //file_offset
	BYTE infoheader_data[] = {"\x28\x00\x00\x00"   //header size
                              "\x00\x00\x00\x00"   //width
                              "\x00\x00\x00\x00"   //height
                              "\x01\x00"           //planes
                              "\x18\x00"           //bits per pixel
                              "\x00\x00\x00\x00"   //compression
                              "\x00\x00\x00\x00"   //imgSize
                              "\x12\x0b\x00\x00"   //X Resolution
                              "\x12\x0b\x00\x00"   //Y Resolution
                              "\x00\x00\x00\x00"   //colours
                              "\x00\x00\x00\x00"}; //important Colours

	memcpy(_image->file_header, fileheader_data, sizeof(BITMAPFILEHEADER));
	memcpy(_image->info_header, infoheader_data, sizeof(BITMAPINFOHEADER));

	_image->info_header->biWidth = width;
	_image->info_header->biHeight = height;
	_image->info_header->biSizeImage = img_size;
	_image->file_header->bfSize = img_size + _image->file_header->bfOffBits;

	_image->working_file.seekg(0);
	_image->working_file.write(reinterpret_cast<const char*>(_image->file_header),
                                sizeof(BITMAPFILEHEADER));
	_image->working_file.write(reinterpret_cast<const char*>(_image->info_header),
                                sizeof(BITMAPINFOHEADER));

	return BMP_OK;
}

int BMPImage::WriteImagePixels (void) {
	MyImageCheck();
	std::cout << "[+] Writing image pixels to \"" << _image->file_name << "\"...";
	if (!_image->working_file.is_open())
		return BMP_ERR_WRITING_FILE;

	DWORD height        = _image->height;
	DWORD width         = _image->width;
	BYTE padding_amount = ((width * 3 + 3) & (~3)) - (width * 3);
	BYTE *padding       = new BYTE[padding_amount];

	_image->working_file.seekg(_image->file_header->bfOffBits);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			_image->working_file.write(reinterpret_cast<const char*>(&_image->pixels[(i * width) + j]), sizeof(RGB));
			_image->working_file.write(reinterpret_cast<const char*>(padding), padding_amount);
	}

	delete[] padding;

	return BMP_OK;
}

RGB& BMPImage::index (LONG x, LONG y) {
	MyImageCheck();
	// if ((x >= _image->width) || (y >= _image->height)) throw ERROR_OUT_OF_BOUNDS;
  
	return _image->pixels[(y * _image->height) + x];
}

DWORD BMPImage::size (void) {
	MyImageCheck();
	return (_image->width * _image->height);
}

std::string BMPImage::file_name (void) {
	MyImageCheck();
	return _image->file_name;
}

void BMPImage::set_file_name (std::string file_name) {
	MyImageCheck();
	_image->file_name = file_name;
}

LONG BMPImage::width (void) {
	MyImageCheck();
	return _image->width;
}

LONG BMPImage::height (void)  {
	MyImageCheck();
	return _image->height;
}

std::vector<RGB> BMPImage::pixels (void) {
	MyImageCheck();
	return _image->pixels;
}

void BMPImage::set_width (LONG width) {
	MyImageCheck();
	_image->width = width;
}


void BMPImage::set_height (LONG height) {
	MyImageCheck();
	_image->height = height;
}

void BMPImage::set_pixels (std::vector<RGB> pixels) {
	MyImageCheck();
	_image->pixels = pixels;
}
/*
std::shared_ptr<BMPImage::MyImage> BMPImage::MyImageData (RGBErrors error) {
	if (_image == NULL) throw error;
	return _image;
}

void BMPImage::MyImageCheck (void) {
	if (_image == NULL) throw ERROR_MYIMAGE_NOT_DEF;
}
*/
