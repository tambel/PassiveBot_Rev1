#pragma once
using  namespace std;
class Configuration
{
	static string game_data_path;
	static bool localized;
public:
	static string GetGameDataPath() {return game_data_path;}
	static bool IsLocalized() { return localized; }

};

