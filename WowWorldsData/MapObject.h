#pragma once
#include <string>
using  namespace std;
class MapObject
{
protected:
	
	static unsigned  lifetime;
	unsigned unused_iter_count=0;
	bool refreshed=true;
	string name;
	void swap(MapObject & other);
	bool occupied = false;
public:
	MapObject();
	MapObject(string  filename);
	~MapObject();
	MapObject(MapObject && other);
	MapObject & operator=(MapObject && right);
	inline unsigned GetUnusedIterCount() { return unused_iter_count; }
	void HitUnuseed();
	inline bool IsAlive() { return unused_iter_count < lifetime; }
	void Refresh();
	inline void Occupie() { occupied = true; }
	inline void Free() { occupied = false; }
	inline bool  IsOccupied() { return occupied; }
	inline string & GetName() { return name; }
	//inline bool IsRefreshed() {  return  refreshed}
};

