#pragma once
#include "bmpfunc.h"

class SeamCarver {
public:
	SeamCarver(BMP bmp);
	double energy(int x, int y);
	vector<int> findHorizontalSeam();
	vector<int> findVerticalSeam;
	void removeHorizontalSeam(vector<int>);
	void removeVerticalSeam(vector<int>);
	BMP image;
};
