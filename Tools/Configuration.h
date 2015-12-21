#pragma once
#include <string>
using  namespace std;
class Configuration
{
	static string game_data_path;
public:
	static string GetGameDataPath() {return game_data_path;}

};

