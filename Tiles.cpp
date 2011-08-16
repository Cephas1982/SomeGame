#ifndef TILES_CPP
#define TILES_CPP
#include "Tiles.h"
C_Tiles::C_Tiles()
{
			m_tileX = 0;
			m_tileY = 0;
			m_tileWidth	= TILE_SIZE;
			m_tileHeight = TILE_SIZE;
			m_tilePlane	= false;
			m_destructable = false;
			m_solid = false;
			m_friendlyDamage = false;
			m_enemyDamage = false;
			m_tileType = -1; // -1 is nothing

			m_tileSDL_Rect.w = TILE_SIZE;
			m_tileSDL_Rect.h = TILE_SIZE; //16x16
}
C_Tiles::~C_Tiles()
{
}
//SET FUNCTIONS//////////
void C_Tiles::Set_tileX(int newValue)
{
	m_tileX = newValue;
}
void C_Tiles::Set_tileY(int newValue)
{
	m_tileY = newValue;
}
void C_Tiles::Set_tileWidth(int newValue)
{
	m_tileWidth = newValue;
}
void C_Tiles::Set_tileHeight(int newValue)
{
	m_tileHeight = newValue;
}
void C_Tiles::Set_tilePlane(int newValue)
{
	m_tilePlane = newValue;
}
void C_Tiles::Set_tileType(int newValue)
{
	m_tileType = newValue;
}
void C_Tiles::Set_destructable(bool newValue)
{
	m_destructable = newValue;
}
void C_Tiles::Set_solid(bool newValue)
{
	m_solid = newValue;
}
void C_Tiles::Set_friendlyDamage(bool newValue)
{
	m_friendlyDamage = newValue;
}
void C_Tiles::Set_enemyDamage(bool newValue)
{
	m_enemyDamage = newValue;
}

//GET Functions////////////////////
SDL_Rect C_Tiles::Get_tileSDL_Rect()
{
	m_tileSDL_Rect.x = m_tileX;
	m_tileSDL_Rect.y = m_tileY;

	return m_tileSDL_Rect;
}

int C_Tiles::Get_tileX()
{
	return m_tileX;
}

int C_Tiles::Get_tileY()
{
	return m_tileY;
}
int C_Tiles::Get_tileWidth()
{
	return m_tileWidth;
}
int C_Tiles::Get_tileHeight()
{
	return m_tileHeight;
}
int C_Tiles::Get_tilePlane()
{
	return m_tilePlane;
}
int C_Tiles::Get_tileType()
{
	return m_tileType;
}
bool C_Tiles::Get_destructable()
{
	return m_destructable;
}
bool C_Tiles::Get_solid()
{
	return m_solid;
}
bool C_Tiles::Get_friendlyDamage()
{
	return m_friendlyDamage;
}
bool C_Tiles::Get_enemyDamage()
{
	return m_enemyDamage;
}
#endif