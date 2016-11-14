#pragma once

#define area_type Area

class MutableArea :public area_type
{
public:
	//mutex data_mutex;
	mutex data_mutex;
	bool to_update;
	MutableArea() ;
	MutableArea(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius);
	~MutableArea() {}
	MutableArea & operator=(MutableArea && other);
	virtual void Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);

};

