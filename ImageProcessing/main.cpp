#include "x32\FreeImage.h"
#include <math.h>
#include <vector>
#include "ClustersDetector.h"
using namespace std;
int main()
{
	float l = 0.00845947210;
	float r = 0.371162564;
	float t = 0.779490888;
	float b = 0.322991401;
	string fn = "C:\\Users\\laptop\\PycharmProjects\\untitled\\WoWScrnShot_042417_111737.tga";
	string fn2 = "C:\\Users\\laptop\\PycharmProjects\\untitled\\WoWScrnShot_042417_111739.tga";

	ClustersDetector d = ClustersDetector();
	d.LoadImages(fn, fn2, l, t, r, b);
	d.DiffImages(10);
	d.GenerateClusterMap();
	d.ShrinkClusters(2);
	d.GenerateClusters();
	return 0;
}