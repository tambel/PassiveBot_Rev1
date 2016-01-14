#pragma once
#include "Recast.h"

#include "Sample.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"

#define MAX_PATHSLOT      128 // how many paths we can store
#define MAX_PATHPOLY      256 // max number of polygons in a path
#define MAX_PATHVERT      512 // most verts in a path 
class WowBuildContext : public rcContext
{
public:
	WowBuildContext();
	~WowBuildContext();
	virtual void doLog(const rcLogCategory /*category*/, const char* /*msg*/, const int /*len*/);
};
typedef struct
{
	float PosX[MAX_PATHVERT];
	float PosY[MAX_PATHVERT];
	float PosZ[MAX_PATHVERT];
	int MaxVertex;
	int Target;
}
PATHDATA;
class SquareArea;
class Navigation
{
	
	float * vertices;
	unsigned long vert_count;
	int * indices;
	unsigned long ind_count;
	dtNavMeshQuery * m_navQuery;


	
	//inline void SetGeometry(float * vert, unsigned long vert_count, unsigned * ind, unsigned long ind_count);
public:
	float bmin[3]; 
	float bmax[3];
	PATHDATA m_PathStore[MAX_PATHSLOT];
	rcPolyMesh * result_mesh;
	void SetArea(SquareArea & area);
	void BuildNavMesh();
	int FindPath(Vector3 & pStartPos, Vector3 & pEndPos, int nPathSlot, int nTarget);
	Navigation();
	~Navigation();
	Navigation & operator=(Navigation && right);
	Model<unsigned> * GetModel();
	void ToFile();
};

