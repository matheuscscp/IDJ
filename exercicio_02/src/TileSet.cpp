#include "../include/TileSet.hpp"

TileSet::TileSet (const string& filename, int tile_w, int tile_h)
{
	this->tile_w = tile_w;
	this->tile_h = tile_h;
	
	tileset = new Sprite ( filename );
	
	vtiles = NULL;
	
	dstrect = new SDL_Rect;
	
	using_single_file = true;
	
	rows = tileset->h () / tile_h;
	cols = tileset->w () / tile_w;
}

TileSet::TileSet (int rows, int cols, const string& filename)
{
	this->rows = rows;
	this->cols = cols;
	
	tileset = new Sprite ( filename );
	
	vtiles = NULL;
	
	dstrect = new SDL_Rect;
	
	using_single_file = true;
	
	tile_w = tileset->w () / cols;
	tile_h = tileset->h () / rows;
}

TileSet::TileSet (int tile_w, int tile_h)
{
	this->tile_w = tile_w;
	this->tile_h = tile_h;
	rows = 0;
	cols = 0;
	
	tileset = NULL;
	
	vtiles = new vector< Sprite* >;
	
	dstrect = NULL;
	
	using_single_file = false;
}

TileSet::~TileSet ()
{
	if ( vtiles )
	{
		for (	vector< Sprite* >::iterator it = vtiles->begin ();
			it != vtiles->end ();
			++it	)
			delete ( *it );
		
		delete vtiles;
	}
	else
	{
		delete tileset;
		delete dstrect;
	}
}

void TileSet::addTile (const string& filename)
{
	if ( vtiles )
		vtiles->push_back ( new Sprite ( filename ) );
}

void TileSet::render (int index, float posX, float posY)
{
	if ( vtiles )
		vtiles->at ( index )->render ( posX, posY );
	else
	{
		tileset->clip (	( index % cols ) * tile_w,
				( index / cols ) * tile_h,
				tile_w,
				tile_h	);
		tileset->render ( posX, posY );
	}
}

bool TileSet::usingSingleFile () const
{
	return using_single_file;
}

int TileSet::tileW () const
{
	return tile_w;
}

int TileSet::tileH () const
{
	return tile_h;
}
