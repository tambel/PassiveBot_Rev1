#include "WMORoot.h"
#include "stdafx.h"



WMORoot::WMORoot(string filename):filename(filename)
{
	wmo_groups = vector<WMOGroup>();
	BinaryReader reader = BinaryReader(filename);
	MOHD header;
	if (!reader.IsFileExist())
		throw exception("Cant init WMO");
	bool hasHeader = false;
	while (!hasHeader)
	{

		unsigned signature = reader.Read<unsigned>();
		unsigned size = reader.Read<unsigned>();
		//var curPos = file.Position;
		switch (signature)
		{
		case Utils::ChunkSignatures::WMOSignature::Mohd:
			header = reader.Read<MOHD>();
			hasHeader = true;
			break;
		}
		reader.MoveForward(size);
		
	}
	LoadGroups(header);

}


WMORoot::~WMORoot()
{
}
void WMORoot::LoadGroups(MOHD & header)
{
	//skin_path = skin_path.erase(skin_path.length() - 3, 3) + "0" + to_string(index) + ".skin";
	for (unsigned i = 0; i < header.nGroups;i++)
	{
		string grop_path = filename;
		grop_path.erase(grop_path.length() - 4, 4);
		string number = to_string(i);
		while (number.length() < 3)
			number = "0" + number;
		grop_path += "_"+number + ".wmo";
		try
		{
			wmo_groups.push_back(WMOGroup(grop_path));

		}
		catch (exception e)
		{
			cout << e.what();
		}
		
	}
}
