#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;



//ModelVertBase::ModelVertBase(ModelVertBase && right)
//{
//	rvertices = right.rvertices;
//	rvertices = nullptr;
//	vertices = right.vertices;
//	vertex_count = right.vertex_count;
//	right.vertices = nullptr;
//	right.vertex_count = 0;
//}
//
//ModelVertBase::ModelVertBase(void)
//{
//	vertices = 0;
//	vertex_count = 0;
//}
//
//ModelVertBase::~ModelVertBase(void)
//{
//	delete[] rvertices;
//	rvertices = nullptr;
//	delete[] vertices;
//	vertices = nullptr;
//	vertex_count = 0;
//}
//
//ModelVertBase & ModelVertBase::operator=(ModelVertBase && right)
//{
//	rvertices = right.rvertices;
//	right.rvertices = nullptr;
//	position = right.position;
//	vertices = right.vertices;
//	vertex_count = right.vertex_count;
//	right.vertices = nullptr;
//	right.vertex_count = 0;
//	return *this;
//}
//
//
//void ModelVertBase::Rotate()
//{
//		
//	float angle_x = position.rotation.x *M_PI / 180.0;
//	float angle_y = position.rotation.y *M_PI / 180.0;
//	float angle_z = position.rotation.z * M_PI / 180.0;
//	boost::numeric::ublas::vector<float> vertice(4);
//	Geometry::Transformer3D::SetRotationAngle(Geometry::Axis::X, angle_x);
//	Geometry::Transformer3D::SetRotationAngle(Geometry::Axis::Y, angle_z);
//	Geometry::Transformer3D::SetRotationAngle(Geometry::Axis::Z, angle_y-1.57);
//	boost::numeric::ublas::matrix<float> rotX = Geometry::Transformer3D::rotation_matrix_X;
//	boost::numeric::ublas::matrix<float> rotY = Geometry::Transformer3D::rotation_matrix_Y;
//	boost::numeric::ublas::matrix<float> rotZ = Geometry::Transformer3D::rotation_matrix_Z;
//
//	
//	for (unsigned long i = 0; i < vertex_count; i++)
//	{
//		vertice(0) = vertices[i].position.x;
//		vertice(1) = vertices[i].position.y;
//		vertice(2) = vertices[i].position.z;
//		vertice(3) = 1.0f;
//		boost::numeric::ublas::matrix<float>tm = prod(rotY,rotZ);
//		tm = prod(rotX, tm);
//		vertice= boost::numeric::ublas::prod( vertice,tm);
//		vertices[i].position.x = vertice(0);
//		vertices[i].position.y = vertice(1);
//		vertices[i].position.z = vertice(2);
//
//	}
//}
//
//void ModelVertBase::Rescale(float scale)
//{
//	for (unsigned long i = 0; i < vertex_count; i++)
//	{
//		//vertices[i].position = Vector3(vertices[i].position.x + scale, vertices[i].position.y + scale, vertices[i].position.z + scale);
//		vertices[i].position.x *= scale;
//		vertices[i].position.y *= scale;
//		vertices[i].position.z *= scale;
//	}
//}

void Model::_move(Model & other)
{
	bounding_box = other.bounding_box;
	name = other.name;

	rvertices = other.rvertices;
	rvertices = nullptr;
	//vertices = other.vertices;
	vertex_count = other.vertex_count;
	//other.vertices = nullptr;
	other.vertex_count = 0;

	indices = other.indices;
	index_count = other.index_count;
	other.indices = nullptr;
	other.index_count = 0;
}

Model::Model()
{

}

Model::Model(Utils::Graphics::BoundingBox bounding_box):
	bounding_box(bounding_box)
{
}

Model::~Model(void)
{
	delete[] rvertices;
	rvertices = nullptr;
	//delete[] vertices;
	//vertices = nullptr;
	vertex_count = 0;
	delete[] indices;
	indices = 0;
	index_count = 0;
}

Model & Model::operator=(Model && right)
{
	_move(right);
	return *this;
}

void Model::Rotate()
{

	float angle_x = position.rotation.x *M_PI / 180.0;
	float angle_y = position.rotation.y *M_PI / 180.0;
	float angle_z = position.rotation.z * M_PI / 180.0;
	boost::numeric::ublas::vector<float> vertice(4);
	Geometry::Transformer3D::SetRotationAngle(Geometry::Axis::X, angle_x );
	Geometry::Transformer3D::SetRotationAngle(Geometry::Axis::Y, angle_z);
	Geometry::Transformer3D::SetRotationAngle(Geometry::Axis::Z, angle_y -1.57);
	boost::numeric::ublas::matrix<float> rotX = Geometry::Transformer3D::rotation_matrix_X;
	boost::numeric::ublas::matrix<float> rotY = Geometry::Transformer3D::rotation_matrix_Y;
	boost::numeric::ublas::matrix<float> rotZ = Geometry::Transformer3D::rotation_matrix_Z;

	
	for (unsigned long i = 0; i < vertex_count*3; i+=3)
	{
		vertice(0) = rvertices[i];
		vertice(2) = rvertices[i  + 2];
		vertice(1) = rvertices[i  + 1];
		vertice(3) = 1.0f;
		boost::numeric::ublas::matrix<float>tm = prod(rotY, rotZ);
		tm = prod(rotX, tm);
		vertice = boost::numeric::ublas::prod(vertice, tm);
		rvertices[i ] = vertice(0);
		rvertices[i  + 1] = vertice(2);
		rvertices[i  + 2] = vertice(1);
	}
	

}

void Model::Rotate(float vertices, unsigned vertex_count, int indices, unsigned index_count, float angle)
{

}

void Model::Rescale(float scale)
{
	for (unsigned long i = 0; i < vertex_count * 3; i++)
	{
		//vertices[i].position = Vector3(vertices[i].position.x + scale, vertices[i].position.y + scale, vertices[i].position.z + scale);
		rvertices[i] *= scale;
	}

}