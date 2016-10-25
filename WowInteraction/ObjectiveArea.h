#pragma once


using   QuestAreaPoint = int[2];

class ObjectiveArea
{
public:
	unsigned points_count = 0;
	QuestAreaPoint * points = nullptr;
	ObjectiveArea();
	~ObjectiveArea();
	bool IsInArea(Point2D<float> point);

};