#include "Button.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

using namespace lalge;

Button::Button (
	const R2Vector& r,
	const Scalar& depthconst,
	TileSet* tileset,
	bool camera
) : Rectangle ( r, depthconst, 0, 0, 0 ), clicked ( false ), active ( true ),
tileset ( tileset ), camera ( camera )
{
	subject.init ( 1 );
	
	InputManager::instance ()->connect (
		InputManager::MOUSEDOWN_LEFT,
		this,
		&Button::handleMouseDownLeft
	);
	
	InputManager::instance ()->connect (
		InputManager::MOUSEUP_LEFT,
		this,
		&Button::handleMouseUpLeft
	);
	
	if ( tileset )
	{
		w_ = tileset->tileW();
		h_ = tileset->tileH();
	}
}

Button::~Button ()
{
	InputManager::instance ()->disconnect ( this );
}

void Button::update ()
{
}

void Button::render ()
{
	if ( ( tileset ) && ( active ) )
	{
		int index, x, y;
		
		if ( clicked )
		{
			clicked = false;
			index = 2;
		}
		else if ( !mouseInside () )
			index = 0;
		else if ( !InputManager::instance ()->mousePressed ( SDL_BUTTON_LEFT ) )
			index = 1;
		else if ( mouseDownInside () )
			index = 2;
		
		x = r.x ( 0 ) - Camera::r.x ( 0 ) * depthconst * camera;
		y = r.x ( 1 ) - Camera::r.x ( 1 ) * depthconst * camera;
	
		tileset->render ( index, x, y );
	}
}

GameObject* Button::clone () const
{
	return new Button ( *this );
}

void Button::setTileSet (TileSet* tileset)
{
	this->tileset = tileset;
	
	if ( tileset )
	{
		w_ = tileset->tileW ();
		h_ = tileset->tileH ();
	}
}

void Button::changeState ()
{
	active = ( !active );
}

void Button::handleMouseDownLeft ()
{
	if ( ( mouseDownInside () ) && ( active ) )
		clicked = true;
}

void Button::handleMouseUpLeft ()
{
	if ( ( mouseDownInside () ) && ( mouseInside () ) && ( active ) )
	{
		clicked = true;
		subject.broadcast ( observer::Event ( CLICKED ) );
	}
}
