#pragma once
#include "Utils.h"
struct MOHD
{
	unsigned  nMaterials;
	unsigned  nGroups;
	unsigned  nPortals;
	unsigned  nLights;
	unsigned  nModels;
	unsigned  nDoodads;
	unsigned  nSets;
	unsigned ambientColor;
	unsigned  wmoAreaTable;
	Utils::WowTypes::Vector3 bboxMin;
	Utils::WowTypes::Vector3 bboxMax;
	unsigned  flags;
};
 struct MOGP
{
	unsigned groupName;
	unsigned descGroupName;
	unsigned flags;
	Vector3 bboxMin;
	Vector3 bboxMax;
	short ofsMopr;
	short numMopr;
	short numBatchesA;
	short numBatchesB;
	unsigned numBatchesC;
	char fogIndices[4];
	unsigned liquidType;
	unsigned wmoGroupId;
	unsigned unk1, unk2;
 };
