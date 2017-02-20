/// @ingroup MOD_GAMEMANAGER
/// @file GMDefaultConf.hpp
/// @brief Definitions of all default values used in GameManager class
/// @author Matheus Pimenta

#ifndef GMDEFAULTCONF_HPP
#define GMDEFAULTCONF_HPP

// =============================================================================
// SDL configuration
// =============================================================================

/// @brief Path to the SDL configuration file
#define SDL_CONF	"./conf/SDL.conf"

/// @brief Window width
#define SDL_WIDTH	800

/// @brief Window height
#define SDL_HEIGHT	600

/// @brief Bits per pixel
#define SDL_BPP		32

/// @brief Title of the window string
#define SDL_TITLE	"Trabalho 03 - 09/0125789"

/// @brief Path to the icon image file
#define SDL_ICON	"./bin/img/icon.png"

/// @brief Frames-per-second rate
#define SDL_FPS 	30

// =============================================================================
// Sprite set configuration
// =============================================================================

/// @brief Path to the Sprite set configuration file
#define SPR_CONF	"./conf/spr.conf"

/// @brief Path to the background image file
#define SPR_BG 		"./bin/img/bg.png"

/// @brief Path to the red planet image file
#define SPR_REDPLANET	"./bin/img/redplanet.png"

/// @brief Path to the Earth image file
#define SPR_EARTH	"./bin/img/earth.png"

/// @brief Path to the Moon image file
#define SPR_MOON	"./bin/img/moon.png"

/// @brief Path to the follower object image file
#define SPR_UFO		"./bin/img/ufo.png"

#endif
