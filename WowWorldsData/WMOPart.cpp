#include "stdafx.h"
#include "WMOPart.h"


WMOPart::WMOPart(WMOGroup & group, WMO & wmo)
{
	this->filename = move(group.filename);
	this->position = wmo.GetPosition();
	vertex_count = group.vertex_count;
	index_count = group.index_count;
	vertices = new Utils::Graphics::Vertex[vertex_count];
	for (unsigned long i = 0; i < vertex_count; i++)
	{
		vertices[i].position = group.vertices[i];
		vertices[i].color = Utils::Graphics::Color(0.0f, 0.0f, 0.0f, 1.0f);
		if (i % 2 == 0)
			vertices[i].color = Utils::Graphics::Color(1.0f, 0.0f, 1.0f, 1.0f);
	}
	indices = new int[index_count];
	for (unsigned i = 0; i < index_count; i++)
	{
		indices[i] = group.indices[i];
	}
	//indices = group.indices;
	//group.indices = nullptr;
	//Rotate();
	/*unsigned vc = 0;
	rvertices = new float[vertex_count * 3];
	for (unsigned long i = 0; i < vertex_count; i++)
	{
		rvertices[vc] = vertices[i].position.x + wmo.GetPosition().coords.x;
		rvertices[vc + 1] = vertices[i].position.z + wmo.GetPosition().coords.z;
		rvertices[vc + 2] = vertices[i].position.y + wmo.GetPosition().coords.y;
		vc += 3;
	}
	
	rcCalcBounds(rvertices, vertex_count, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());*/
}

WMOPart::WMOPart(WMOPart && right):filename(std::move(right.filename))
{
	Model::operator=(std::move(right));
	this->position = right.position;
	//vertices = right.vertices;
	//indices = right.indices;
	//vertex_count = right.vertex_count;
	//index_count = right.index_count;
	//right.vertices = nullptr;
	//right.indices = nullptr;
	/*for (unsigned long i = 0; i<vertex_count; i++)
	{
		vertices[i].color = Utils::Graphics::Color(0.0f, 0.0f, 0.0f, 1.0f);
		if (i % 2 == 0)
			vertices[i].color = Utils::Graphics::Color(1.0f, 0.0f, 1.0f, 1.0f);


	}*/
	
}


WMOPart::~WMOPart()
{
}
