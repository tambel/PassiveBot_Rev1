#pragma once
#include <exception>
#include <iostream>
#include <string>
using namespace std;
namespace Tools
{
	void print_exception(const std::exception& e, int level = 0);

}