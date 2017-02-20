#ifndef TILESET_HPP
#define TILESET_HPP

#include <vector>
#include <string>

#include "Sprite.hpp"

using std::vector;
using std::string;

class TileSet
{
private:
	int tile_w;
	int tile_h;
	int rows;
	int cols;
	
	Sprite* tileset;
	
	vector< Sprite* >* vtiles;
	
	SDL_Rect* dstrect;
	
	bool using_single_file;
public:
	TileSet (const string& filename, int tile_w, int tile_h);
	TileSet (int rows, int cols, const string& filename);
	TileSet (int tile_w, int tile_h);
	~TileSet ();
	
	void addTile (const string& filename);
	
	void render (int index, float posX, float posY);
	
	bool usingSingleFile () const;
	
	int tileW () const;
	int tileH () const;
};

#endif
