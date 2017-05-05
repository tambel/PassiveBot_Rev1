#pragma once
#include "WowWorldsData\WowWorldsData.h" 

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

class CTX2 :public rcContext
{
public:
	CTX2() :rcContext() {}
	virtual void doLog(const rcLogCategory category, const char* msg, const int len)
	{
		cout << msg << endl;
	}
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};



class GlobalNavMesh
{
	BoundingBox bbox;
	NavConfig config;
	dtNavMesh * nav_mesh;
	dtNavMeshQuery * m_navQuery;
	Vector3 nav_mesh_offset;

	unsigned char* m_triareas=nullptr;
	rcHeightfield* m_solid=nullptr;
	rcCompactHeightfield* m_chf = nullptr;
	rcContourSet* m_cset = nullptr;
	rcPolyMesh* m_pmesh = nullptr;
	rcPolyMeshDetail* m_dmesh = nullptr;
	rcConfig m_cfg;
	CTX2 * m_ctx = nullptr;
	vector<rcPolyMesh*> poly_meshes;
	vector<Vector3> last_path_points;
	Area * area;


	unsigned char * BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int & dataSize);
	unsigned char * BuildTileMesh(Model * model, int x, int y, const float* bmin, const float* bmax, int & dataSize);
	void Cleanup();

public:
	GlobalNavMesh();
	~GlobalNavMesh();
	void InitConfig();
	void Start(int x, int y);
	void AddTile(Point2D<int> coordinates, Point2D<int> block_coordinates);
	void AddArea(Point2D<float> start, Point2D<float> end);
    dtNavMesh * GetNavMesh() { return nav_mesh; }
	inline vector<Vector3> & GetLastPath() { return last_path_points; }
	inline vector<rcPolyMesh*> & GetPolyMeshes() { return poly_meshes; }
	bool FindPath(Vector3 start, Vector3 end, bool tf=true);
	inline Vector3 GetNavMeshOffset() { return nav_mesh_offset; }
	bool Save();
	bool Load();
};

