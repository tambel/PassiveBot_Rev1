#pragma once
#include <string>
#include <vector>
#include "M2Header.h"
#include "Tools\BinaryReader.h"
#include "Utils.h"
#include "M2Structs.h"

class M2
{
	
	M2Header header;
	//BinaryReader * reader;
	//BinaryReader * skin_reader;
	void LoadSkinFile(int index);
public:
	M2Vertex * vertices;
	unsigned long vertex_count;
	int * indices;
	unsigned index_count;
	string filename;
	M2(string filename);
	~M2();
	M2Vertex * GetVertices() {return vertices;}
	unsigned long GetVertexCount() {return vertex_count;}
	inline int * GetIndices(){return indices;}
	unsigned GetIndexCount() {return index_count;}
};

