#include "seamcarv.h"

#include <time.h>

int main() {
	BMP bmp;
	char path[256];
	do {
		cout << endl << "  >> Input file path: ";
		cin >> path;
	} while (!bmp.openImage(path));

	bmp.readBmp(path);
	cout << "  >> image loaded" << endl;
	
	SeamCarver seamcarver(bmp);
	cout << "  >> seamcarver created" << endl << endl;
	cout << "  >> processing..." << endl << endl;
	clock_t start = clock();
	for (int i = 0; i < 50; ++i) {
		vector<int> seam = seamcarver.findVerticalSeam();
		seamcarver.removeVerticalSeam(seam);
	}
	clock_t end = clock();
	cout << "  >> 50 seams: runtime " << (end - start) << "ms" << endl;
	seamcarver.output("d:\\resize.bmp");
}
