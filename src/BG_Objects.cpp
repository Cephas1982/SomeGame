#ifndef BG_OBJECTS_CPP
#define BG_OBJECTS_CPP
#include "BG_Objects.h"

C_BG_Objects::C_BG_Objects()
{
	m_XYWH.x = 0;
	m_XYWH.y = 0;
	m_XYWH.w = 0;
	m_XYWH.h = 0;

	m_bgObjectPlane	= false;
	m_destructable = false;
	m_solid = false;
	m_friendlyDamage = false;
	m_enemyDamage = false;
	m_bgObjectType = -1; // -1 is nothing
	m_name = "";
	m_highlight = false;
	m_layer = 1;
}
C_BG_Objects::~C_BG_Objects()
{
}
//SET FUNCTIONS//////////
void C_BG_Objects::Set_XYWH(SDL_Rect newXYWH)
{
	m_XYWH.x = newXYWH.x;
	m_XYWH.y = newXYWH.y;
	m_XYWH.w = newXYWH.w;
	m_XYWH.h = newXYWH.h;
}
void C_BG_Objects::Set_position(SDL_Rect newPosition)
{
	m_position.x = newPosition.x;
	m_position.y = newPosition.y;
	
}
void C_BG_Objects::Set_type(int newType)
{
	m_type = newType;
}
void C_BG_Objects::Set_plane(int newValue)
{
	//TODO Bounds check
	m_bgObjectPlane = newValue;
}
void C_BG_Objects::Set_layer(int newValue)
{
	m_layer = newValue;
	if(m_layer > 3)
		m_layer = 1;
	if(m_layer < 1)
		m_layer = 3;
}
void C_BG_Objects::Set_destructable(bool newValue)
{
	m_destructable = newValue;
}
void C_BG_Objects::Set_solid(bool newValue)
{
	m_solid = newValue;
}
void C_BG_Objects::Set_friendlyDamage(bool newValue)
{
	m_friendlyDamage = newValue;
}
void C_BG_Objects::Set_enemyDamage(bool newValue)
{
	m_enemyDamage = newValue;
}
void C_BG_Objects::Set_name(std::string newName)
{
	m_name = newName;
}
void C_BG_Objects::Set_highlight(bool onOff)
{
	m_highlight = onOff;
}

//GET Functions////////////////////
SDL_Rect C_BG_Objects::Get_XYWH()
{
	return m_XYWH;
}
SDL_Rect C_BG_Objects::Get_Position()
{
	return m_position;
}
int C_BG_Objects::Get_type()
{
	return m_type;
}
int C_BG_Objects::Get_plane()
{
	return m_bgObjectPlane;
}
int C_BG_Objects::Get_layer()
{
	return m_layer;
}
bool C_BG_Objects::Get_destructable()
{
	return m_destructable;
}
bool C_BG_Objects::Get_solid()
{
	return m_solid;
}
bool C_BG_Objects::Get_friendlyDamage()
{
	return m_friendlyDamage;
}
bool C_BG_Objects::Get_enemyDamage()
{
	return m_enemyDamage;
}
bool C_BG_Objects::Get_highlight()
{
	return m_highlight;
}	
std::string C_BG_Objects::Get_name()
{
	return m_name;
}


#endif