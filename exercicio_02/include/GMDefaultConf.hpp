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
#define SDL_BPP		24

/// @brief Title of the window string
#define SDL_TITLE	"Trabalho 01 - 09/0125789"

/// @brief Path to the icon image file
#define SDL_ICON	"./bin/img/icon.png"

// =============================================================================
// Sprite set configuration
// =============================================================================

/// @brief Path to the Sprite set configuration file
#define SPR_CONF	"./conf/spr.conf"

/// @brief Path to the background image file
#define SPR_BG 		"./bin/img/bg.png"

/// @brief Path to the red planet image file
#define SPR_REDPLANET	"./bin/img/redplanet.png"

#define SCROLL 20.0f
#define TILEMAP_SIZE 16
#define TILE_SIZE 75
#define N_TEXTURES 30

#endif
