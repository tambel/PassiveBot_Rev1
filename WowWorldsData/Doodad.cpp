#include "stdafx.h"
#include "Doodad.h"
using namespace Utils;
using namespace std;
Doodad::Doodad(Doodad && right):
	MapObject(),
	uuid(right.uuid),
	game_position(right.game_position),
	scale(right.scale)
{
	skip = right.skip;
	Model::operator=(std::move(right));
	//uuid = right.uuid;
	//game_position = right.game_position;
	//scale = right.scale;
}
Doodad::Doodad(string filename, unsigned uuid, Position position, short scale) :
	Model(),
	MapObject(filename),
	uuid(uuid),
	scale(scale/1024.0f)
{
	skip = false;
	M2 m2 = M2(this->filename);
	name = m2.filename;
	this->scale = scale/1024.0f;
	this->position.coords=Vector3(position.coords.x,-position.coords.z,position.coords.y);
	this->position.rotation = position.rotation;
	this->game_position=Vector3(Metrics::MapMidPoint-this->position.coords.x,Metrics::MapMidPoint-this->position.coords.z,position.coords.y);
	vertex_count=m2.vertex_count;
	vertices=new Utils::Graphics::Vertex[vertex_count];
	
	/*for (unsigned long i=0;i<vertex_count;i++)
	{
		vertices[i].position=m2.GetVertices()[i].position;
		vertices[i].color=Utils::Graphics::Color(0.0f,0.0f,0.0f,1.0f);
		if (i%2==0)
			vertices[i].color=Utils::Graphics::Color(1.0f,0.0f,1.0f,1.0f);	
	}*/
	unsigned vc = 0;
	rvertices = new float[vertex_count * 3];

	for (unsigned long i = 0; i < vertex_count; i++)
	{
		rvertices[vc] = m2.vertices[i].position.x;
		rvertices[vc+1] = m2.vertices[i].position.y;
		rvertices[vc+2] = m2.vertices[i].position.z;
		vc += 3;
	}
	index_count=m2.index_count;
	indices = m2.indices;
	m2.indices = 0;
	//indices=new unsigned[index_count];

	/*
	for (unsigned long i=0;i<index_count;i++)
	{
		indices[i]=m2->GetIndices()[i];
	}
	*/
	vertices = nullptr;
	Rescale(this->scale);
	Rotate();
	for (unsigned long i = 0; i < vertex_count*3; i+=3)
	{
		rvertices[i] +=this->position.coords.x;
		rvertices[i + 1] += this->position.coords.z;
		rvertices[i + 2] += this->position.coords.y;
	}
	/*unsigned vc = 0;
	rvertices = new float[vertex_count * 3];
	for (unsigned long i = 0; i < vertex_count; i++)
	{
		rvertices[vc] = vertices[i].position.x + this->position.coords.x;
		rvertices[vc+1] = vertices[i].position.z + this->position.coords.z;
		rvertices[vc+2] = vertices[i].position.y + this->position.coords.y;
		vc += 3;
	}*/
	rcCalcBounds(rvertices, vertex_count, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());
	for (unsigned long i = 0; i < index_count; i += 3)
	{
		int i1= indices[i + 2];
		int i2= indices[i + 1];
		int i3 = indices[i];
		indices[i] = i1;
		indices[i + 1] = i2;
		indices[i + 2] = i3;


		/*indices[i] = indices[i + 2];
		indices[i + 1] = indices[i + 1];
		indices[i + 2] = indices[i];*/
		//i += 3;
	}
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
	skip = right.skip;
	return *this;
}

