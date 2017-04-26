#include "ClustersDetector.h"

template <class OutputIterator>
void alpha_edges(const Alpha_shape_2&  A,
	OutputIterator out)
{
	for (Alpha_shape_edges_iterator it = A.alpha_shape_edges_begin();
		it != A.alpha_shape_edges_end();
		++it) {
		*out++ = A.segment(*it);
	}
}

vector<point> SimplfyBorder(Polygon_2 & polygon)
{
	Polygon_2 result;
	std::cin >> polygon;
	Cost cost;
	result = PS::simplify(polygon, cost, Stop(0.5));

	std::cout.precision(12);
	std::cout << polygon << std::endl;
	return vector<point>();
	
}





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

void ClustersDetector::DiffImages(int impatience)
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



			if (abs(color.rgbBlue - color2.rgbBlue) < impatience &&  abs(color.rgbGreen - color2.rgbGreen) < impatience &&  abs(color.rgbRed - color2.rgbRed) < impatience)
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
	diff_result.Save(FIF_PNG, "C:\\Users\\laptop\\PycharmProjects\\untitled\\diff_result_"+to_string(impatience)+".png");
}

void ClustersDetector::ShrinkClusters(int iterations_count)
{
	for (int t = 0; t < iterations_count; t++)
	{
		for (int ci = 1; ci < cluster_map.cluster_count; ci++)
		{
			for (int i = 0; i < cluster_map.width; i++)
			{
				for (int j = 0; j < cluster_map.height; j++)
				{
					if (cluster_map.map[i][j] == ci)
					{
						bool on_edge = false;
						for (int pi = i - 1; pi <= i + 1 && !on_edge; pi++)
						{
							for (int pj = j - 1; pj <= j + 1; pj++)
							{
								if ((pi >= 0 && pi < cluster_map.width) && (pj >= 0 && pj < cluster_map.height) && (pi != i || pj != j))
								{
									if (cluster_map.map[pi][pj] == -1)
									{
										cluster_map.map[i][j] = -2;
										on_edge = true;
										break;

									}
								}
							}

						}
					}
				}
			}
		}
		for (int i = 0; i < cluster_map.width; i++)
		{
			for (int j = 0; j < cluster_map.height; j++)
			{
				if (cluster_map.map[i][j] == -2)
				{
					cluster_map.map[i][j] = -1;
				}
			}
		}
	}
	RGBQUAD color;
	color.rgbBlue = 255;
	color.rgbGreen = 255;
	color.rgbRed = 255;
	FIBITMAP * bm = diff_result.GetBitMap();
	for (int i = 0; i < cluster_map.width; i++)
	{
		for (int j = 0; j < cluster_map.height; j++)
		{
			if (cluster_map.map[i][j] == -1)
			{
				FreeImage_SetPixelColor(bm, i, j, &color);
			}
		}
	}
	diff_result.Save(FIF_PNG, "C:\\Users\\laptop\\PycharmProjects\\untitled\\shrinked.png");
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
					cluster_map.map[i][j] = cluster_map.cluster_count;
					while (neighbour_pixels.size())
					{
						for (auto & p : neighbour_pixels)
						{
							for (int pi = p.x - 1; pi <= p.x + 1; pi++)
							{
								for (int pj = p.y - 1; pj <= p.y + 1; pj++)
								{
									//if ((pi >= 0 && pi < width) && (pj >= 0 && pj < height))
									//if (pi != p.x || pj != p.y)
									if ((pi >= 0 && pi < width) && (pj >= 0 && pj < height) && (pi != p.x || pj != p.y))
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
}

void ClustersDetector::RemoveSmallClusters()
{

	for (int i = 0; i < cluster_map.width; i++)
	{
		for (int j = 0; j < cluster_map.height; j++)
		{
			if (cluster_map.map[i][j] == 0)
			{

			}
		}
	}
}

void ClustersDetector::GenerateClusters()
{
	for (int i = 1; i < cluster_map.cluster_count;i++)
	{
		Cluster c = Cluster(i, cluster_map);
		if (c.GetPointsCount() >= 100)
		{
			clusters.push_back(c);
		}

	}
}

void ClustersDetector::_ClustersImageToFile()
{
	FIBITMAP * bm;
	bm = FreeImage_Allocate(cluster_map.width, cluster_map.height,32);
	WrappedImage img = WrappedImage(bm);
	RGBQUAD color;
	color.rgbBlue = 0;
	color.rgbGreen = 0;
	color.rgbRed = 0;
	color.rgbReserved = 255;
	for (auto & c:clusters)
	{
		for (auto & p : c.GetSBorderPolygon()->container())
		{
			FreeImage_SetPixelColor(bm, p.x(), p.y(), &color);

		}
	}
}

void ClustersDetector::WholeBorder()
{
	int w = diff_result.GetWidth();
	int h = diff_result.GetHeight();
	std::list<Point> points;
	RGBQUAD color;
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			FreeImage_GetPixelColor(diff_result.GetBitMap(), i, j, &color);
			//if (map->map[i][j] == id)
			if (color.rgbBlue == 0 && color.rgbGreen == 0 && color.rgbRed == 0)
			{
				points.push_back(Point(i, j));
			}
		}
	}
	Alpha_shape_2 A(points.begin(), points.end(),
		FT(20),
		Alpha_shape_2::Mode::REGULARIZED);
	std::vector<Segment> segments;
	alpha_edges(A, std::back_inserter(segments));

	FIBITMAP * bm = FreeImage_Allocate(w, h, 32);
	//WrappedImage img=WrappedImage()
	color.rgbBlue = 0;
	color.rgbGreen = 0;
	color.rgbRed = 0;
	for (auto & s : segments)
	{
		FreeImage_SetPixelColor(bm, s.start().x(), s.start().y(), &color);
		FreeImage_SetPixelColor(bm, s.end().x(), s.end().y(), &color);
	}
	WrappedImage img = WrappedImage(bm);
	return;
	vector<Polygon_2> polygons;
	vector<Segment*> poly_segments;
	while (segments.size())
	{
		Segment * start = &segments[0];
		Segment * s1 = start;
		Segment * s2 = nullptr;
		poly_segments.push_back(start);
		Polygon_2 poly;
		do
		{
			for (auto & seg : segments)
			{
				if (s1->target() == seg.source())
				{
					s1 = &seg;
					poly_segments.push_back(s1);
					break;
				}
			}
		} while (start->source() != s1->target());

		for (auto sp : poly_segments)
		{
			poly.push_back(Point(sp->start().x(), sp->start().y()));
		}
		for_each(poly_segments.begin(), poly_segments.end(), [&segments](Segment * seg) {segments.erase(std::remove(segments.begin(), segments.end(), *seg), segments.end()); });

	}

}

void ClustersDetector::CalculateClusterAverageColor(int cluster_id)
{
	int width = cluster_map.width;
	int height = cluster_map.height;
	auto bm = with_added.GetBitMap();
	auto bm2 = diff_result.GetBitMap();
	RGBQUAD color;
	unsigned pixel_count = 0;
	unsigned ar=0;
	unsigned ag = 0;
	unsigned ab = 0;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (cluster_map.map[i][j] == cluster_id)
			{
				FreeImage_GetPixelColor(bm, i, j, &color);
				ar += color.rgbRed;
				ag += color.rgbGreen;
				ab += color.rgbBlue;
				pixel_count++;
			}
		}
	}
	ar /= pixel_count;
	ag /= pixel_count;
	ab /= pixel_count;

	color.rgbBlue = ab;
	color.rgbGreen = ag;
	color.rgbRed = ar;
	color.rgbReserved = 255;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (cluster_map.map[i][j] == cluster_id)
			{
				FreeImage_SetPixelColor(bm2, i, j, &color);
			}
		}
	}


}

