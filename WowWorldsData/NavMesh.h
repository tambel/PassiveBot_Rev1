#pragma once
#include "Area.h" 


class CTX :public rcContext
{
public:
	CTX() :rcContext() {}
	virtual void doLog(const rcLogCategory category, const char* msg, const int len)
	{
		cout << msg << endl;
	}
};

#include "NavArea.h"
class NavMesh
{
	void RasterizeModel(Model * model);
protected:
	Area area;
	NavConfig config;
	BoundingBox bbox;
	dtNavMesh * nav_mesh = nullptr;
	dtNavMeshQuery * nav_query;
	unsigned char* m_triareas = nullptr;
	rcHeightfield* m_solid = nullptr;
	rcCompactHeightfield* m_chf = nullptr;
	rcContourSet* m_cset = nullptr;
	rcPolyMesh* m_pmesh = nullptr;
	rcPolyMeshDetail* m_dmesh = nullptr;
	rcConfig m_cfg;
	CTX * m_ctx = nullptr;
	vector<rcPolyMesh*> poly_meshes;


	void InitConfig();
	void Cleanup();
	void BuildAllTiles();
	unsigned char * BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int & dataSize);

public:
	NavMesh();
	NavMesh(NavMesh &&) = default;
	~NavMesh();
	void Build();
	inline vector<rcPolyMesh*> & GetPolyMeshes() { return poly_meshes; }
};

