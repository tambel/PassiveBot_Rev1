#include "stdafx.h"
#include <Windows.h>

int * ChunkModel::indices=ChunkModel::Init();

Chunk::Chunk(void)
{
}
Chunk::Chunk(Area * area,ChunkStreamInfo info, ChunkStreamInfo obj_info, ADT * adt_file/*, Location * location, Point2D<int> block_coordinates*/,Point2D<int> coordinates):
	Model<int>(),
	root_info(info),
	adt(adt_file)/*,location(location),block_coordinates(block_coordinates)*/,
	coordinates(coordinates),
	area(area)
{

	yey=false;
	result_mesh = 0;
	location = adt->GetLocation();
	block_coordinates = adt->GetCoordinates();
	//wmos = vector<WMO*>();
	header=MCNK();
	root_info=info;
	root_reader=root_info.reader;
	this->obj_reader = obj_info.reader;
	//indices=ChunkModel::indices;
	index_count=ChunkModel::index_count;
	indices = new int[768];
	vertex_count=ChunkModel::vertex_count;
	//root_reader->GetStream()->seekg(root_info.start,ios::beg);
	root_reader->SetPosition(root_info.start);
	header =root_reader->Read<MCNK>();
	game_position=header.position;
	//root_reader->GetStream()->read((char*)&header,sizeof(MCNK));
	unsigned sig;
	unsigned size;
	while (root_reader->GetPosition()<=root_info.start+root_info.size-8)
	{
		sig=root_reader->ReadUInt();
		size=root_reader->ReadUInt();
		switch (sig)
		{
		case Utils::ChunkSignatures::ADTSignature::MCNKSignatures::Mcvt:
		{
			LoadMcvt();
			break;
		}
		default:
			break;
		}
	}
	obj_reader->SetPosition(obj_info.start);
	while (obj_reader->GetPosition() <= obj_info.start + obj_info.size-8)
	{
		sig = obj_reader->ReadUInt();
		size = obj_reader->ReadUInt();
		switch (sig)
		{
		case Utils::ChunkSignatures::ADTSignature::MCNKSignatures::Mcrd:
		{
			LoadMcrd(size);
			break;
		}
		case Utils::ChunkSignatures::ADTSignature::MCNKSignatures::Mcrw:
		{	
			LoadMcrw(size);
			break;
		}
		default:
			break;
		}
	}
	//InitNavigation();

	//root_reader->ReadBytes((char*)&height,

}
Chunk::~Chunk(void)
{
}
void Chunk::LoadMcvt()
{
	
	float * heights=new float[145];
	root_reader->ReadArray<float>((char*)heights,145);
	position.coords=Vector3(0,0,header.position.z);
	real_position=Vector3(Metrics::MapMidPoint - header.position.y,-(Metrics::MapMidPoint - header.position.x),header.position.z);
	/*
	float posx = Metrics::MapMidPoint - header.position.y;
	float posy = Metrics::MapMidPoint + header.position.x;
	float posz = header.position.z;
	*/
	game_position= Vector3(-game_position.y,game_position.x,game_position.z);
				
	//posx=0;
	//posy=0;
	int counter = 0;
	int counter2 = 0;
	vertices=new Utils::Graphics::Vertex[145];
	rvertices = new float[435];
	for(int i = 0; i < 17; ++i)
	{
		for(int j = 0; j < (((i % 2) != 0) ? 8 : 9); ++j)
		{
			float height = position.coords.z + heights[counter];
			float x = position.coords.x + j * Metrics::UnitSize;
			if ((i % 2) != 0)
				x += 0.5f * Metrics::UnitSize;
			float y = position.coords.y - i * Metrics::UnitSize * 0.5f;

			vertices[counter].position =Vector3(x, y, heights[counter]);
			rvertices[counter2++] = x + real_position.x;
			rvertices[counter2++] = heights[counter] + real_position.z;
			rvertices[counter2++] = y + real_position.y;

			
			if ((i % 2) != 0)
			{

				vertices[counter].color=Graphics::Color(1.0f,1.0f,1.0f,1.0f);
				
			}
			else
			{
				vertices[counter].color=Graphics::Color(0.0f,0.0f,0.0f,0.0f);
				
				
			}
			if (i == 0 && (j == 0 || j == 1))
				{
					
					vertices[counter].color=Graphics::Color(0.0f,1.0f,1.0f,0.0f);
				}
			++counter;
		}
	}
	for (int i = 0; i < index_count; i += 3)
	{
		indices[i] = ChunkModel::indices[i+2];
		indices[i+1] = ChunkModel::indices[i + 1];
		indices[i+2] = ChunkModel::indices[i];
	}
	rcCalcBounds(rvertices, vertex_count, reinterpret_cast<float*>(&bounding_box.up), reinterpret_cast<float*>(&bounding_box.down));

	delete [] heights;
}
void Chunk::LoadMcrd(unsigned long size)
{
	bool exist;
	unsigned count = size / 4;
	unique_ptr<unsigned> doodads_refs = unique_ptr<unsigned>(new unsigned[count]);
	obj_reader->ReadArray<unsigned>(doodads_refs.get(), count);
	for (unsigned i = 0; i < count; i++)
	{
		
		MDDF mddf = adt->GetMDDFs()[doodads_refs.get()[i]];
		exist = false;
		for (auto &doodad_ptr : area->GetDoodads())
		{
			Doodad * doodad = doodad_ptr.get();
			if (mddf.UniqueId == doodad->GetUUID())
			{
				exist = true;
				doodad->Refresh();
				if (!doodad->IsOccupied())
				{
					doodads.push_back(doodad);
					doodad->Occupie();
				}
				break;
			}
		}


		string filename= Configuration::GetGameDataPath()+(adt->GetDoodadsFilenames()+adt->GetDoodadsIds()[mddf.Mmid]);
		Doodad * doodad = new Doodad(filename, mddf.UniqueId, Position(mddf.Position, mddf.Rotation), mddf.Scale);
		doodad->Occupie();
		//wmos.push_back(move(wmo));
		area->GetDoodads().push_back(move(unique_ptr<Doodad>(doodad)));
		doodads.push_back(area->GetDoodads().back().get());
	}
}
void Chunk::LoadMcrw(unsigned long size)
{
	bool exist;
	unsigned count = size / 4;
	unique_ptr<unsigned> wmo_refs = unique_ptr<unsigned>(new unsigned[count]);
	obj_reader->ReadArray<unsigned>(wmo_refs.get(), count);
	for (unsigned i = 0; i < count; i++)
	{
		MODF modf = adt->GetMODFs()[wmo_refs.get()[i]];
		exist = false;
		for (auto &wmo_ptr : area->GetWMOs())
		{
			WMO * wmo = wmo_ptr.get();
			if (modf.UniqueId == wmo->GetUUID())
			{
				exist = true;
				//wmo->Refresh();
				if (!wmo->IsOccupied())
				{
					wmos.push_back(wmo);
					wmo->Occupie();
				}
				break;
			}
		}
		if (!exist)
		{
			string filename = Configuration::GetGameDataPath() + (adt->GetWMOFilenames() + adt->GetWMOsIds()[modf.Mwid]);
			WMO  * wmo = new WMO(filename, modf.UniqueId, Position(modf.Position, modf.Rotation));
			wmo->Occupie();
			area->GetWMOs().push_back(move(unique_ptr<WMO>(wmo)));
			wmos.push_back(area->GetWMOs().back().get());
		
		}
		/*string filename = Configuration::GetGameDataPath() + (adt->GetWMOFilenames() + adt->GetWMOsIds()[modf.Mwid]);
		WMO wmo = WMO(filename, modf.UniqueId, Position(modf.Position, modf.Rotation));
		wmos.push_back(move(wmo));*/

	}
}
void Chunk::InitNavigation()
{
	//unsigned long vert_count=0;
	//unsigned long ind_count = 0;;
	//unique_ptr<float> vertices;
	//unique_ptr<int> indices;
	////float * nav_vertices;
	vert_count = 0;
	ind_count = 0;
	vert_count += vertex_count;
	ind_count += index_count;


	/*for (auto &doodad : doodads)
	{
		vert_count += doodad->GetVertexCount();
		ind_count += doodad->GetIndexCount();
	}*/



	//for (auto &wmo : wmos)
	//{
	//	//if (wmo->IsSkipped()) continue;
	//	for (auto &part : wmo->GetParts())
	//	{
	//		vert_count += part.GetVertexCount();
	//		ind_count += part.GetIndexCount();
	//	}
	//}
	nav_vertices = unique_ptr<float>(new float[vert_count*3]);
	nav_indices = unique_ptr<int>(new int[ind_count]);
	unsigned long vert_offset = 0;
	for (unsigned i = 0; i < 145; i++)
	{
		/*vertices.get()[vert_offset] = this->vertices[i].position.x + position.coords.x;
		vertices.get()[vert_offset+1] = this->vertices[i].position.z + position.coords.z;
		vertices.get()[vert_offset+2] = this->vertices[i].position.y + position.coords.y;*/

		nav_vertices.get()[vert_offset] = this->vertices[i].position.x + real_position.x;
		nav_vertices.get()[vert_offset+1] = this->vertices[i].position.z + real_position.z;
		nav_vertices.get()[vert_offset+2] = this->vertices[i].position.y + real_position.y;
		vert_offset += 3;
	}
	rcCalcBounds(nav_vertices.get(), vertex_count, reinterpret_cast<float*>(&terrain_bounding_box.up), reinterpret_cast<float*>(&terrain_bounding_box.down));

	/*for (auto &doodad : doodads)
	{
		for (unsigned long i = 0; i < doodad->GetVertexCount(); i++)
		{
			nav_vertices.get()[vert_offset] = doodad->GetVertices()[i].position.x + doodad->GetPosition().coords.x;
			nav_vertices.get()[vert_offset + 1] = doodad->GetVertices()[i].position.z + doodad->GetPosition().coords.z;
			nav_vertices.get()[vert_offset + 2] = doodad->GetVertices()[i].position.y + doodad->GetPosition().coords.y;
			vert_offset += 3;
		}
	}*/

	//for (auto &wmo : wmos)
	//{
	//	for (auto &part : wmo->GetParts())
	//	{
	//		for (unsigned long i = 0; i < part.GetVertexCount(); i++)
	//		{
	//			Vector3 vert = part.GetVertices()[i].position + (wmo->GetPosition().coords - real_position);

	//			nav_vertices.get()[vert_offset] = part.GetVertices()[i].position.x+wmo->GetPosition().coords.x;
	//			nav_vertices.get()[vert_offset + 1] = part.GetVertices()[i].position.z + wmo->GetPosition().coords.z;
	//			nav_vertices.get()[vert_offset + 2] = part.GetVertices()[i].position.y + wmo->GetPosition().coords.y;
	//			vert_offset += 3;
	//		}
	//	}
	//}
	rcCalcBounds(nav_vertices.get(), vert_count, reinterpret_cast<float*>(&full_bounding_box.up), reinterpret_cast<float*>(&full_bounding_box.down));

	vert_offset = 0;
	unsigned long ind_offset = 0;
	for (unsigned i = 0; i < 768; i+=3)
	{
		nav_indices.get()[ind_offset] = this->indices[i + 2] + vert_offset;
		nav_indices.get()[ind_offset + 1] = this->indices[i + 1] + vert_offset;
		nav_indices.get()[ind_offset + 2] = this->indices[i] + vert_offset;
		ind_offset += 3;
	}
	vert_offset += this->vertex_count;

	/*for (auto &doodad : doodads)
	{
		for (unsigned long i = 0; i < doodad->GetIndexCount(); i+=3)
		{
			nav_indices.get()[ind_offset] = doodad->GetIndices()[i + 2] + vert_offset;
			nav_indices.get()[ind_offset + 1] = doodad->GetIndices()[i + 1] + vert_offset;
			nav_indices.get()[ind_offset + 2] = doodad->GetIndices()[i] + vert_offset;
			ind_offset += 3;
		}
		vert_offset += doodad->GetVertexCount();
	}*/

	//for (auto &wmo : wmos)
	//{
	//	//if (wmo->IsSkipped()) continue;
	//	for (auto &part : wmo->GetParts())
	//	{
	//		for (unsigned long i = 0; i < part.GetIndexCount(); i+=3)
	//		{
	//			nav_indices.get()[ind_offset] = part.GetIndices()[i + 2] + vert_offset;
	//			nav_indices.get()[ind_offset + 1] = part.GetIndices()[i + 1] + vert_offset;
	//			nav_indices.get()[ind_offset + 2] = part.GetIndices()[i] + vert_offset;
	//			ind_offset += 3;
	//		}
	//		vert_offset += part.GetVertexCount();
	//	}
	//}
	ofstream file;
	file.open(to_string(coordinates.X)+" "+to_string(coordinates.Y)+" test.obj");
	for (unsigned vi = 0; vi < vert_count*3; vi+=3)
	{
		file << "v " << nav_vertices.get()[vi] << " " << nav_vertices.get()[vi + 1] << " " << nav_vertices.get()[vi + 2] << endl;
	}
	for (unsigned ii = 0; ii < ind_count; ii += 3)
	{
		file << "f " << nav_indices.get()[ii]+1 << " " << nav_indices.get()[ii + 1]+1 << " " << nav_indices.get()[ii + 2]+1 << endl;
	}
	file.close();
	////
	return;
}
bool Chunk::operator==(const Chunk & right)
{
	return this->location->id==right.location->id && this->block_coordinates==right.block_coordinates && this->coordinates==right.coordinates;
}
void Chunk::SearchForObjects()
{

}