#pragma once
class WMOPart:public Model<unsigned short>
{
public:
	WMOPart(WMOGroup & group);
	~WMOPart();
};

