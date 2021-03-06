#include <fstream>

#include "simplestructures.hpp"

#include "TileMap.hpp"

using std::fstream;
using std::string;

class TileMap::Layer
{
private:
	int** tile_matrix;
	
	int w_;
	int h_;
	
	void alloc ()
	{
		tile_matrix = new int* [ h_ ];
		
		for ( int i = 0; i < h_; ++i )
			tile_matrix[i] = new int [ w_ ];
	}
public:
	Layer (fstream& f)
	{
		f >> w_;
		f.get ();
		f >> h_;
		f.get ();
		
		alloc ();
		
		for ( int i = 0; i < h_; ++i )
		{
			for ( int j = 0; j < w_; ++j )
			{
				f >> tile_matrix[i][j];
				f.get ();
				
				--tile_matrix[i][j];
			}
		}
	}
	
	Layer (int w, int h) : w_ ( w ), h_ ( h )
	{
		alloc ();
	}
	
	~Layer ()
	{
		for ( int i = 0; i < h_; ++i )
			delete[] tile_matrix[i];
		
		delete[] tile_matrix;
	}
	
	int& at (int i, int j)
	{
		return tile_matrix[i][j];
	}
	
	void render (TileSet* tileset, float cameraX, float cameraY)
	{
		for ( int i = 0; i < h_; ++i )
		{
			for ( int j = 0; j < w_; ++j )
			{
				if ( tile_matrix[i][j] >= 0 )
				{
					tileset->render (
						tile_matrix[i][j],
						( j * tileset->tileW () ) - cameraX,
						( i * tileset->tileH () ) - cameraY
					);
				}
			}
		}
	}
	
	int w () const
	{
		return w_;
	}
	
	int h () const
	{
		return h_;
	}
	
	Layer* clone () const
	{
		Layer* ret = new Layer ( w_, h_ );
		
		for ( int i = 0; i < h_; ++i )
		{
			for ( int j = 0; j < w_; ++j )
				ret->at ( i, j ) = tile_matrix[i][j];
		}
		
		return ret;
	}
};

TileMap::TileMap (TileSet* tileset, const string& map_path) :
tileset ( tileset ), data ( NULL ), map_layers ( 0 )
{
	load ( map_path );
}

TileMap::~TileMap ()
{
	clear ();
}

void TileMap::clear ()
{
	if ( data )
	{
		for ( int i = 0; i < map_layers; ++i )
			delete ( data[i] );
		
		delete[] data;
		
		data = NULL;
	}
}

void TileMap::load (const string& map_path)
{
	this->map_path = map_path;
	
	reload ();
}

void TileMap::reload ()
{
	clear ();
	
	if ( map_path.size () > 0 )
	{
		fstream f ( map_path.c_str () );
		
		f >> map_layers;
		f.get ();
		
		data = new Layer* [ map_layers ];
		
		for ( int i = 0; i < map_layers; ++i )
			data[i] = new Layer ( f );
		
		f.close ();
	}
}

int& TileMap::at (int layer, int i, int j)
{
	if ( ( !data ) || ( layer >= map_layers ) )
		throw ( mexception ( "Trying to access non-allocated tilemap layer" ) );
	
	return data[ layer ]->at ( i, j );
}

void TileMap::render (float cameraX, float cameraY)
{
	if ( ( tileset ) && ( data ) )
	{
		for ( int i = 0; i < map_layers; ++i )
			data[i]->render ( tileset, cameraX, cameraY );
	}
}

void TileMap::renderLayer (int layer, float cameraX, float cameraY)
{
	if ( ( tileset ) && ( data ) )
		data[ layer ]->render ( tileset, cameraX, cameraY );
}

int TileMap::width (int layer) const
{
	if ( ( !data ) || ( layer >= map_layers ) )
		throw ( mexception ( "Trying to access non-allocated tilemap layer" ) );
	
	return data[ layer ]->w ();
}

int TileMap::height (int layer) const
{
	if ( ( !data ) || ( layer >= map_layers ) )
		throw ( mexception ( "Trying to access non-allocated tilemap layer" ) );
	
	return data[ layer ]->h ();
}

int TileMap::layers () const
{
	return map_layers;
}
