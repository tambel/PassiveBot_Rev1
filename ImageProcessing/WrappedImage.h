#pragma once
#include "x32\FreeImage.h"
#include <string>

using namespace std;
class WrappedImage
{
	int width;
	int height;
	char pixel_size;
	FIBITMAP * bitmap;
public:
	WrappedImage();
	WrappedImage(string filename);
	WrappedImage(FIBITMAP * bm);
	~WrappedImage();

	WrappedImage(WrappedImage && other);
	WrappedImage & operator= (WrappedImage && other);

	inline int GetHeight() { return height; }
	inline int GetWidth() { return width; }
	inline FIBITMAP * GetBitMap() { return bitmap; }
	inline char GetPixelSize() { return pixel_size; }
	void _init();
	void _move(WrappedImage & other);
	WrappedImage Cut(int l, int t, int r, int b);
	void Save(FREE_IMAGE_FORMAT format,string filename, int flags=0);
	

};

