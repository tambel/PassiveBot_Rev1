#include "x32\FreeImage.h"
#include <math.h>
#include <vector>
#include "ClustersDetector.h"
using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/algorithm.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Alpha_shape_2.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
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
template <class OutputIterator>
void
alpha_edges(const Alpha_shape_2&  A,
	OutputIterator out)
{
	for (Alpha_shape_edges_iterator it = A.alpha_shape_edges_begin();
		it != A.alpha_shape_edges_end();
		++it) {
		*out++ = A.segment(*it);
	}
}
template <class OutputIterator>
bool
file_input(OutputIterator out)
{
	std::ifstream is("D:\\Bot\\Projects\\PassiveBot\\Debug\\1.txt", std::ios::in);
	if (is.fail()) {
		std::cerr << "unable to open file for input" << std::endl;
		return false;
	}
	int n;
	is >> n;
	std::cout << "Reading " << n << " points from file" << std::endl;
	CGAL::cpp11::copy_n(std::istream_iterator<Point>(is), n, out);
	return true;
}
// Reads a list of points and returns a list of segments
// corresponding to the Alpha shape.

int mainn()
{

	std::list<Point> points;
	if (!file_input(std::back_inserter(points))) {
		return -1;
	}
	Alpha_shape_2 A(points.begin(), points.end(),
		FT(10000),
		Alpha_shape_2::GENERAL);
	std::vector<Segment> segments;
	alpha_edges(A, std::back_inserter(segments));
	std::cout << "Alpha Shape computed" << std::endl;
	std::cout << segments.size() << " alpha shape edges" << std::endl;
	std::cout << "Optimal alpha: " << *A.find_optimal_alpha(1) << std::endl;
	return 0;
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
	d.CalculateAllClustersAverageColor();
	d.GenerateClusters();
	d._ClustersToImage();

	return 0;
}