#pragma once
#include <string>
#include "M2.h"
#include "Model.h"
#include "Utils.h"
using namespace std;
class Doodad:public Model
{
	unsigned long uuid;
public:
	Vector3 game_position;
	float scale;
	//Vector3 position;
	Vector3 orientation;
	Doodad(void){};
	Doodad(M2 * m2, unsigned uuid, Position position, short scale );
	~Doodad(void);
	unsigned long GetUUID() {return uuid;}
	Vector3 GetGamePosition() {return game_position;}
};
