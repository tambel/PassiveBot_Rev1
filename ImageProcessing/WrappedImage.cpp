#include "WrappedImage.h"


WrappedImage::WrappedImage()
{
	
}

WrappedImage::WrappedImage(string filename)
{
	FREE_IMAGE_FORMAT format1 = FreeImage_GetFileType(filename.c_str(), 0);
	bitmap = FreeImage_Load(format1, filename.c_str());
	_init();
	
}

WrappedImage::WrappedImage(FIBITMAP * bm)
{
	bitmap = bm;
	_init();
}

WrappedImage::~WrappedImage()
{
	FreeImage_Unload(bitmap);
}

WrappedImage::WrappedImage(WrappedImage && other)
{
	_move(other);
}

WrappedImage & WrappedImage::operator=(WrappedImage && other)
{
	_move(other);
	return *this;


}

void WrappedImage::_init()
{
	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);
	pixel_size = FreeImage_GetBPP(bitmap);
}

void WrappedImage::_move(WrappedImage & other)
{
	this->bitmap = other.bitmap;
	other.bitmap = nullptr;
	this->height = other.height;
	other.height = 0;
	this->width = other.width;
	other.width = 0;
	this->pixel_size = other.pixel_size;
	other.pixel_size = 0;
}

WrappedImage WrappedImage::Cut(int l, int t, int r, int b)
{
	FIBITMAP * cutted = FreeImage_Copy(bitmap, l, t, r, b);
	return WrappedImage(cutted);
}

void WrappedImage::Save(FREE_IMAGE_FORMAT format, string filename, int flags)
{
	FreeImage_Save(format, bitmap, filename.c_str(),flags);
}
