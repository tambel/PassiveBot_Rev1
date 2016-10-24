#include "WMOGroup.h"
#include "stdafx.h"

WMOGroup::WMOGroup(string filename) :filename(std::move(filename))
{
	indices = 0;
	vertices = 0;
	BinaryReader reader = BinaryReader(this->filename);
	//CascReader * reader = new  CascReader(this->filename);
	if (!reader.IsFileExist())
		throw exception((this->filename + "\nWMO file not exist").c_str());
	MOGP header;
	if (!ChunkedData::SeekChunk(reader, ChunkSignatures::WMOSignature::GroupSignatures::Mogp))
		return;
	unsigned chunk_size = reader.Read<unsigned>() - sizeof(MOGP);
	header = reader.Read<MOGP>();
	unsigned long long end_pos = reader.GetPosition() + chunk_size;
	while (reader.GetPosition() < end_pos)
	{
		unsigned sig = reader.Read<unsigned>();
		unsigned size = reader.Read<unsigned>();
		switch (sig)
		{
		case ChunkSignatures::WMOSignature::GroupSignatures::Movt:
			using namespace Utils::Graphics;
			vertex_count = size / sizeof(Vector3);
			vertices = new float[vertex_count*3];
			reader.ReadArray<float>(vertices, vertex_count*3);
			break;
		case ChunkSignatures::WMOSignature::GroupSignatures::Movi:
			index_count = size / 2;
			indices = new unsigned short[index_count];

			reader.ReadArray<unsigned short>(indices, index_count);
			break;
		default:
			reader.MoveForward(size);
		}
		//reader.MoveForward(size);

	}
}

WMOGroup::WMOGroup(WMOGroup && right):filename(std::move(right.filename))
{
	vertices = right.vertices;
	indices = right.indices;
	vertex_count = right.vertex_count;
	index_count = right.index_count;
	right.vertices = nullptr;
	right.indices = nullptr;

}

WMOGroup::~WMOGroup()
{
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
	vertex_count = 0;
	index_count = 0;
}
