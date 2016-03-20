#pragma once
#include "Area.h"
struct NavConfig
{
	int m_maxTiles;
	int m_maxPolysPerTile;
	float m_tileSize;
	float m_cellSize;
	float m_cellHeight;
	float m_agentHeight;
	float m_agentRadius;
	float m_agentMaxClimb;
	float m_agentMaxSlope;
	float m_regionMinSize;
	float m_regionMergeSize;
	float m_edgeMaxLen;
	float m_edgeMaxError;
	float m_vertsPerPoly;
	float m_detailSampleDist;
	float m_detailSampleMaxError;
	int m_partitionType;
};
class NavArea :
	public Area
{
protected:
	virtual void Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	void _move(NavArea & other);
public:
	NavArea();
	NavArea(int radius);
	//NavArea(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius);
	~NavArea();
	NavArea & operator=(NavArea && right);
	NavArea(NavArea && area);
	//virtual  void Fill(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	//virtual void CheckAndMove(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	//virtual void CheckAndMoveImpl(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	void InitAreaBoundingBox();

	static NavConfig config;
	//vector<unique_ptr<rcPolyMesh>> polys;
	dtNavMeshQuery* m_navQuery= nullptr;
	dtNavMesh * m_navMesh=nullptr;
	rcPolyMesh *  m_pmesh=nullptr;
	rcPolyMeshDetail * m_dmesh=nullptr;
	rcHeightfield * m_solid=nullptr;
	rcCompactHeightfield * m_chf=nullptr;
	rcContourSet * m_cset = nullptr;
	rcContext * m_ctx;
	//unique_ptr<PATHDATA>  m_PathStore;
	static int MAX_SMOOTH;// = 2048;
	float m_smoothPath[2048 * 3];
	int m_nsmoothPath=0;

	void InitNavigation();
	void CleanUp();

	void BuildAllTiles();
	unsigned char * BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int & dataSize);
	int  FindPath(Vector3 & start, Vector3  & end, int nPathSlot);
	static void InitNavConfig();

	static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
		const float minTargetDist,
		const dtPolyRef* path, const int pathSize,
		float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
		float* outPoints = 0, int* outPointCount = 0);
	inline static  bool inRange(const float* v1, const float* v2, const float r, const float h);
	static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
		const dtPolyRef* visited, const int nvisited);
	static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery);
	static int  RastChunks(Model * model, rcConfig & m_cfg, rcContext * m_ctx, rcHeightfield * m_solid, int & m_tileTriCount);
};

