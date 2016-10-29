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
	size_t read = 0;

	do
	{
		read = socket.read_some(boost::asio::buffer(buf));
		cout << "REC!!!" << read << endl;
		if (!read)
			continue;
		memcpy(buffer.data() + current_buffer_lenght, buf.data(), read);
		current_buffer_lenght += read;
		unsigned packet_size = *reinterpret_cast<unsigned*>(buffer.data() + 4);
		if (*reinterpret_cast<unsigned*>(buffer.data()) == PREAMBULE && packet_size >= current_buffer_lenght)
		{
			char * rb = new char[packet_size];
			memcpy(rb, buffer.data(), packet_size);
			rotate(buffer.begin(), buffer.begin() + packet_size, buffer.end());
			current_buffer_lenght -= packet_size; 
			//return reinterpret_cast<UnkPacket*>(rb-4);
			return rb;
		}
	} while (1);
}



bool NetworkCommunicator::SendPacket(tcp::socket & socket,void * packer_ptr, unsigned size)
{
	char * c_p = reinterpret_cast<char*>(packer_ptr);
	static_cast<PacketHeader*>(packer_ptr)->size = size;
	boost::system::error_code ignored_error;
	unsigned sent = socket.write_some(boost::asio::buffer(c_p, size), ignored_error);
	return sent == size;
}

void NetworkCommunicator::Start()
{
}


