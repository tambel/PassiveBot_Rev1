#pragma once
#include "boost\asio.hpp"
#include "Packets.h"
#include <string>
using namespace std;
using boost::asio::ip::tcp;



class NetworkCommunicator
{
protected:
	//boost::asio::io_service * io_service;
	tcp::endpoint endpoint;
	std::array<char, 1000>  buffer;
	unsigned buffer_size;
	unsigned current_buffer_lenght = 0;
	//tcp::socket * socket;
	string address;
	unsigned short port;

public:
	NetworkCommunicator();
	NetworkCommunicator(string address, unsigned short port);
	~NetworkCommunicator();

	char *  RecievePacket(tcp::socket & socket);
	template <typename T>
	bool SendPacket(tcp::socket & socket,T & packet);
	bool SendPacket(tcp::socket & socket,void * packer_ptr, unsigned size);
	void Start();
};

template<typename T>
inline bool NetworkCommunicator::SendPacket(tcp::socket & socket,T & packet)
{
	char * c_p = reinterpret_cast<char*>(&packet);
	unsigned size = sizeof(T);
	dynamic_cast<PacketHeader*>(&packet)->size = size;
	boost::system::error_code ignored_error;
	unsigned sent = socket.write_some(boost::asio::buffer(c_p,size), ignored_error);
	return sent == size;
}
