#pragma once
namespace Utils
{
	const float PI = 3.14159265358979323846f;
	namespace WowTypes
	{
		struct Vector3
		{
			float x;
			float y;
			float z;
			Vector3()
			{
				x = 0;
				y = 0;
				z = 0;
			}
			Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
			Vector3 operator+(const Vector3 &right)
			{
				return Vector3(this->x + right.x, this->y + right.y, this->z + right.z);
			}
			Vector3 operator-(const Vector3 &right)
			{
				return Vector3(this->x - right.x, this->y - right.y, this->z - right.z);
			}

			Vector3 operator*(const float  &right)
			{
				return Vector3(this->x*right, this->y*right, this->z*right);
			}
			bool  operator==(const Vector3 &right)
			{
				return x == right.x && y == right.y;
			}
			bool  operator!=(const Vector3 &right)
			{
				return x != right.x || y != right.y;
			}
		};
		/*
		struct Vector3
		{
		float x;
		float y;
		float z;
		};
		*/
		struct Position
		{
			Vector3 coords;
			Vector3 rotation;
			Position() {};
			Position(Vector3 coords, Vector3 rotation) : coords(coords), rotation(rotation) {}
		};
	}
}