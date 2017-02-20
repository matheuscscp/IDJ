#include "Button.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

using namespace lalge;

Button::Button (
	const R2Vector& r,
	const Scalar& depthconst,
	TileSet* tileset,
	bool use_camera
) : GameObject ( r, depthconst ), clicked ( false ), active ( true ),
tileset ( tileset ), use_camera ( use_camera )
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
		
		x = r.x ( 0 ) - Camera::r.x ( 0 ) * depthconst * use_camera;
		y = r.x ( 1 ) - Camera::r.x ( 1 ) * depthconst * use_camera;
	
		tileset->render ( index, x, y );
	}
}

GameObject* Button::clone () const
{
	return new Button ( *this );
}

void Button::switch_state ()
{
	active = ( !active );
}

bool Button::mouseInside () const
{
	SDL_Rect rect;
	
	rect.x = r.x ( 0 ) - Camera::r.x ( 0 ) * depthconst * use_camera;
	rect.y = r.x ( 1 ) - Camera::r.x ( 1 ) * depthconst * use_camera;
	rect.w = tileset->tileW ();
	rect.h = tileset->tileH ();
	
	return InputManager::instance ()->mouseInside ( rect );
}

bool Button::mouseDownInside () const
{
	SDL_Rect rect;
	
	rect.x = r.x ( 0 ) - Camera::r.x ( 0 ) * depthconst * use_camera;
	rect.y = r.x ( 1 ) - Camera::r.x ( 1 ) * depthconst * use_camera;
	rect.w = tileset->tileW ();
	rect.h = tileset->tileH ();
	
	return InputManager::instance ()->mouseDownInside ( rect );
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
