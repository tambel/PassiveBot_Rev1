#include "stdafx.h"


WowObjectAvatar::WowObjectAvatar(Wow::WowObject * wow_object): wow_object(wow_object)
{

	Position pos=wow_object->GetPosition();
	pos.coords=Vector3(Metrics::MapMidPoint - pos.coords.y,-(Metrics::MapMidPoint - pos.coords.x),pos.coords.z);
	//M2 * m2 = new M2();
	doodad=new Doodad("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2",0,pos,1024);
	//Doodad * doodad=new Doodad(m2,0,object->GetPosition(),1024);
	//real_position=Vector3(Metrics::MapMidPoint - header.position.y,-(Metrics::MapMidPoint - header.position.x),header.position.z);
	doodad->SetPosition(pos);
	for (unsigned long i=0;i<doodad->GetVertexCount();i++)
	{
		//doodad->GetVertices()[i].position.x*=100;
		//doodad->GetVertices()[i].position.y*=100;
		//doodad->GetVertices()[i].position.z*=100;
	}
	//delete m2;
}


WowObjectAvatar::~WowObjectAvatar(void)
{
}
Position WowObjectAvatar::GetPosition()
{
	Position pos=wow_object->GetPosition();
	pos.coords=Vector3(Metrics::MapMidPoint - pos.coords.y,-(Metrics::MapMidPoint - pos.coords.x),pos.coords.z);
	doodad->SetPosition(pos);
	return doodad->GetPosition();
}
