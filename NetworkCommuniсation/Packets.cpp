#include "stdafx.h"
#include "Packets.h"

/*
Packet::Packet()
{
	size = 0;
}

Packet::Packet(vector<char> buffer)
{
	//size = *reinterpret_cast<unsigned*>(buffer + 4);
	//data = vector<char>(buffer, buffer + size);
	data = std::move(buffer);
	size = data.size();
	//data = new char[size];
	//memcpy(data,buffer, size);
}

Packet::Packet(Packet && packet)
{
	data = move(packet.data);
	size = packet.size;
	packet.size = 0;
}


Packet::~Packet()
{
}

vector<char> &  Packet::Pack()
{
	this->_Pack();
	return data;
}

vector<char>& Packet::_Pack()
{
	unsigned pl_size;
	char * c_data = this->GetPayload(pl_size);
	size = _GetSize()-4;
	//size += pl_size;

	std::copy(reinterpret_cast<char*>(&pr), reinterpret_cast<char*>(&pr) + size, std::back_inserter(data));
	//std::copy(reinterpret_cast<char*>(&pr), reinterpret_cast<char*>(&pr) + 12, std::back_inserter(data));
	//std::copy(reinterpret_cast<char*>(c_data), reinterpret_cast<char*>(c_data) + pl_size, std::back_inserter(data));
	

	return data;
	}

char * Packet::GetPayload(unsigned & size)
{
	size = sizeof(1);
	if (size)
		return reinterpret_cast<char*>(0);
}

void Packet::AddStringField(string field)
{
	const char * s = field.c_str();
	std::copy(s,s + field.length(), std::back_inserter(data));
}






PlayerPositionRequest::PlayerPositionRequest()
{
	type = 0;

}

PlayerPositionRequest::~PlayerPositionRequest()
{
}
*/
