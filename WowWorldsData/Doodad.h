#pragma once
#include <string>
#include "M2.h"
#include "Model.h"
#include "Utils.h"
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include "MapObject.h"
using namespace std;
class M2;
class Doodad:public Model,public MapObject
{
	unsigned long uuid;
public:
	//Vector3 game_position;
	float scale;
	Doodad(void){};
	Doodad(Doodad && right);
	Doodad(string filename, unsigned uuid, Position position, short scale );
	~Doodad(void);
	unsigned long GetUUID() {return uuid;}
	//Vector3 GetGamePosition() {return game_position;}
	Doodad & operator=(Doodad && right);
};
