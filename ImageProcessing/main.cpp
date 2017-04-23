#include "x32\FreeImage.h"
#include <math.h>
#include <vector>
#include "ClustersDetector.h"

using namespace std;



FIBITMAP * CutImage(FIBITMAP * bm, int l, int t, int r, int b)
{
	return FreeImage_Copy(bm, l, t, r, b);
}




FIBITMAP * GetDiffedImage()
{
	char * fn1 = "C:\\Users\\laptop\\PycharmProjects\\untitled\\WoWScrnShot_042317_111312.tga";
	FREE_IMAGE_FORMAT format1 = FreeImage_GetFileType(fn1, 0);
	FIBITMAP *fullsize = FreeImage_Load(format1, fn1);

	unsigned fs_width = FreeImage_GetWidth(fullsize);
	unsigned fs_height = FreeImage_GetHeight(fullsize);
	int bpp = FreeImage_GetBPP(fullsize);



	char * fn2 = "C:\\Users\\laptop\\PycharmProjects\\untitled\\WoWScrnShot_042317_111317.tga";
	FREE_IMAGE_FORMAT format2 = FreeImage_GetFileType(fn2, 0);
	FIBITMAP *fullsize2 = FreeImage_Load(format2, fn2);
	//FIBITMAP *bmp2 = FreeImage_Load(FIF_PNG, "C:\\Users\\laptop\\PycharmProjects\\untitled\\cpp2.png");

	unsigned fs_width2 = FreeImage_GetWidth(fullsize2);
	unsigned fs_height2 = FreeImage_GetHeight(fullsize2);
	int bpp2 = FreeImage_GetBPP(fullsize2);


	if (fs_height != fs_height2 || fs_width != fs_width2 || bpp != bpp2)
	{

		return 0;
	}


	int l = 0.00845947210*fs_width;
	int r = 0.371162564*fs_width;
	int t = 0.779490888*fs_height;
	int b = 0.322991401*fs_height;

	FIBITMAP * bm1 = FreeImage_Copy(fullsize, l, fs_height - t, r, fs_height - b);
	FIBITMAP * bm2 = FreeImage_Copy(fullsize2, l, fs_height - t, r, fs_height - b);


	FreeImage_Unload(fullsize);
	FreeImage_Unload(fullsize2);

	int width1 = FreeImage_GetWidth(bm1);
	int height1 = FreeImage_GetHeight(bm1);

	int width2 = FreeImage_GetWidth(bm2);
	int height2 = FreeImage_GetHeight(bm2);
	if (height1 != height2 || width1 != width2)
	{

		return 0;
	}


	FIBITMAP * result = FreeImage_Allocate(width1, height1, bpp);
	RGBQUAD color, color2, res_color;


	for (int i = 0; i < width1; i++)
	{
		for (int j = 0; j < height1; j++)
		{
			FreeImage_GetPixelColor(bm1, i, j, &color);
			FreeImage_GetPixelColor(bm2, i, j, &color2);
			int tolerance = 10;



			if (abs(color.rgbBlue - color2.rgbBlue) < tolerance &&  abs(color.rgbGreen - color2.rgbGreen) < tolerance &&  abs(color.rgbRed - color2.rgbRed) < tolerance)
				//if (color.rgbBlue != color2.rgbBlue && color.rgbGreen != color2.rgbGreen && color.rgbRed != color2.rgbRed && color.rgbReserved == color2.rgbReserved)
			{

				res_color.rgbBlue = 255;
				res_color.rgbGreen = 255;
				res_color.rgbRed = 255;
				res_color.rgbReserved = 255;
			}
			else
			{

				res_color.rgbBlue = 0;
				res_color.rgbGreen = 0;
				res_color.rgbRed = 0;
				//res_color.rgbReserved = 0;
			}

			FreeImage_SetPixelColor(result, i, j, &res_color);
		}
	}

	FreeImage_Save(FIF_PNG, bm1, "C:\\Users\\laptop\\PycharmProjects\\untitled\\cpp1.png", PNG_Z_NO_COMPRESSION);
	FreeImage_Save(FIF_PNG, bm2, "C:\\Users\\laptop\\PycharmProjects\\untitled\\cpp2.png", PNG_Z_NO_COMPRESSION);

	FreeImage_Unload(bm1);
	FreeImage_Unload(bm2);
	return result;
}




