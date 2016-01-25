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
	indices = group.indices;
	group.indices = nullptr;
}

WMOPart::WMOPart(WMOPart && right):filename(std::move(right.filename))
{
	this->position = right.position;
	vertices = right.vertices;
	indices = right.indices;
	vertex_count = right.vertex_count;
	index_count = right.index_count;
	right.vertices = nullptr;
	right.indices = nullptr;
	for (unsigned long i = 0; i<vertex_count; i++)
	{
		vertices[i].color = Utils::Graphics::Color(0.0f, 0.0f, 0.0f, 1.0f);
		if (i % 2 == 0)
			vertices[i].color = Utils::Graphics::Color(1.0f, 0.0f, 1.0f, 1.0f);


	}
}


WMOPart::~WMOPart()
{
}
