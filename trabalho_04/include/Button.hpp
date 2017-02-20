#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "observer.hpp"

#include "TileSet.hpp"
#include "Geometry.hpp"

class Button : public Rectangle
{
SUBJECT
private:
	bool clicked;
	bool active;
	
	TileSet* tileset;
public:
	enum event_id
	{
		CLICKED = 0 // 1
	};
	
	bool camera;
	
	Button (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		TileSet* tileset = NULL,
		bool camera = false
	);
	~Button ();
	
	void update ();
	
	void render ();
	
	GameObject* clone () const;
	
	void setTileSet (TileSet* tileset);
	
	void changeState ();
private:
	void handleMouseDownLeft ();
	void handleMouseUpLeft ();
};

#endif
