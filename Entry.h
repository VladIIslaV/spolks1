#pragma once
#include <string>

using namespace std;

class Entry
{
protected:
	string name;
public:
	Entry();
	Entry(string name) : name(name) {};
	void ChangeName(string name) { this->name = name; };
	virtual ~Entry();
};

