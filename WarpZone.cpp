#ifndef WARPZONE_CPP
#define WARPZONE_CPP
#include "WarpZone.h"
#include "Camera.h"
extern SDL_Surface* screen;
extern C_Camera Camera;

C_WarpZone::C_WarpZone()
{
	m_startPosition.x = 122;
	m_startPosition.y = 545;
	m_startPosition.w = 48;
	m_startPosition.h = 48;

	m_endPosition.x = 320;
	m_endPosition.y = 100;
	m_endPosition.w = 0;
	m_endPosition.h = 0;

	m_warpType = CAVE;

	m_active = true;
	m_visible = true;//visible by default

	m_startMap.x = 4;
	m_startMap.y = 4;

	m_endMap.x = 0;
	m_endMap.y = 0;

	m_red = 0;
	m_green = 200;
	m_blue = 200;
	m_colorIncrease = true;
	m_highlighted = false;
}
C_WarpZone::~C_WarpZone()
{
}

void C_WarpZone::Render()
{
	if(m_highlighted){//USED TO MAKE SELECTED WARP FLASH
		if(m_blue < 255 && m_colorIncrease){
			m_blue += 3;
			m_green +=3;
		}
		else{
			if(m_colorIncrease){
				m_blue = 255;
				m_green = 255;
				m_colorIncrease = false;
			}
		}

		if(m_blue > 0 && !m_colorIncrease){
			m_blue -= 3;
			m_green -=3;
		}
		else{
			if(!m_colorIncrease){
				m_blue = 0;
				m_green = 0;
				m_colorIncrease = true;
			}
		}
	}
	else{
		m_red = 100;
		m_green = 100;
		m_blue = 200;
	}//end HIGHTLIGHTED

	SDL_Rect offset = {m_startPosition.x - Camera.Get_XYWH().x, m_startPosition.y - Camera.Get_XYWH().y, m_startPosition.w, m_startPosition.h};
	Uint32 color = SDL_MapRGB(screen->format, m_red, m_green, m_blue);
	SDL_FillRect(screen, &offset, color);
}
//GET FUNCTIONS
SDL_Rect C_WarpZone::Get_position()
{
	return m_startPosition;
}
SDL_Rect C_WarpZone::Get_endPosition()
{
	return m_endPosition;
}
SDL_Rect C_WarpZone::Get_startMap()
{
	return m_startMap;
}
SDL_Rect C_WarpZone::Get_endMap()
{
	return m_endMap;
}
int C_WarpZone::Get_warpType()
{
	return m_warpType;
}
bool C_WarpZone::Get_active()
{
	return m_active;
}
bool C_WarpZone::Get_visible()
{
	return m_visible;
}

//Set functions
void C_WarpZone::Set_position(SDL_Rect newPosition)
{
	//set m_startPosition to newposition
	m_startPosition.x = newPosition.x;
	m_startPosition.y = newPosition.y;
}
void C_WarpZone::Set_endPosition(SDL_Rect newPosition)
{
	//set m_startPosition to newposition
	m_endPosition.x = newPosition.x;
	m_endPosition.y = newPosition.y;	
}
void C_WarpZone::Set_startMap(SDL_Rect newStartMap)
{
	//TODO BOUND CHECK BASED ON ZONE
	
	if(newStartMap.x < 0 || newStartMap.x > 8)
		newStartMap.x = 0;
	if(newStartMap.y < 0 || newStartMap.y > 4)
		newStartMap.y = 0;
	
	m_startMap.x = newStartMap.x;
	m_startMap.y = newStartMap.y;
}
void C_WarpZone::Set_endMap(SDL_Rect newEndMap)
{
	//TODO BOUND CHECK BASED ON ZONE
	if(newEndMap.x < 0 || newEndMap.x > 8)
			newEndMap.x = 0;
	if(newEndMap.y < 0 || newEndMap.y > 4)
		newEndMap.y = 0;

	m_endMap.x = newEndMap.x;
	m_endMap.y = newEndMap.y;
}
void C_WarpZone::Set_warpTo(int areaType)
{
	if(areaType > 3)
		areaType = 1;//overworld when invalid type given

	m_warpType = areaType;
}
void C_WarpZone::Set_active(bool is_it_active)
{
	m_active = is_it_active;
}
void C_WarpZone::Set_visible(bool is_it_visible)
{
	m_visible = is_it_visible;
}
//TOGGLE STUFF
void C_WarpZone::ToggleHighlight(bool onOff)
{
	m_highlighted = onOff;
}
#endif