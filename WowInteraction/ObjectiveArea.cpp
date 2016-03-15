#include "stdafx.h"
#include "ObjectiveArea.h"


ObjectiveArea::ObjectiveArea()
{
}


ObjectiveArea::~ObjectiveArea()
{
	delete[] points;
	points = nullptr;
	points_count = 0;
}

bool ObjectiveArea::IsInArea(Point2D<float> point)
{
#define INF 10000
	class PIPSolver
	{
	public:
		struct Point
		{
			float x;
			float y;
		};
		static bool onSegment(Point2D<int> p, Point2D<int> q, Point2D<int> r)
		{
			if (q.X <= max(p.X, r.X) && q.X >= min(p.X, r.X) &&
				q.Y <= max(p.Y, r.Y) && q.Y >= min(p.Y, r.Y))
				return true;
			return false;
		}

		static int orientation(Point2D<int> p, Point2D<int> q, Point2D<int> r)
		{
			int val = (q.Y - p.Y) * (r.X - q.X) -
				(q.X - p.X) * (r.Y - q.Y);

			if (val == 0) return 0; // colinear
			return (val > 0) ? 1 : 2; // clock or counterclock wise
		}

		static bool doIntersect(Point2D<int> p1, Point2D<int> q1, Point2D<int> p2, Point2D<int> q2)
		{

			int o1 = orientation(p1, q1, p2);
			int o2 = orientation(p1, q1, q2);
			int o3 = orientation(p2, q2, p1);
			int o4 = orientation(p2, q2, q1);

			// General case
			if (o1 != o2 && o3 != o4)
				return true;

			
			if (o1 == 0 && onSegment(p1, p2, q1)) return true;

			
			if (o2 == 0 && onSegment(p1, q2, q1)) return true;

			if (o3 == 0 && onSegment(p2, p1, q2)) return true;

			
			if (o4 == 0 && onSegment(p2, q1, q2)) return true;

			return false; 
		}

		
		static bool isInside(Point2D<int> polygon[], int n, Point2D<int> p)
		{
			
			if (n < 3) return false;

		
			Point2D<int> extreme = { INF, p.Y };

			
			int count = 0, i = 0;
			do
			{
				int next = (i + 1) % n;

			
				if (doIntersect(polygon[i], polygon[next], p, extreme))
				{
					
					if (orientation(polygon[i], p, polygon[next]) == 0)
						return onSegment(polygon[i], p, polygon[next]);

					count++;
				}
				i = next;
			} while (i != 0);

			return count & 1; // Same as (count%2 == 1)
		}
	};


	PIPSolver::Point polygon1[] = { { -3419, -446 },{ -3371, -417 },
		{ -3367, -407 },{ -3346, -346 },
		{ -3400, -321 },{ -3409, -320 },{ -3425, -375 } };
	int n = sizeof(polygon1) / sizeof(polygon1[0]); 
		PIPSolver::Point p = { -3383, -390 };


		//PIPSolver::isInside(polygon1, n, p) ? cout << "Yes \n" : cout << "No \n";
		Point2D<int> pp = Point2D<int>(point.X, point.Y);
		return PIPSolver::isInside(reinterpret_cast<Point2D<int>*> (points), points_count, pp);
}
