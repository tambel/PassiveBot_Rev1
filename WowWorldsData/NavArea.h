#pragma once
#include "SquareArea.h"
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
public:
	NavArea();
	NavArea(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius);
	~NavArea();
	NavArea & operator=(NavArea && right);
	virtual  void Fill(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	virtual void CheckAndMove(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);


	static NavConfig config;
	vector<unique_ptr<rcPolyMesh>> polys;
	dtNavMeshQuery* m_navQuery;
	dtNavMesh * m_navMesh;
	//unique_ptr<PATHDATA>  m_PathStore;
	const static int MAX_SMOOTH = 2048;
	float m_smoothPath[MAX_SMOOTH * 3];
	int m_nsmoothPath;

	void InitNavigation();

	void BuildAllTiles();
	unsigned char * BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int dataSize);
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
};

