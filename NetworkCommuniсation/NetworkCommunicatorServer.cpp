#include "stdafx.h"
#include "NetworkCommunicatorServer.h"

#include <ctime>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;


void NetworkCommunicatorServer::InitServer()
{
	auto make_daytime_string = []()
	{
		using namespace std; // For time_t, time and ctime;
		time_t now = time(0);
		return string(ctime(&now));
	};
	try
	{
		boost::asio::io_service io_service;
		boost::asio::ip::address address;
		address.from_string("127.0.0.1");
		tcp::endpoint endpoint = tcp::endpoint(address, 8001);
		tcp::acceptor acceptor(io_service, endpoint);
	
		while(1)
		{
			try
			{


				tcp::socket socket(io_service);
				acceptor.accept(socket);
				while (1)
				{
					Player * player = ObjectManager::GetPlayer();
					Vector3 p = player->GetPosition().coords;
					boost::system::error_code ignored_error;
					char data[12];
					boost::asio::write(socket, boost::asio::buffer((char*)&p, 12), boost::asio::transfer_all(), ignored_error);
					string message = make_daytime_string();
					//boost::asio::write(socket, boost::asio::buffer(message),boost::asio::transfer_all(), ignored_error);



					//boost::asio::write(socket, boost::asio::buffer(reinterpret_cast<char*>(&p),12), ignored_error);
					char b[12] = {};
					//boost::asio::read(socket, boost::asio::buffer(b));
					boost::array<char, 12> buf;
					boost::asio::read(socket, boost::asio::buffer(buf), ignored_error);
					Sleep(100);
				}
			}
			catch (...)
			{

			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


NetworkCommunicatorServer::NetworkCommunicatorServer()
{
	InitServer();
}


NetworkCommunicatorServer::~NetworkCommunicatorServer()
{
}
