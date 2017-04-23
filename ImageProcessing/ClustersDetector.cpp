#include "ClustersDetector.h"






ClustersDetector::ClustersDetector()
{
}


ClustersDetector::~ClustersDetector()
{
}

ClustersDetector::ClustersDetector(ClustersDetector && other)
{
	this->original = move(other.original);
	this->with_added = move(other.with_added);
}

void ClustersDetector::LoadImages(string fn1, string fn2, float l, float t, float r, float b)
{
	WrappedImage img1 = WrappedImage(fn1);
	WrappedImage img2 = WrappedImage(fn2);



	


	if (img1.GetHeight() != img2.GetHeight() && img1.GetWidth() != img2.GetWidth())
	{
		throw(runtime_error("Fullsize images has not sae size"));
	}
	int fs_height = img1.GetHeight();
	int fs_width = img1.GetWidth();
	int left = l*fs_width;
	int right = r*fs_width;
	int top = t*fs_height;
	int bottom = b*fs_height;

	original = img1.Cut(left, fs_height-top, right, fs_height - bottom);
	with_added = img2.Cut(left, fs_height - top, right, fs_height - bottom);

}

void ClustersDetector::DiffImages()
{
	int width = original.GetWidth();
	int height = original.GetHeight();
	FIBITMAP * bm1,* bm2, *result;
	bm1 = original.GetBitMap();
	bm2 = with_added.GetBitMap();
	RGBQUAD color, color2, res_color;
	result = FreeImage_Allocate(width, height, original.GetPixelSize());


	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
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
	diff_result = WrappedImage(result);
}

void ClustersDetector::GenerateClusterMap()
{
	FIBITMAP *bm = diff_result.GetBitMap();
	int width = diff_result.GetWidth();
	int height = diff_result.GetHeight();
	//int bpp = FreeImage_GetBPP(bm);
	//int ** clustermap = new int *[width];
	cluster_map.map = new int*[width];
	cluster_map.width = width;
	cluster_map.height = height;
	for (int i = 0; i < width; i++)
	{
		cluster_map.map[i] = new int[height];
		memset(cluster_map.map[i], 0, height * 4);

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
	cluster_map.cluster_count= 1;
	RGBQUAD color;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (cluster_map.map[i][j] == 0)
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
										if (cluster_map.map[pi][pj] == 0)
										{
											FreeImage_GetPixelColor(bm, pi, pj, &color);
											if (color.rgbBlue == 0 && color.rgbGreen == 0 && color.rgbRed == 0)
											{
												next_neighbour_pixels.push_back(pixel(pi, pj));
												cluster_map.map[pi][pj] = cluster_map.cluster_count;
											}
											else
											{
												cluster_map.map[pi][pj] = -1;
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
					cluster_map.cluster_count++;
				}
				else
				{
					cluster_map.map[i][j] = -1;
				}
			}
		}
	}
	//cluster_map.GetImage().Save(FIF_PNG, "C:\\Users\\laptop\\PycharmProjects\\untitled\\test.png", PNG_Z_NO_COMPRESSION);
}

WrappedImage ClusterMap::GetImage()
{
	FIBITMAP *  bm = FreeImage_Allocate(width, height, 32);
	RGBQUAD color;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (map[i][j] > 0)
			{

				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				if (map[i][j] == 1)
					color.rgbBlue = 1;
				if (map[i][j] == 2)
					color.rgbGreen = 1;
				if (map[i][j] == 3)
					color.rgbRed = 1;

			}
			else
			{

				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
				color.rgbReserved = 255;
			}
			FreeImage_SetPixelColor(bm, i, j, &color);
		}
	}
	return WrappedImage(bm);
}
