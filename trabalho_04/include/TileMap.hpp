#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "TileSet.hpp"

class TileMap
{
private:
	class Layer;
	
	std::string map_path;
public:
	TileSet* tileset;
private:
	Layer** data;
	
	int map_layers;
public:
	TileMap (TileSet* tileset = NULL, const std::string& map_path = "");
	~TileMap ();
private:
	void clear ();
public:
	void load (const std::string& map_path);
	void reload ();
	
	int& at (int layer, int i, int j);
	
	void render (float cameraX, float cameraY);
	void renderLayer (int layer, float cameraX, float cameraY);
	
	int width (int layer) const;
	int height (int layer) const;
	int layers () const;
};

#endif
