#pragma once
#include "Utils.h"
using namespace Utils;
class Model
{

protected:
	Utils::Graphics::Vertex * vertices;
	unsigned long  vertex_count;
	unsigned * indices;
	unsigned long index_count ;
	Position position;
public:

	Model(void);
	~Model(void);
	virtual Utils::Graphics::Vertex * GetVertices(){return vertices;}
	virtual unsigned long GetVertexCount(){return vertex_count;}
	virtual unsigned * GetIndices(){return indices;}
	virtual unsigned long GetIndexCount(){return index_count;} 
	virtual Position GetPosition() { return position;}
	void SetPosition(Position position) {this->position=position;} 
};

