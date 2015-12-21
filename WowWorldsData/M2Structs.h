#pragma once
#include "Utils.h"
struct M2Vertex
{
	Vector3 position;
	unsigned char bone_weights[4];
	unsigned char bone_indices[4];
	Vector3	normal_vector;
	struct Vector2
	{
		float X;
		float Y;
	}tex_coords[2];
};
struct M2Skin
{
	unsigned id;
	int nIndices;
	int ofsIndices;
	int nTriangles;
	int ofsTriangles;
	int nProperties;
	int ofsProperties;
	int nSubmeshes;
	int ofsSubmeshes;
	int nTexUnits;
	int ofsTexUnits;
	int bones;
};
struct SkinHeader {
	unsigned ID;
	unsigned nIndices;
	unsigned ofsIndices;
	unsigned nTriangles;
	unsigned ofsTriangles;
	unsigned nProperties;
	unsigned ofsProperties;
	unsigned nSubmeshes;
	unsigned ofsSubmeshes;
	unsigned nTextureUnits;
	unsigned ofsTextureUnits;
	unsigned LOD;
};