#pragma once
#include "boost\array.hpp"
#include "WowInteraction\WowInteraction.h"
#define PREAMBULE 0xDEADBEEF



struct PacketHeader
{
	//const unsigned preambule = PREAMBULE;
	unsigned size;
	unsigned type;
	//PacketHeader(unsigned type) :type(type) {}

};

template<unsigned type_number>
struct TypedPacket :PacketHeader
{
	TypedPacket() :PacketHeader(type_number) {}
};
template<unsigned type_number>
using RequestPacket = TypedPacket<type_number>;

struct UnkPacket :TypedPacket<0>
{
};

//struct RequestPacket :PacketHeader
//{
//	RequestPacket(unsigned type_number) :PacketHeader(type_number) {}
//};


#pragma pack(push, 1)
struct SmallStringField
{
	unsigned length;
	char str[256];
};
#pragma pack(pop)


struct PlayerPositionReply
{
	PacketHeader header;
	Vector3 position;
	Vector3 rotation;
};
#pragma pack(push, 1)
struct TargetObjectInfoReply
{
	PacketHeader header;
	Guid128 guid;
	char type;
	Position position;
	SmallStringField name;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct BasePacket
{
	PacketHeader header;
};

struct BoolRequestResult:BasePacket
{
	unsigned result;
};



#pragma pack(pop)