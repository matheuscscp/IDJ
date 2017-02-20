#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "mod/observer.hpp"

#include "TileSet.hpp"
#include "GameObject.hpp"

class Button : public GameObject
{
SUBJECT
private:
	bool clicked;
	bool active;
public:
	enum event_id
	{
		CLICKED = 0 // 1
	};
	
	TileSet* tileset;
	
	bool use_camera;
	
	Button (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		TileSet* tileset = NULL,
		bool use_camera = false
	);
	~Button ();
	
	void update ();
	
	void render ();
	
	GameObject* clone () const;
	
	void switch_state ();
private:
	bool mouseInside () const;
	bool mouseDownInside () const;
	void handleMouseDownLeft ();
	void handleMouseUpLeft ();
};

#endif
