#include "stdafx.h"
namespace Utils
{
	Point2D<int> WorldPositionToBlockCoords(Vector3 position)
	{
		return Point2D<int>(floor((32 - (position.x / (Metrics::BlockSize)))),floor((32 - (position.y / (Metrics::BlockSize)))));
	}
	Point2D<float> BlockCoordsToWorldPosition(Point2D<int> coords)
	{
		return Point2D<float>((32-coords.X)*Metrics::BlockSize,(32-coords.Y)*Metrics::BlockSize);
	}
	Point2D<int> WorldPositionToChunkCoords(Vector3 position)
	{
		Point2D<int> block_coords=WorldPositionToBlockCoords(position);
		Point2D<float> block_position=BlockCoordsToWorldPosition(block_coords);
		Point2D<int> r = Point2D<int>(floor(abs(position.x-block_position.X)/Metrics::ChunkSize),floor(abs(position.y-block_position.Y)/Metrics::ChunkSize));
		return r;
	}
	namespace Game
	{
		vector<Location*> LocationBase::locations=vector<Location*>();
		bool LocationBase::initialized=false;
		void LocationBase::Add(Location * loc)
		{
			locations.push_back(loc);
		}
		void LocationBase::Clear()
		{
			for (auto loc:locations)
			{
				delete loc;
			}
			locations.clear();
		}
		void LocationBase::Init()
		{
			locations.push_back(new Location("Kalimdor",1));
			initialized=true;
		}
		bool LocationBase::IsInitialized()
		{
			return initialized;
		}
		Location * LocationBase::Get(string name)
		{
			for (auto loc:locations)
			{
				if (loc->name==name)
				{
					return loc;
				}
			}
			return 0;
		}
		Location * LocationBase::Get(unsigned id)
		{
			for (auto loc:locations)
			{
				if (loc->id==id)
				{
					return loc;
				}
			}
			return 0;
		}

	}
	namespace Graphics
	{
		bool BoundingBox::IsInside2D(Vector3 point)
		{
			return ((up.x<=point.x && up.y<=point.y) && (down.x>=point.x && down.y>=point.y));
		}
	};
}