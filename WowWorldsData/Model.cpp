#include "stdafx.h"

Model::Model(void)
{
	vertices=0;
	vertex_count=0;
	indices=0;
	index_count=0;
}


Model::~Model(void)
{
	delete [] vertices;
	vertices=0;
	vertex_count=0;
	delete [] indices;
	indices=0;
	index_count=0;
}
