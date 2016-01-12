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
	ModelVertBase(ModelVertBase && right);
	ModelVertBase(void);
	~ModelVertBase(void);
	ModelVertBase & operator=(ModelVertBase && right);
	virtual Utils::Graphics::Vertex * GetVertices() { return vertices; }
	virtual unsigned long GetVertexCount() { return vertex_count; }
	virtual Position GetPosition() { return position; }
	void SetPosition(Position position) { this->position = position; }
	inline void SetPosition(Vector3 & position) { this->position.coords = position; }
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
	ModelIndBase(ModelIndBase && right)
	{
		indices = right.indices;
		index_count = right.index_count;
		right.indices = nullptr;
		right.index_count = 0;
	}
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
	ModelIndBase & operator=(ModelIndBase && right)
	{
		indices = right.indices;
		index_count = right.index_count;
		right.indices = nullptr;
		right.index_count = 0;
		return *this;
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
	Model(Model&) = delete;
	Model & operator=(Model &) = delete;
	~Model(void)
	{

	}
	Model(Model && right)
	{

	}
	Model & operator=(Model && right)
	{
		ModelVertBase::operator=(std::move(right));
		ModelIndBase::operator=(std::move(right));
		name = right.name;
		return *this;
	}

};


