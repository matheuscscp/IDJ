#ifndef GAMESTATES_HPP
#define GAMESTATES_HPP

#include <list>

#include "linearalgebra.hpp"

#include "State.hpp"
#include "Audio.hpp"
#include "Animation.hpp"
#include "Planet.hpp"
#include "FollowerObject.hpp"
#include "AccObject.hpp"
#include "TileMap.hpp"
#include "Timer.hpp"

// game states
enum
{
	STATEQUIT = -1,
	NOSTATE,
	STATESPLASH,
	STATEGAME,
	STATEWINLOSE
};

// first game state
class StateSplash : public State
{
private:
	Audio* bgm;
public:
	void load(MainArgs* args, StateArgs* st_args = 0);
	StateArgs* unload();
	
	int input();
	int update();
	void render();
private:
	void renderMenu();
	
	void handleQuit();
	void handleKeyDown();
};

class StateGame : public State
{
private:
	Audio* bgm;
	Audio* sfx;
	
	Sprite* spr_bg;
	Sprite* spr_redplanet;
	Sprite* spr_earth;
	Sprite* spr_moon;
	Sprite* spr_ufo;
	
	Animation* anim_ship;
	Animation* anim_shipturn;
	Animation* anim_boom;
	
	lalge::R2Vector boom_r;
	
	std::list< Planet* > planets;
	
	Planet* earth;
	Planet* moon;
	
	FollowerObject* ufo;
	
	AccObject* ship;
	
	TileSet* tileset;
	TileMap* tilemap;
	
	Timer gameover;
	Timer newplanet;
public:
	void load(MainArgs* args, StateArgs* st_args = 0);
	StateArgs* unload();
	
	int input();
	int update();
	void render();
private:
	void handleQuit();
	void handleKeyDown();
	void addPlanet();
	
	void showFPS();
	
	void checkCollision();
	void checkGameOver();
};

class StateWinLose : public State
{
private:
	Audio* bgm;
public:
	void load(MainArgs* args, StateArgs* st_args = 0);
	StateArgs* unload();
	
	int input();
	int update();
	void render();
private:
	void renderMenu(StateArgs* st_args);
	
	void handleQuit();
	void handleKeyDown();
};

#endif
