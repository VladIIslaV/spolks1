#pragma once

#include <fstream>
#include <vector>

#include "Map.h"

using namespace std;


class MapMgr
{
	vector<string> mapList;
	Map map;
public:
	MapMgr();

	void CreateMap();
	void ReadMap(int mapId);
	void SaveMapToFile();

	~MapMgr();
private:
	int CreateMapList();	// create list of existed maps
};

