#include "bmp.h"

int main() {
	BMP bmp;
	char path[256];
	do {
		cout << endl << "  >> Input file path: ";
		cin >> path;
	} while (!bmp.openImage(path));

	bmp.readBmp(path);
	cout << "  >> complete loading image" << endl;

  cout << endl << "  >> Input new file path: ";
  cin >> path;
	bmp.writeBmp(path);
}
