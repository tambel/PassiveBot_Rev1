#pragma once
#include "Utils.h"

using namespace Utils;
class ModelVertBase
{
	
protected:
	Utils::Graphics::Vertex * vertices;
	unsigned long vertex_count;
	Position position;
	
public:
	ModelVertBase(void);
	~ModelVertBase(void);
	virtual Utils::Graphics::Vertex * GetVertices() { return vertices; }
	virtual unsigned long GetVertexCount() { return vertex_count; }
	virtual Position GetPosition() { return position; }
	void SetPosition(Position position) { this->position = position; }
	virtual void Rotate();
	virtual void Rescale(float scale);
};
template<typename T>
class ModelIndBase
{
	
protected:
	T * indices;
	unsigned long index_count;
public:
	ModelIndBase(void)
	{
		indices = 0;
		index_count = 0;

	}
	~ModelIndBase(void)
	{
		delete[] indices;
		indices = 0;
		index_count = 0;
	}
	virtual T * GetIndices() { return indices; }
	virtual unsigned long GetIndexCount() { return index_count; }
};
template<typename T>
class Model:public ModelIndBase<T>, public ModelVertBase
{
public:
	string name;
	Model(void) :
		ModelIndBase<T>(),
		ModelVertBase()
	{

	}
	~Model(void)
	{

	}

};

