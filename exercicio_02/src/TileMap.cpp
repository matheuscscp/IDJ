#include "../include/TileMap.hpp"

TileMap::TileMap (	int map_w, int map_h, int layers,
			TileSet* tileset	)
{
	this->tileset = tileset;
	
	this->map_w = map_w;
	this->map_h = map_h;
	this->map_layers = layers;
	
	resize ();
}

TileMap::TileMap (const string& map_path, TileSet* tileset)
{
	this->tileset = tileset;
	
	map_w = 0;
	map_h = 0;
	map_layers = 0;
	
	load ( map_path );
}

TileMap::~TileMap ()
{
}

void TileMap::resize ()
{
	tile_matrix.resize (
		map_h,
		vector< vector< int > > (
			map_w,
			vector< int > ( map_layers, -1 )
		)
	);
}

void TileMap::load (const string& map_path)
{
	fstream f ( map_path.c_str () );
	
	f >> map_w;
	f.get ();
	f >> map_h;
	f.get ();
	f >> map_layers;
	f.get ();
	
	resize ();
	
	for ( int k = 0; k < map_layers; ++k )
	{
		for ( int i = 0; i < map_h; ++i )
		{
			for ( int j = 0; j < map_w; ++j )
			{
				f >> tile_matrix[i][j][k];
				f.get ();
				
				tile_matrix[i][j][k]--;
			}
		}
	}
	
	f.close ();
}

void TileMap::setTileSet (TileSet* tileset)
{
	this->tileset = tileset;
}

int& TileMap::at (int i, int j, int k)
{
	return tile_matrix[i][j][k];
}

void TileMap::render (float cameraX, float cameraY)
{
	if ( tileset )
	{
		for ( int k = 0; k < map_layers; ++k )
			renderLayer_ ( k, cameraX, cameraY );
	}
}

void TileMap::renderLayer_ (int layer, float cameraX, float cameraY)
{
	float x, y;
	
	for ( int i = 0; i < map_h; ++i )
	{
		for ( int j = 0; j < map_w; ++j )
		{
			if ( tile_matrix[i][j][ layer ] >= 0 )
			{
				x = cameraX + ( j * tileset->tileW () );
				y = cameraY + ( i * tileset->tileH () );
				
				tileset->render (
					tile_matrix[i][j][ layer ], x, y
				);
			}
		}
	}
}

void TileMap::renderLayer (int layer, float cameraX, float cameraY)
{
	if ( tileset )
		renderLayer_ ( layer, cameraX, cameraY );
}

int TileMap::width () const
{
	return map_w;
}

int TileMap::height () const
{
	return map_h;
}

int TileMap::layers () const
{
	return map_layers;
}
