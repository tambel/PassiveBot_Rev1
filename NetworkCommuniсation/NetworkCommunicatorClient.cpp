#include "stdafx.h"
#include "NetworkCommunicatorClient.h"
#include <iostream>




void NetworkCommunicatorClient::InitClient()
{

	try
	{
		io_service = new boost::asio::io_service;
		tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8001);
		boost::asio::ip::address address;
		tcp::resolver resolver(*io_service);
		//tcp::resolver::query query(ep);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(ep);

		socket=new tcp::socket(*io_service);
		boost::asio::connect(*socket, endpoint_iterator);

		//for (;;)
		//{
		//	boost::array<char, 128> buf;
		//	boost::system::error_code error;

		//	size_t len = socket.read_some(boost::asio::buffer(buf), error);

		//	if (error == boost::asio::error::eof)
		//		break; // Connection closed cleanly by peer.
		//	else if (error)
		//		throw boost::system::system_error(error); // Some other error.

		//	std::cout.write(buf.data(), len);
		//}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}


}

NetworkCommunicatorClient::NetworkCommunicatorClient()
{
	InitClient();
}


NetworkCommunicatorClient::~NetworkCommunicatorClient()
{
}

void NetworkCommunicatorClient::WaitForMessage()
{
	try
	{

		boost::array<char, 12> buf;
		boost::system::error_code error;
		size_t len = socket->read_some(boost::asio::buffer(buf), error);
		//size_t len = socket->read_some(boost::asio::buffer(buf), error);


		//std::cout.write(buf.data(), len);
		player_position = *reinterpret_cast<Vector3*>(buf.data());
		cout << player_position.x << " " << player_position.y << endl;
		socket->write_some(boost::asio::buffer("CLIENT"), error);


	}
	catch (...)
	{

	}
}
