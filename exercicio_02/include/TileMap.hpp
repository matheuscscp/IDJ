#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <string>
#include <fstream>

#include "TileSet.hpp"

using std::vector;
using std::string;
using std::fstream;

class TileMap
{
private:
	vector< vector< vector< int > > > tile_matrix;
	
	TileSet* tileset;
	
	int map_w;
	int map_h;
	int map_layers;
public:
	TileMap (int map_w, int map_h, int layers = 1, TileSet* tileset = NULL);
	TileMap (const string& map_path, TileSet* tileset = NULL);
	~TileMap ();
private:
	void resize ();
public:
	void load (const string& map_path);
	
	void setTileSet (TileSet* tileset);
	
	int& at (int i, int j, int k = 0);
	
	void render (float cameraX = 0, float cameraY = 0);
private:
	void renderLayer_ (int layer, float cameraX = 0, float cameraY = 0);
public:
	void renderLayer (int layer, float cameraX = 0, float cameraY = 0);
	
	int width () const;
	int height () const;
	int layers () const;
};

#endif
