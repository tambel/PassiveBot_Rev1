// PassiveBot.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
#include <thread>
#include <random>
#include "WowInteraction\DBCReader.h"
#include <fstream>
#include "NetworkCommuniсation\NetworkCommunicatorServer.h"
using namespace std;
using namespace Utils;
using namespace Tools;


int bot_activity()
{
	while (1)
	{
		cout << "WORKING" << endl;
	}
	return 0;
}

int main(int argc, wchar_t * argv[])
{
	Sleep(5000);
	BotInteractor::StartGame("arttambel@gmail.com", "archi911", L"Testintauenr", bot_activity);
	return 0;

}

