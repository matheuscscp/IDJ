#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GameObject.hpp"

class Camera : public GameObject
{
private:
	static Camera* instance_;
public:
	lalge::R2Vector v;
	lalge::Scalar scroll;
private:
	Camera ();
	~Camera ();
public:
	static Camera* instance ();
	static void close ();
	
	void update ();
	
	void render ();
	
	GameObject* clone () const;
private:
	void handleKeyDown ();
	void handleKeyUp ();
};

#endif
