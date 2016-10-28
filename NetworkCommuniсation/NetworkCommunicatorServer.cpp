#include "stdafx.h"
#include "RemoteControl.h"
#include <ctime>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;


void NetworkCommunicatorServer::InitServer()
{
}


NetworkCommunicatorServer::NetworkCommunicatorServer()
{
}

NetworkCommunicatorServer::NetworkCommunicatorServer(string address, unsigned short port):
	NetworkCommunicator(address,port)
{

}


NetworkCommunicatorServer::~NetworkCommunicatorServer()
{
}

void NetworkCommunicatorServer::Start(RemoteControl * parent_rm)
{
	/*
	NetworkCommunicator::Start();
	tcp::acceptor acceptor(*io_service, endpoint);
	socket = new tcp::socket(*io_service);
	acceptor.accept(*socket);
	*/

	while (1)
	{
		try
		{
			boost::asio::io_service io_service;
			tcp::acceptor acceptor(io_service, endpoint);
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			while (1)
			{

				UnkPacket * rp = RecievePacket(socket);
				UnkPacket * sp = parent_rm->ProcessPacket(rp);
				SendPacket(socket,*sp);
				delete sp;
				sp = nullptr;
				//delete rp;

			}
		}
		catch (exception & e)
		{
			memset(buffer.data(), 0, buffer_size);
		}
	}
}

void NetworkCommunicatorServer::PacketRecievedCallback(UnkPacket & packets)
{


	
}
