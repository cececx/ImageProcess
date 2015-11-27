#pragma once
#include "bmpfunc.h"

class SeamCarver {
public:
	SeamCarver(BMP bmp);
	int getEnergy(BGR** p, int x, int y);
	vector<int> findVerticalSeam();
	vector<int> findHorizontalSeam();
	void removeHorizontalSeam(vector<int> seam); 
	void removeVerticalSeam(vector<int> seam);
	void output(char* path);

private:
	BMP image;
	int width;
	int height;
	vector<int> energy;
	vector<BGR> pixels;
};

SeamCarver::SeamCarver(BMP bmp)
: image(bmp), width(bmp.getWidth()), height(bmp.getHeight()),
energy(width*height), pixels(bmp.getPixel()) {

	// extend border
	BGR** extend = new BGR*[height + 2];
	BGR tmp;
	for (int i = 0; i < height + 2; ++i)
		extend[i] = new BGR[width + 2];
	for (int j = 0; j < width + 2; ++j) {
		extend[0][j] = tmp;
		extend[height + 1][j] = tmp;
	}
	for (int i = 0; i < height; ++i) {
		extend[i + 1][0] = tmp;
		extend[i + 1][width + 1] = tmp;
		for (int j = 0; j < width; ++j) {
			extend[i + 1][j + 1] = pixels[i*width + j];
		}
	}

	// calc energy
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			energy[i*width + j] = getEnergy(extend, i + 1, j + 1);
		}
	}

	for (int i = 0; i < height + 2; ++i)
		delete[] extend[i];
	delete[] extend;
}

int SeamCarver::getEnergy(BGR** p, int raw, int col) {
	int e = 0;
	for (int k = 0; k < 3; ++k) {
		int ex = p[raw][col-1][k] - p[raw][col+1][k];
		int ey = p[raw+1][col][k] - p[raw-1][col][k];
		e += ex*ex + ey*ey;
	}
	return e;
}

vector<int> SeamCarver::findVerticalSeam() {

	vector<int> seam(height, 0);
	vector<int> distTo(width*height, INT_MAX);
	vector<int> edgeTo(width*height, -1);

	// raw = 0;
	for (int j = 0; j < width; ++j)
		distTo[j] = energy[j];

	for (int i = 1; i < height; ++i) {
		// col = 0;
		int cur = i*width;
		int pre = (i - 1)*width;
		if (distTo[pre] > distTo[pre + 1]) ++pre;
		distTo[cur] = distTo[pre] + energy[cur];
		edgeTo[cur] = pre;
		// col = 1 ~ w-1;
		for (int j = 1; j < width - 1; ++j) {
			++cur;
			int pre0 = (i - 1)*width + j - 1;
			pre = pre0;
			if (distTo[pre0 + 1] < distTo[pre]) pre = pre0 + 1;
			if (distTo[pre0 + 2] < distTo[pre]) pre = pre0 + 2;
			distTo[cur] = distTo[pre] + energy[cur];
			edgeTo[cur] = pre;
		}

		// col = w-1;
		++cur;
		pre = i*width - 2;
		if (distTo[pre] > distTo[pre + 1]) ++pre;
		distTo[cur] = distTo[pre] + energy[cur];
		edgeTo[cur] = pre;
	}


	// min distance
	int last = (height - 1)*width;
	int min_index = last;
	for (int j = 1; j < width; ++j) {
		if (distTo[last + j] < distTo[min_index])
			min_index = last + j;
	}

	for (int i = height - 1; i >= 0; --i) {
		seam[i] = min_index;
		last = min_index;
		min_index = edgeTo[last];
	}

	return seam;
}

void SeamCarver::removeVerticalSeam(vector<int> seam) {
	int start = 0;
	int i, j, k;
	for (i = 0, k = 0; i < height; ++i) {
		int end = seam[i];
		for (j = start; j < end; ++j) {
			energy[k] = energy[j];
			pixels[k] = pixels[j];
			++k;
		}
		start = end + 1;
	}
	int end = width * (height - 1);
	for (i = start; i < end; ++i) {
		energy[k] = energy[i];
		pixels[k] = pixels[i];
		++k;
	}
	--width;
}

void SeamCarver::output(char* path) {
	BMP bmp(width, height, pixels);
	bmp.writeBmp(path);
}
