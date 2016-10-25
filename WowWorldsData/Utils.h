#pragma once
#include "WowInteraction\Utils.h"
#include <math.h>
#include <string>
#include <vector>
#include "boost\geometry\geometry.hpp"
#include "boost\numeric\ublas\matrix.hpp"
#include "boost\numeric\ublas\vector.hpp"
using Vector3F = boost::geometry::model::point<float, 3, boost::geometry::cs::cartesian>;
using namespace std;
namespace Utils
{
	namespace Geometry
	{
		enum class  Axis
		{
			X,
			Y,
			Z
		};
		enum class AngleValueType
		{
			degree,
			radian
		};
		class Transformer3D
		{
			

		public:
			static boost::numeric::ublas::matrix<float> rotation_matrix_X;
			static boost::numeric::ublas::matrix<float> rotation_matrix_Y;
			static boost::numeric::ublas::matrix<float> rotation_matrix_Z;
			Transformer3D();
			static Vector3F Rotate(float angle, AngleValueType value, Axis axis);
			static void SetRotationAngle(Axis axis, float angle);

		};
	};
	namespace Game
	{
		struct Location
		{
			string name;
			unsigned id=0;
			Location() {}
			Location(string name,unsigned id):name(name),id(id){};
			//Location(Location & other) = default;
			//Location & operator=(Location & other) = default;
			bool operator==(const Location & other) { return id == other.id; }
			bool operator!=(const Location & other) { return id != other.id; }
		};
		class LocationBase
		{
			static bool initialized;
			static vector<Location> locations;
		public:
			static void Init();
			static void Add(Location & loc);
			static void Clear();
			static bool IsInitialized();
			static Location & Get(string name);
			static Location & Get(unsigned id);
		};
	};
	namespace ChunkSignatures
	{
		namespace ADTSignature
		{
			static const unsigned Mver = 0x4D564552;
			static const unsigned Mhdr = 0x4D484452;
			static const unsigned Mcin = 0x4D43494E;
			static const unsigned Mtex = 0x4D544558;
			static const unsigned Mmdx = 0x4D4D4458;
			static const unsigned Mmid = 0x4D4D4944;
			static const unsigned Mwmo = 0x4D574D4F;
			static const unsigned Mwid = 0x4D574944;
			static const unsigned Mddf = 0x4D444446;
			static const unsigned Modf = 0x4D4F4446;
			// ReSharper disable once InconsistentNaming
			static const unsigned Mh2o = 0x4D48324F;
			static const unsigned Mfbo = 0x4D46414F;
			static const unsigned Mtxf = 0x4D545846;
			static const unsigned Mtxp = 0x4D545850;
			static const unsigned Mcnk = 0x4D434E4B;
			namespace MCNKSignatures
			{
				static const unsigned Mcvt = 0x4D435654;
				static const unsigned Mcrd = 0x4D435244;
				static const unsigned Mcrw = 0x4D435257;
			};
		};
		namespace WMOSignature
		{
			static const unsigned Mohd = 0x4D4F4844;
			namespace GroupSignatures
			{
				static const unsigned Mogp = 0x4D4F4750;
				static const unsigned Movt = 0x4D4F5654;
				static const unsigned Movi = 0x4D4F5649;
			};
		};
	};
	namespace Resources
	{
	};
	namespace Metrics
	{
		const float BlockSize=533.0f + 1.0f / 3.0f;
		const float ChunkSize=BlockSize/16.0f;
		const float UnitSize = ChunkSize / 8.0f;
		const float MapSize=BlockSize*64.0f;
		const float MapMidPoint = 32.0f * BlockSize;





	}
	/*template<class T>
	struct Point2D
	{
		T X;
		T Y;
		Point2D():X(0),Y(0){}
		Point2D(T x,T y):X(x),Y(y){}
		bool operator==(const Point2D & right)
		{
			return (this->X==right.X && this->Y==right.Y);
		}
		bool operator!=(const Point2D & right)
		{
			return (this->X!=right.X || this->Y!=right.Y);
		}
		Point2D operator -(const Point2D & right)
		{
			return Point2D(this->X-right.X,this->Y-right.Y);
		}
		Point2D operator +(const Point2D & right)
		{
			return Point2D(this->X+right.X,this->Y+right.Y);
		}
		Point2D operator -=(const Point2D & right)
		{
			return Point2D(this->X-right.X,this->Y-right.Y);
		}
		Point2D operator +=(const Point2D & right)
		{
			return Point2D(this->X+right.X,this->Y+right.Y);
		}
		
	};*/
	Point2D<int> WorldPositionToBlockCoords(Vector3 position);
	Point2D<float> BlockCoordsToWorldPosition(Point2D<int> coords);
	Point2D<int> WorldPositionToChunkCoords(Vector3 position);
	Point2D<int> ToGlobalChunkCoords(Point2D<int> & block_coordinates, Point2D<int> & coordinates);

	namespace Graphics
	{
		struct Triangle
		{
			unsigned short indices[3];
		};



		/*
		struct Point2DI
		{
		int X;
		int Y;
		};
		*/
		struct Color
		{
			float r;
			float g;
			float b;
			float a;
			Color(){};
			Color(float r,float g, float b, float a) :r(r),a(a),g(g),b(b){}
		};

		struct Vertex
		{
			Vector3 position;
			Color color;
			Vertex(){};
			Vertex(Vector3 pos,Color color): position(pos), color(color){}
		};
		struct BoundingBox
		{
			Vector3 up;
			Vector3 down;
			bool IsInside2D(Vector3 point);
			inline float * GetArrayMin() { return reinterpret_cast<float*>(&up); }
			inline float * GetArrayMax() { return reinterpret_cast<float*>(&down); }
		};
	}
}
