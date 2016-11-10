#pragma once
using  namespace std;
class Configuration
{
	static string game_data_path;
	static string game_path;
	static bool localized;
	static bool navigation_enabled;
	
public:
	static string GetGameDataPath() {return game_data_path;}
	static string GetGamePath() { return game_path; }
	static bool IsLocalized() { return localized; }
	static bool IsNavigationEnabled() { return navigation_enabled; }

};

