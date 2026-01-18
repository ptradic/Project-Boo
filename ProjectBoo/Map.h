#pragma once
#include <string>
using namespace std;

class Map {

public:

	Map(const char* mfp, int ms, int ts);
	~Map();

	void LoadMap(string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xpos, int ypos);

private:

	const char* mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};