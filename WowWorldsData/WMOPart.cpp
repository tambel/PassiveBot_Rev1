#include "stdafx.h"
#include "WMOPart.h"


WMOPart::WMOPart(WMOGroup & group)
{
	vertex_count = group.vertex_count;
	index_count = group.index_count;
	vertices = new Utils::Graphics::Vertex[vertex_count];
	for (unsigned long i = 0; i < vertex_count; i++)
	{
		vertices[i].position = group.vertices[i];
	}
	indices = group.indices;
	group.indices = 0;
}


WMOPart::~WMOPart()
{
}
