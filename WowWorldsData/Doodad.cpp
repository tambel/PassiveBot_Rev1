#include "stdafx.h"
#include "Doodad.h"
using namespace Utils;
using namespace std;
Doodad::Doodad(Doodad && right):
	MapObject(),
	uuid(right.uuid),
	//game_position(right.game_position),
	scale(right.scale)
{
	Model::operator=(std::move(right));
	//uuid = right.uuid;
	//game_position = right.game_position;
	//scale = right.scale;
}
Doodad::Doodad(string filename, unsigned uuid, Position & position, short scale) :
	Model(),
	MapObject(filename),
	uuid(uuid),
	scale(scale/1024.0f)
{
	M2 m2 = M2(this->name);
	//name = m2.filename;
	this->scale = scale/1024.0f;
	this->position.coords=Vector3(position.coords.z,position.coords.x,position.coords.y);
	this->position.rotation = Vector3(position.rotation.z, position.rotation.x, position.rotation.y);
	vertex_count=m2.vertex_count;
	
	unsigned vc = 0;
	vertices = new float[vertex_count * 3];

	for (unsigned long i = 0; i < vertex_count; i++)
	{
		vertices[vc] = m2.vertices[i].position.x;
		vertices[vc+1] = m2.vertices[i].position.z;
		vertices[vc+2] = m2.vertices[i].position.y;
		vc += 3;
	}
	index_count=m2.index_count;
	indices = new int[index_count];
	/*indices = m2.indices;
	m2.indices = 0;*/
	Rescale(this->scale);
	Rotate();
	for (unsigned long i = 0; i < vertex_count*3; i+=3)
	{
		vertices[i] +=this->position.coords.x;
		vertices[i + 1] += this->position.coords.z;
		vertices[i + 2] += this->position.coords.y;
	}
	rcCalcBounds(vertices, vertex_count, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());
	for (unsigned long i = 0; i < index_count; i += 3)
	{
		indices[i] = m2.indices[i + 2];
		indices[i + 1] = m2.indices[i + 1];
		indices[i + 2] = m2.indices[i];
	}
}


Doodad::~Doodad(void)
{

}

Doodad & Doodad::operator=(Doodad && right)
{
	Model::operator=(std::move(right));
	uuid = right.uuid;
	//game_position = right.game_position;
	scale = right.scale;
	return *this;
}

