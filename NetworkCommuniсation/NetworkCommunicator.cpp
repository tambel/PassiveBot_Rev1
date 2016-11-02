#include "stdafx.h"
#include "NetworkCommunicator.h"





NetworkCommunicator::NetworkCommunicator()
{

}

NetworkCommunicator::NetworkCommunicator(string address, unsigned short port):
	address(move(address)),port(port)
{
	endpoint = tcp::endpoint(boost::asio::ip::address().from_string(this->address), this->port);
	buffer_size = 1000;
	buffer = std::array<char, 1000>();
}

NetworkCommunicator::~NetworkCommunicator()
{
}

char * NetworkCommunicator::RecievePacket(tcp::socket & socket)
{
	std::array<char, 1000> buf;
	unsigned * ubuffer_ptr;
	bool is_begin = true;
	auto _read = [](tcp::socket & socket, std::array<char, 1000>& buf, unsigned size)
	{
		size_t read = 0;
		boost::system::error_code ec;
		read = boost::asio::read(socket, boost::asio::buffer(buf), boost::asio::transfer_exactly(size), ec);
		if (ec)
		{
			return false;
		}
		return true;
	};
	unsigned size = 0;
	if (!_read(socket, buf, 4))
		throw exception("Cant read packet");
	size = *reinterpret_cast<unsigned*>(buf.data());
	char * rb = new char[size];
	std::memcpy(rb, buf.data(), 4);
	if (!_read(socket, buf, size - 4))
	{
		delete[] rb;
		throw exception("Cant read packet");
	}
	std::memcpy(rb + 4, buf.data(), size - 4);
	return rb;
}



void NetworkCommunicator::SendPacket(tcp::socket & socket,void * packer_ptr, unsigned size)
{

	char * c_p = reinterpret_cast<char*>(packer_ptr);
	boost::system::error_code ec;
	boost::asio::write(socket, boost::asio::buffer(c_p, size), boost::asio::transfer_exactly(size), ec);
	if (ec)
		throw exception("Cant read packet");
	
}

void NetworkCommunicator::Start()
{
}


