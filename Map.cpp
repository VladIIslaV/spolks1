#include "Map.h"


Map::Map() : 
	logFile (ofstream())
{
	map = vector<vector<uint8_t>>();
	for (auto item : map)
	{
		item = vector<uint8_t>();
	}
}


/*
	X X X X X
	Y Y Y Y Y
	Y Y Y Y Y
	Y   Y Y
	Y     Y
	Y 
*/
void Map::Resize(int X, int Y)
{
	map.resize(X);
	for (auto &item : map)
	{
		item.resize(Y);
	}
}

void Map::Save(const char* fileName)
{
	uint16_t *arr = new uint16_t[10000];
	for (int i = 0; i < 10000; i++)
	{
		arr[i] = i;
	}
	//ofstream mapFile(fileName);
	try 
	{
		//img.write("IMAGE.PNG");
		/*
		Geometry imageGeom = Geometry(100, 100, 10, 20);
		Blob imageBlob = Blob(arr, 10000);
		Image newI = Image(imageBlob, imageGeom);
		newI.write(fileName);*/
	}
	catch (exception &error_)
	{
		cout << "Caught exception: " << error_.what() << endl;
	}
    delete arr;
    arr = nullptr;
	/*
	for (auto item : map)
	{
		for (auto brick : item)
		{
			mapFile << brick;
		}
		mapFile << endl;
	}*/
	//mapFile.close();
}

void Map::Show()
{
	for (auto item : map)
	{
		for (auto brick : item)
		{
			cout << (char)brick << " ";
		}
		cout << endl;
	}
}

uint8_t Map::GetBrick(int x, int y) const
{
	return map[y][x];
}

void Map::SetBrick(int x, int y, uint8_t brick)
{
	map[y][x] = brick;
}

void Map::RandomFill(uint8_t min, uint8_t max)
{
	for (auto &item : map)
	{
		for (auto &brick : item)
		{
			brick = rand()%(max-min) + min;
		}
	}
	cout << "Map filled by random values." << endl;
}

void Map::InitializeLog(const char* name)
{
	logFile.open(name);
	logFile << __DATE__ << "\t" << __TIME__ << endl << "Distance" << endl;
}

void Map::WriteLog(int dist)
{
	logFile << dist << endl;
}

void Map::CloseLog()
{
	logFile << endl << "LOG FINISHED" << endl;
	logFile << __DATE__ << "\t" << __TIME__ << endl;
	logFile.close();
}



Map::~Map()
{
	for (auto &item : map)
	{
		item.clear();
	}
	map.clear();
}