void SearchClusters(FIBITMAP * bm)
{
	int width = FreeImage_GetWidth(bm);
	int height = FreeImage_GetHeight(bm);
	int bpp = FreeImage_GetBPP(bm);
	int ** clustermap = new int *[width];
	for (int i = 0; i < width; i++)
	{
		clustermap[i] = new int[height];
		memset(clustermap[i], 0, height * 4);

	}
	struct pixel
	{
		pixel(int x, int y)
		{
			this->x = x;
			this->y = y;
		}
		int x;
		int y;
	};
	vector<pixel> neighbour_pixels;
	vector<pixel> next_neighbour_pixels;
	int cluster_id = 1;
	RGBQUAD color;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (clustermap[i][j] == 0)
			{
				FreeImage_GetPixelColor(bm, i, j, &color);
				if (color.rgbBlue == 0 && color.rgbGreen == 0 && color.rgbRed == 0)
				{
					neighbour_pixels.push_back(pixel(i, j));
					while (neighbour_pixels.size())
					{
						for (auto & p : neighbour_pixels)
						{
							for (int pi = p.x - 1; pi <= p.x + 1; pi++)
							{
								for (int pj = p.y - 1; pj <= p.y + 1; pj++)
								{
									//if ((pi >= 0 && pi < width) && (pj >= 0 && pj < height))
									if (pi != p.x || pj != p.y)
									{
										if (clustermap[pi][pj] == 0)
										{
											FreeImage_GetPixelColor(bm, pi, pj, &color);
											if (color.rgbBlue == 0 && color.rgbGreen == 0 && color.rgbRed == 0)
											{
												next_neighbour_pixels.push_back(pixel(pi, pj));
												clustermap[pi][pj] = cluster_id;
											}
											else
											{
												clustermap[pi][pj] = -1;
											}
										}

									}
								}
							}
						}
						neighbour_pixels.clear();
						neighbour_pixels = move(next_neighbour_pixels);
						next_neighbour_pixels.clear();
					}
					cluster_id++;
				}
				else
				{
					clustermap[i][j] = -1;
				}
			}
		}
	}
	FIBITMAP *  map = FreeImage_Allocate(width, height, bpp);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (clustermap[i][j] > 0)
			{

				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				if (clustermap[i][j] == 1)
					color.rgbBlue = 1;
				if (clustermap[i][j] == 2)
					color.rgbGreen = 1;
				if (clustermap[i][j] == 3)
					color.rgbRed = 1;

			}
			else
			{

				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
			}
			FreeImage_SetPixelColor(map, i, j, &color);
		}
	}
	FreeImage_Save(FIF_PNG, map, "C:\\Users\\laptop\\PycharmProjects\\untitled\\map.png", PNG_Z_NO_COMPRESSION);
}






int main()
{
	float l = 0.00845947210;
	float r = 0.371162564;
	float t = 0.779490888;
	float b = 0.322991401;
	string fn = "C:\\Users\\laptop\\PycharmProjects\\untitled\\WoWScrnShot_042317_111312.tga";
	string fn2 = "C:\\Users\\laptop\\PycharmProjects\\untitled\\WoWScrnShot_042317_111317.tga";

	ClustersDetector d = ClustersDetector();
	d.LoadImages(fn, fn2, l, t, r, b);
	d.DiffImages();
	d.GenerateClusterMap();




	//FIBITMAP * result = GetDiffedImage();
	//SearchClusters(result);
	//FreeImage_Save(FIF_PNG, result, "C:\\Users\\laptop\\PycharmProjects\\untitled\\cpp_result.png", PNG_Z_NO_COMPRESSION);
	return 0;
}