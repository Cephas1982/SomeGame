#ifndef TILES_H
#define TILES_H
#include <SDL.h>
#include "Config.h"

class C_Tiles
{
	protected:
			SDL_Rect m_tileSDL_Rect;				//used to return value of both x/y in a SDL_Rect
			int m_tileX, m_tileY;			//tile position
			int m_tileWidth, m_tileHeight;	//see #define in config for TILESIZE
			int m_tilePlane;				//used for different altitudes
			int m_tileType;					//for type of tile. -1 is default for "no tile"
			bool m_destructable;			//tile is destructable??
			bool m_solid;					//true if solid
			bool m_friendlyDamage;			//true if tile does damage to player
			bool m_enemyDamage;				//true if tile does damage to enemy
			

	public:

			C_Tiles();
			~C_Tiles();

			//SET Functions
			void Set_tileX(int);
			void Set_tileY(int);
			void Set_tileWidth(int);
			void Set_tileHeight(int);
			void Set_tilePlane(int);
			void Set_tileType(int);
			void Set_destructable(bool);
			void Set_solid(bool);
			void Set_friendlyDamage(bool);
			void Set_enemyDamage(bool);

			//GET Functions
			SDL_Rect* Get_tileSDL_Rect();
			int* Get_tileX();
			int* Get_tileY();
			int Get_tileWidth();
			int Get_tileHeight();
			int Get_tilePlane();
			int* Get_tileType();
			bool Get_destructable();
			bool Get_solid();
			bool Get_friendlyDamage();
			bool Get_enemyDamage();
};
#endif