#include "stdafx.h"
#include "Doodad.h"
using namespace Utils;
using namespace std;
Doodad::Doodad(Doodad && right):
	uuid(right.uuid),
	game_position(game_position),
	scale(right.scale)
{
}
Doodad::Doodad(string filename,unsigned uuid, Position position, short scale):uuid(uuid),scale(scale/1024.0f)
{
	M2 m2 = M2(filename);
	name = m2.filename;
	this->position.coords=Vector3(position.coords.x,-position.coords.z,position.coords.y);
	this->position.rotation = position.rotation;
	this->game_position=Vector3(Metrics::MapMidPoint-this->position.coords.x,Metrics::MapMidPoint-this->position.coords.z,position.coords.y);
	vertex_count=m2.GetVertexCount();
	vertices=new Utils::Graphics::Vertex[vertex_count];
	for (unsigned long i=0;i<vertex_count;i++)
	{
		vertices[i].position=m2.GetVertices()[i].position;
		vertices[i].color=Utils::Graphics::Color(0.0f,0.0f,0.0f,1.0f);
		if (i%2==0)
			vertices[i].color=Utils::Graphics::Color(1.0f,0.0f,1.0f,1.0f);
		
			
	}
	index_count=m2.GetIndexCount();
	indices = m2.indices;
	m2.indices = 0;
	//indices=new unsigned[index_count];

	/*
	for (unsigned long i=0;i<index_count;i++)
	{
		indices[i]=m2->GetIndices()[i];
	}
	*/
	Rescale(this->scale);
	Rotate();
}


Doodad::~Doodad(void)
{

}

Doodad & Doodad::operator=(Doodad && right)
{
	Model::operator=(std::move(right));
	uuid = right.uuid;
	game_position = right.game_position;
	scale = right.scale;
	return *this;
}

