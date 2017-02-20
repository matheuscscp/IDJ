#ifndef TILESET_HPP
#define TILESET_HPP

#include <vector>
#include <string>

#include "Sprite.hpp"

class TileSet
{
private:
	int tile_w;
	int tile_h;
	int rows;
	int cols;
	
	Sprite* tileset;
	
	std::vector< Sprite* >* tiles;
	
	SDL_Rect* dstrect;
	
	bool using_single_file;
public:
	TileSet (const std::string& filename, int tile_w, int tile_h);
	TileSet (int rows, int cols, const std::string& filename);
	TileSet (int tile_w, int tile_h);
	~TileSet ();
	
	void addTile (const std::string& filename);
	
	void render (int index, float posX, float posY);
	
	bool usingSingleFile () const;
	
	int tileW () const;
	int tileH () const;
};

#endif
