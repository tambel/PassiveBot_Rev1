#pragma once
#include "x32\FreeImage.h"
#include "WrappedImage.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/algorithm.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyline_simplification_2/simplify.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>

namespace PS = CGAL::Polyline_simplification_2;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K>                   Polygon_2;
typedef PS::Stop_below_count_ratio_threshold Stop;
typedef PS::Squared_distance_cost            Cost;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef K::Point_2  Point;
typedef K::Segment_2  Segment;
typedef CGAL::Alpha_shape_vertex_base_2<K> Vb;
typedef CGAL::Alpha_shape_face_base_2<K>  Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation_2;
typedef CGAL::Alpha_shape_2<Triangulation_2>  Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator Alpha_shape_edges_iterator;
using namespace std;

#define MIN_CLUSTER_POINT_COUNT 100 

struct point
{
	point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};

using pixel = point;
struct ClusterMap
{
	int width;
	int height;
	int ** map;
	int cluster_count = 0;
	WrappedImage GetImage();
};

class Cluster
{
	ClusterMap * map;
	int id;
	int minx;
	int miny;
	int maxx;
	int maxy;
	int center_x;
	int center_y;
	int point_count;
	//vector<point> border_points;
	Polygon_2 border_polygon;
	Polygon_2 simplified_border_polygon;
public:
	Cluster(int id,ClusterMap & map);
	void CalculateBounds();
	Polygon_2 SimplfyBorder();
	//inline vector<point> & GetBorderPoints() { return border_points; }
	inline Polygon_2 * GetBorderPolygon() { return &border_polygon; }
	inline Polygon_2 * GetSBorderPolygon() { return &simplified_border_polygon; }
	inline int GetPointsCount() { return point_count; }

	//vector<pixel>
};

class ClustersDetector
{
	WrappedImage original;
	WrappedImage with_added;
	WrappedImage diff_result;
	ClusterMap cluster_map;
	vector<Cluster> clusters;
public:
	ClustersDetector();
	~ClustersDetector();

	ClustersDetector(ClustersDetector && other);

	ClusterMap * GetClusterMap() { return &cluster_map; }
	inline WrappedImage * GetDiffResult() { return &diff_result; }
	void LoadImages(string fn1, string fn2, float l, float t, float r, float b);
	void DiffImages(int impatience);
	void ShrinkClusters(int iterations_count);


	void GenerateClusterMap();
	void RemoveSmallClusters();
	void GenerateClusters();
	void _ClustersImageToFile();
	void WholeBorder();




	void CalculateClusterAverageColor(int cluster_id);
	void CalculateAllClustersAverageColor();

	void test();
};




