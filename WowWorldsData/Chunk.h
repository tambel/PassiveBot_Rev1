#pragma once
#include "ChunkedData.h"
#include "Utils.h"
#include "Model.h"
#include "Doodad.h"
#include "ADTStructs.h"
using namespace Utils;
using namespace Game;

class ChunkModel
{
public:
	static const unsigned long index_count=768;
	static unsigned short * indices;
	static const unsigned long vertex_count=145;
	static unsigned short* Init()
	{
		static unsigned short *indices=new unsigned short[index_count];
		for (unsigned y = 0; y < 8; ++y)
		{
			for (unsigned x = 0; x < 8; ++x)
			{
				unsigned i = y * 8 * 12 + x * 12;
				indices[i + 0] = y * 17 + x;
				indices[i + 2] = y * 17 + x + 1;
				indices[i + 1] = y * 17 + x + 9;

				indices[i + 3] = y * 17 + x + 1;
				indices[i + 5] = y * 17 + x + 18;
				indices[i + 4] = y * 17 + x + 9;

				indices[i + 6] = y * 17 + x + 18;
				indices[i + 8] = y * 17 + x + 17;
				indices[i + 7] = y * 17 + x + 9;

				indices[i + 9] = y * 17 + x + 17;
				indices[i + 11] = y * 17 + x;
				indices[i + 10] = y * 17 + x + 9;
			}
		}
		return indices;
	}
};
class Chunk:public Model<unsigned short>
{
private:

	Location * location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	Vector3 game_position;
	//Vector3 position;
	Vector3 real_position;
	vector<Doodad*> doodads;

	BinaryReader * root_reader;
	ChunkStreamInfo root_info;
	MCNK header;
	//Utils::Graphics::Vertice * vertices;
	//static unsigned * indices;
	void LoadMcvt();

public:
	bool is_active;
	bool is_new;
	Chunk(void);
	~Chunk(void);
	Chunk(ChunkStreamInfo info, Location * loc,Point2D<int> block_coordinates,Point2D<int> coordinates);
	Location * GetLocation() {return location;}
	Point2D<int> GetBlockCoordinates() {return block_coordinates;}
	Point2D<int> GetCoordinates() {return coordinates;}
	//Utils::Graphics::Vertice * GetVertices() {return vertices;}
	bool operator==(const Chunk & right);
	//static unsigned long* GetIndices() {return s_fileds.indices;}
	Vector3 GetGamePosition() {return game_position;}
	void SetGamePosition(Vector3 position) {game_position=position;}
	Vector3 GetRealPosition() {return real_position;}
	void  SetRealPosition(Vector3 pos) {real_position=pos;}
	void SearchForObjects();

};