void ClustersDetector::CalculateAllClustersAverageColor()
{
	for (int i = 1; i < cluster_map.cluster_count; i++)
	{
		CalculateClusterAverageColor(i);
	}
	diff_result.Save(FIF_PNG, "C:\\Users\\laptop\\PycharmProjects\\untitled\\diffed_with_av_color.png");
}

void ClustersDetector::test()
{
	int width = cluster_map.width;
	int height = cluster_map.height;
	auto bm = with_added.GetBitMap();
	auto bm2 = diff_result.GetBitMap();
	RGBQUAD color;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (cluster_map.map[i][j] > 1)
			{
				FreeImage_GetPixelColor(bm, i, j, &color);
				FreeImage_SetPixelColor(bm2, i, j, &color);
			}
		}
	}
	
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

Cluster::Cluster(int id,ClusterMap & map)
{

	if (id >= map.cluster_count)
	{
		throw(runtime_error("invalid cluster id"));
	}
	this->map = &map;
	this->id = id;
	minx = map.width;
	miny = map.height;
	maxx =0;
	maxy = 0;
	point_count = 0;
	for (int i = 0; i < map.width; i++)
	{
		for (int j = 0; j < map.height; j++)
		{
			if (map.map[i][j] ==id)
			{
				minx = i < minx ? i:minx;
				miny = j < miny ? j : miny;
				maxx = i > maxx ? i : maxx;
				maxy = j > maxy ? j : maxy;
				point_count++;

			}
		}
	}

	center_x = minx+(maxx - minx) / 2;
	center_y = miny+(maxy - miny) / 2;
	maxx = maxx;
	if (point_count >= MIN_CLUSTER_POINT_COUNT)
	{
		CalculateBounds();
		SimplfyBorder();
	}

}

void Cluster::CalculateBounds()
{
	int w = map->width;
	int h = map->height;
	std::list<Point> points;
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			if (map->map[i][j] == id)
			{
				points.push_back(Point(i, j));
			}
		}
	}
	Alpha_shape_2 A(points.begin(), points.end(),FT(20),Alpha_shape_2::GENERAL);
	std::vector<Segment> segments;
	alpha_edges(A, std::back_inserter(segments));

	Segment * start = &segments[0];
	Segment * s1 = start;
	border_polygon.push_back(Point(start->start().x(), start->start().y()));
	do
	{
		for (auto & seg : segments)
		{
			if (s1->target() == seg.source())
			{
				s1 = &seg;
				border_polygon.push_back(Point(s1->start().x(), s1->start().y()));
				break;
			}
		}
	} while (start->source() != s1->target());
}

Polygon_2 Cluster::SimplfyBorder()
{
	Cost cost;
	simplified_border_polygon = PS::simplify(border_polygon, cost, Stop(0.5));

	return simplified_border_polygon;
}
