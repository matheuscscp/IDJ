/// @defgroup	MOD_GAMEMANAGER "Game Manager"
/// @{
/// 	@brief Module to declare and implement the GameManager class
/// @}
/// @ingroup MOD_GAMEMANAGER
/// @file GameManager.hpp
/// @brief Declarations of all methods of the GameManager class
/// @author Matheus Pimenta

#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <list>

#include "mod/configfile.hpp"
#define SIMPLES_NOCOUT
#include "mod/simplestructures.hpp"

#include "Animation.hpp"
#include "TileMap.hpp"
#include "Planet.hpp"
#include "FollowerObject.hpp"
#include "AccObject.hpp"

/// The main controller class. It will initialize SDL, hold sprites of the game
/// and control the main game loop.
/// @brief Main game controller class
class GameManager
{
private:
	MainParam args;
	
	bool quit;
	
	/// @brief Configuration for SDL initalization values
	Configuration SDL_conf;
	
	/// @brief Configuration for sprite image file paths
	Configuration spr_conf;
	
	/// @brief Sprite for the background image
	Sprite spr_bg;
	
	/// @brief Sprite for red planets
	Sprite spr_redplanet;
	
	/// @brief Sprite for the Earth
	Sprite spr_earth;
	
	/// @brief Sprite for the Moon
	Sprite spr_moon;
	
	/// @brief Sprite for the follower object
	Sprite spr_ufo;
	
	Animation* ship_anim;
	Animation* ship_turn;
	
	Animation* boom;
	lalge::R2Vector boom_r;
	
	/// @brief Set of all current planets
	std::list< Planet* > planets;
	
	Planet* earth;
	Planet* moon;
	
	FollowerObject* ufo;
	
	AccObject* ship;
	
	TileSet* tileset;
	TileMap* tilemap;
public:
	/// This constructor initalizes SDL e loads the sprites.
	/// @brief Initializing constructor
	GameManager (const MainParam& args);
	
	/// This destructor closes SDL systems.
	/// @brief Closing destructor
	~GameManager ();
private:
	/// This method tries to load all the SDL configuration data from a
	/// file, but sets the default values if needed.
	/// @brief Method to load SDL configuration
	void loadSDLConf ();
	
	/// This method tries to load all the image file paths from a file, but
	/// sets the default paths if needed.
	/// @brief Method to load sprite configuration
	void loadSpriteConf ();
	
	/// This method sets the default values of SDL initalization.
	/// @brief Method to set the default values of SDL initalization
	void setDefaultSDLConf ();
	
	/// This method sets the default image file paths of all sprites.
	/// @brief Method to set the default image file paths
	void setDefaultSpriteConf ();
	
	/// This method loads all sprites with the set values in the sprite
	/// configuration.
	/// @brief Method to load all sprites
	void loadSprite ();
public:
	/// This method implements the main game loop.
	/// @brief Main game loop
	void run ();
private:
	/// This method takes all the input and handle as necessary.
	/// @brief Method to get user input
	void input ();
	
	/// This method updates all the data of game objects as needed.
	/// @brief Method to update data
	void update ();
	
	/// This method renders all sprites as needed.
	/// @brief Method to render sprites
	void render ();
	
	void handleQuit ();
	void showFPS ();
	void addPlanet ();
	void handleKeyDown ();
	void checkCollision ();
};

#endif
