#pragma once
#include "x32\FreeImage.h"
#include "WrappedImage.h"

#include <string>
#include <vector>
using namespace std;

struct ClusterMap
{
	int width;
	int height;
	int ** map;
	int cluster_count = 0;
	WrappedImage GetImage();
};

class ClustersDetector
{
	WrappedImage original;
	WrappedImage with_added;
	WrappedImage diff_result;
	ClusterMap cluster_map;
public:
	ClustersDetector();
	~ClustersDetector();

	ClustersDetector(ClustersDetector && other);
	void LoadImages(string fn1, string fn2, float l, float t, float r, float b);
	void DiffImages();
	void GenerateClusterMap();
};




