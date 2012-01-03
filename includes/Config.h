#ifndef CONFIG_H
#define CONFIG_H
#include <SDL.h>

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define LEVEL_WIDTH		1280 //make dynamic later
#define LEVEL_HEIGHT	960  //make dynamic later

#define CAVE_WIDTH 640
#define CAVE_HEIGHT 480

#define DUNGEON_WIDTH 640
#define DUNGEON_HEIGHT 480

#define SCREEN_BPP 32

#define FPS 60

#define PI 3.1415926535
//TRUE FALSE
#define ON	1
#define OFF 0

//ENTITY FLAGS
enum g_entityFlags{ENTITY_PLAYER_TYPE, ENTITY_ENEMY_TYPE, ENTITY_INVINCIBLE };

//WEAPON FLAGS
enum g_weaponFlags{ INDESTRUCTIBLE, ALWAYS_DESTRUCTIBLE, DEFLECTABLE, WEAK_VS_SHIELD, WEAK_VS_SWORD, WEAK_VS_BOMB, WEAK_VS_BOOMERANG, STRONG_VS_LIGHT_PROJECTILES,
						   DAMAGE_PLAYER, DAMAGE_ENEMY, DEFLECT_BACK_NORMAL, DEFLECT_BACK_FAST};

//WEAPON TYPES  ***UPDATE HERE AS MORE WEAPONS ARE ADDED****  TODO
#define BOOMERANG	1
#define SWORD		2
#define SHIELD		3


//PLAYER VARIABLES
#define PLAYER_HEIGHT	20
#define PLAYER_WIDTH	20

//SOUND --- SFX
#define SWORD_1			0
#define BOOMERANG		1
#define SHIELD_SFX		2


//TILES
#define TILE_SIZE		16	//tiles are 16x16
#define MAX_TILE_COUNT	55

//DIRECTIONS
#define UP			6
#define DOWN		2
#define LEFT		4
#define RIGHT		0
#define UP_LEFT		5
#define UP_RIGHT	7
#define DOWN_RIGHT  1
#define DOWN_LEFT   3


/////////////////
//Camera Types
#define OVERWORLD	1
#define DUNGEON		2
#define CAVE		3


//TRANSITIONS
#define FADE_IN		0
#define FADE_OUT	1

//NETWORK
#define SERVER_PORT 2223
#define SERVER_IP "192.168.1.126"
#define MAXBUFFER 2048
#define SERVER 1
#define CLIENT 2

struct s_gamePacket{
	SDL_Rect m_playerPosition;
	SDL_Rect m_cameraPosition;
	float m_velocityX;
	float m_velocityY;
	
};


#endif