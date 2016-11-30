#include "stdafx.h"
#include "M2.h"
//#include "Tools\BinaryReader.h"
using namespace std;
M2::M2(string filename):filename(filename)
{
	indices = nullptr;
	this->filename = filename;
	BinaryReader  * reader = new BinaryReader(filename);
	string magic = reader->ReadString(4);
	if (magic == "MD21")
	{
		reader->Read<unsigned>();
		//magic = reader.ReadString(4);
	}

	M2Header_Legion header;
	header = reader->Read<M2Header_Legion>();
	if (header.version == 272)
	{
		header = header;
	}
	vertex_count = header.vertices.number;
	vertices = new M2Vertex[vertex_count];
	reader->ReadArrayAbs<M2Vertexx>(vertices, header.vertices.offset_elements + 8, vertex_count);
	/*
	header=reader->Read<M2Header>();
	char * s[10];

	reader->ReadArray<char>(s, 4);
	


	vertex_count=header.NVertices;
	vertices=new M2Vertex[vertex_count];
	reader->ReadArrayAbs<M2Vertex>(vertices,header.OfsVertices,vertex_count);
	*/
	if (vertex_count == 0)
	{
		vertex_count = vertex_count;
	}
	if (vertex_count!=0)
		LoadSkinFile(0);
	delete reader;
	//delete c_reader;

}
M2::~M2()
{
	//delete reader;
	//delete skin_reader;
	delete [] vertices;
	vertices=0;
	vertex_count=0;
	delete [] indices;
	indices=0;
	index_count=0;
}
void M2::LoadSkinFile(int index)
{
	string skin_path = filename;
	SkinHeader m2_skin;
	skin_path = skin_path.erase(skin_path.length() - 3, 3) + "0" + to_string(index) + ".skin";
	
	BinaryReader * skin_reader = new BinaryReader(skin_path);

	skin_reader->MoveToBegining();
	skin_reader->MoveForward(0);
	m2_skin = skin_reader->Read<SkinHeader>();
	unsigned short * indexLookup = new unsigned short[m2_skin.nIndices];
	skin_reader->ReadArrayAbs<unsigned short>(indexLookup, m2_skin.ofsIndices, m2_skin.nIndices);
	unsigned short * triangles = new unsigned short[m2_skin.nTriangles];
	skin_reader->ReadArrayAbs<unsigned short>(triangles, m2_skin.ofsTriangles, m2_skin.nTriangles);
	index_count = m2_skin.nTriangles;
	indices = new int[index_count];
	if (m2_skin.nIndices == 0)
	{
		index_count = index_count;
	}
	for (unsigned long i = 0; i < m2_skin.nTriangles; i++)
	{
		indices[i] = indexLookup[triangles[i]];
	}
	delete[] triangles;
	delete[] indexLookup;
	delete skin_reader;

}

