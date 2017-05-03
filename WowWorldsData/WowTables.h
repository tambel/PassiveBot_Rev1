#pragma once
#include <string>
using namespace std;
using stringref = unsigned;

struct WorldMapAreaRec {
	stringref name;
	float left;
	float right;
	float top;
	float bottom;
	unsigned short uf1;
	unsigned short id;
	unsigned short uf3;
	unsigned short uf4;
	unsigned short uf5;
	unsigned short uf6;
	char bf1;
	char bf2;
	char bf3;
	char bf4;
	unsigned short uf7; 
	unsigned short uf8;
};

