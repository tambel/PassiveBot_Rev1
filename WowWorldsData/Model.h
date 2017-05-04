#pragma once
#include "Utils.h"
using namespace Utils;
using namespace Utils::Geometry;

//class ModelVertBase
//{
//	
//protected:
//	Utils::Graphics::Vertex * vertices;
//	unsigned long vertex_count;
//	
//	
//public:
//	Position position;
//	float * rvertices = 0;
//	ModelVertBase(ModelVertBase && right);
//	ModelVertBase(void);
//	~ModelVertBase(void);
//	ModelVertBase & operator=(ModelVertBase && right);
//	virtual Utils::Graphics::Vertex * GetVertices() { return vertices; }
//	virtual unsigned long GetVertexCount() { return vertex_count; }
//	virtual Position GetPosition() { return position; }
//	void SetPosition(Position position) { this->position = position; }
//	inline void SetPosition(Vector3 & position) { this->position.coords = position; }
//	virtual void Rotate();
//	virtual void Rescale(float scale);
//};
//class ModelIndBase
//{
//	
//protected:
//	int * indices;
//	unsigned long index_count;
//public:
//	ModelIndBase(ModelIndBase && right)
//	{
//		indices = right.indices;
//		index_count = right.index_count;
//		right.indices = nullptr;
//		right.index_count = 0;
//	}
//	ModelIndBase(void)
//	{
//		indices = 0;
//		index_count = 0;
//
//	}
//	~ModelIndBase(void)
//	{
//		delete [] indices;
//		indices = 0;
//		index_count = 0;
//	}
//	ModelIndBase & operator=(ModelIndBase && right)
//	{
//		indices = right.indices;
//		index_count = right.index_count;
//		right.indices = nullptr;
//		right.index_count = 0;
//		return *this;
//	}
//	virtual int * GetIndices() { return indices; }
//	virtual unsigned long GetIndexCount() { return index_count; }
//};
class Model
{


	void _move(Model & other);
public:
	unsigned long vertex_count=0;
	int * indices=nullptr;
	unsigned long index_count=0;
	Position position;
	float * vertices = 0;
	//virtual Utils::Graphics::Vertex * GetVertices() { return vertices; }
	virtual float* GetVertices() { return vertices; }
	virtual unsigned long GetVertexCount() { return vertex_count; }
	virtual Position GetPosition() { return position; }
	void SetPosition(Position position) { this->position = position; }
	inline void SetPosition(Vector3 & position) { this->position.coords = position; }
	virtual void Rotate();
	static  void Rotate(float vertices,unsigned vertex_count,int indices, unsigned index_count,float angle);
	virtual void Rescale(float scale);
	virtual int * GetIndices() { return indices; }
	virtual unsigned long GetIndexCount() { return index_count; }
	BoundingBox bounding_box;
	Model();
	Model(Utils::Geometry::BoundingBox bounding_box);
	~Model();
	Model(Model&) = delete;
	Model & operator=(Model &) = delete;
	void Rotate(float angle);

	Model & operator=(Model && right);
	inline Utils::Geometry::BoundingBox & GetBoundingBox() {return bounding_box;}
	void CalcBounds();
};


