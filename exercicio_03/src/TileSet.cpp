#include "../include/TileSet.hpp"

using namespace std;

TileSet::TileSet (const string& filename, int tile_w, int tile_h)
{
	this->tile_w = tile_w;
	this->tile_h = tile_h;
	
	tileset = new Sprite ( filename );
	
	tiles = NULL;
	
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
	
	tiles = NULL;
	
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
	
	tiles = new vector< Sprite* >;
	
	dstrect = NULL;
	
	using_single_file = false;
}

TileSet::~TileSet ()
{
	if ( tiles )
	{
		for (
			vector< Sprite* >::iterator it = tiles->begin ();
			it != tiles->end ();
			++it
		)
		{
			delete ( *it );
		}
		
		delete tiles;
	}
	else
	{
		delete tileset;
		delete dstrect;
	}
}

void TileSet::addTile (const string& filename)
{
	if ( tiles )
		tiles->push_back ( new Sprite ( filename ) );
}

void TileSet::render (int index, float posX, float posY)
{
	if ( tiles )
		(*tiles)[ index ]->render ( posX, posY );
	else
	{
		tileset->clip (
			( index % cols ) * tile_w,
			( index / cols ) * tile_h,
			tile_w,
			tile_h
		);
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
