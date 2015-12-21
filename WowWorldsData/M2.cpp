#include "stdafx.h"
using namespace std;
M2::M2(string filename):filename(filename)
{
	BinaryReader * reader=new  BinaryReader(filename);
	header=reader->Read<M2Header>();
	vertex_count=header.NVertices;
	vertices=new M2Vertex[vertex_count];
	reader->ReadArrayAbs<M2Vertex>(vertices,header.OfsVertices,vertex_count);
	LoadSkinFile(0);
	delete reader;

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
	string skin_path=filename;
	SkinHeader m2_skin;
	skin_path=skin_path.erase(skin_path.length()-3,3)+"0"+to_string(index)+".skin";
	BinaryReader * skin_reader=new BinaryReader(skin_path);
	if (skin_reader->IsFileExist())
	{
		skin_reader->MoveToBegining();
		skin_reader->MoveForward(0);
		m2_skin=skin_reader->Read<SkinHeader>();
		unsigned short * indexLookup= new unsigned short[m2_skin.nIndices];
		skin_reader->ReadArrayAbs<unsigned short>(indexLookup,m2_skin.ofsIndices,m2_skin.nIndices);
		unsigned short * triangles= new unsigned short[m2_skin.nTriangles];
		skin_reader->ReadArrayAbs<unsigned short>(triangles,m2_skin.ofsTriangles,m2_skin.nTriangles);
		index_count=m2_skin.nTriangles;
		indices=new unsigned short[index_count];
		for (unsigned long i=0;i<m2_skin.nTriangles;i++)
		{
			indices[i]=indexLookup[triangles[i]];
		}
		delete [] triangles;
		delete [] indexLookup;
		delete skin_reader;
	}
}