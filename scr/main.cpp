#include "bmpfunc.h"

int main() {
	
	BMP bmp;
	char path[256];
	do {
		cout << endl << "  >> Input file path: ";
		cin >> path;
	} while (!bmp.openImage(path));

	bmp.readBmp(path);
	cout << "  >> complete loading image" << endl;

	LONG w = bmp.getWidth();
	LONG h = bmp.getHeight();
	BMP bmpRedChannel(w, h, bmp.getChannel(0), bmp.getChannel(0), bmp.getChannel(0));

	bmpRedChannel.writeBmp("c:\\Users\\xic3\\Pictures\\test\\1_red.bmp");
}
