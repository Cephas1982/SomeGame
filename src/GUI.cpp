#ifndef GUI_CPP
#define GUI_CPP
#include "GUI.h"
#include "ImageList.h"
#include "InputControls.h"
extern SDL_Surface* screen;
extern C_ImageList GUI_Img;
extern C_InputControls Controls;
extern bool MapEditorActive;
extern bool mapMode;
C_GUI::C_GUI()
{
	m_initMap =  false;
	p_zoomedScreen = NULL;
	m_mouseOffsetX = 1;
	m_mouseOffsetY = 1;
	m_zoomAxisX    = .5;
	m_zoomAxisY    = .5;
	m_zoomAmount   = 0;
	m_mapPanX	   = 0;
	m_mapPanY	   = 0;

	m_playerPosition.x = 0;
	m_playerPosition.y = 0;
	m_mapMarkerHighlight  = 100;


}
C_GUI::~C_GUI()
{
}
//Draw stuff
void C_GUI::Render_Map()
{
		SDL_Rect markerPosition = {0, 0};
		
		

		if(m_initMap == true){
			//Update X/Y pan
			m_mapPanX = -(m_playerPosition.x - SCREEN_WIDTH/2);

			p_zoomedScreen = zoomSurface(GUI_Img.getImage(0), m_zoomAxisX, m_zoomAxisY, SMOOTHING_ON);//make a copy of the zoomed map

			markerPosition.x = m_playerPosition.x*m_zoomAxisX + m_mapPanX;
			markerPosition.y = m_playerPosition.y*m_zoomAxisX + m_mapPanY;


			m_initMap = OFF;			
		}

		if(Controls.up && m_zoomAxisX < 1){//ZOOM OUT
			m_zoomAxisX+= .01;
			m_zoomAxisY+= .01;
			m_mouseOffsetX = 1 / (m_zoomAxisX * 2);
			m_mouseOffsetY = 1 / (m_zoomAxisY * 2);
			m_zoomAmount--;				
			m_mapPanX -= 5;
			m_mapPanY -= 5;
		//	if(m_mapPanY < -960 * m_zoomAxisX /4)
		//		m_mapPanY = -960 * m_zoomAxisX /4;

			p_zoomedScreen = zoomSurface(GUI_Img.getImage(0), m_zoomAxisX, m_zoomAxisY, SMOOTHING_ON);
		}
		if(Controls.down && m_zoomAxisX > .3){
			m_zoomAxisX-= .01;
			m_zoomAxisY-= .01;
			m_mouseOffsetX = m_zoomAxisX * 2;
			m_mouseOffsetY = m_zoomAxisY * 2;
			m_zoomAmount++;
			m_mapPanX += 5;
			m_mapPanY += 5;

		//	if(m_mapPanY < -960 * m_zoomAxisX /4)
		//		m_mapPanY = -960 * m_zoomAxisX /4;
			p_zoomedScreen = zoomSurface(GUI_Img.getImage(0), m_zoomAxisX, m_zoomAxisY, SMOOTHING_ON);
		}
		if(Controls.left)
			m_mapPanX-=5;
			if(m_mapPanX > 0)
				m_mapPanX = 0;
		if(Controls.right)
			m_mapPanX+=5;

		SDL_Rect clip = {0,0, 2560, 960};
		SDL_Rect offset = {m_mapPanX,m_mapPanY};
		
		SDL_BlitSurface(p_zoomedScreen, &clip, screen, &offset);//draw map
	

		markerPosition.x = m_playerPosition.x*(m_zoomAxisX*2) + m_mapPanX;
		markerPosition.y = m_playerPosition.y*(m_zoomAxisX*2) + m_mapPanY;
		filledEllipseRGBA(screen, markerPosition.x, markerPosition.y - 10, 10, 10, m_mapMarkerHighlight, 0, 200, m_mapMarkerHighlight);//draw player marker
		
		m_mapMarkerHighlight+= 2;
		if(m_mapMarkerHighlight > 255)
			m_mapMarkerHighlight = 100;
}
void C_GUI::Render_PlayerStatus(std::vector<C_BaseEntity*>* pv_Entities)
{
	//Have a static GUI somewhere on screen
	std::stringstream output;
	output << "HP" << (*pv_Entities)[0]->Get_hitPoints();
	GUI_Text.Print(&output, screen, 20, 0, 0);
	int i = 0;
}
void C_GUI::Render_HitPoints()
{
	std::stringstream hitPointDmg;
	for(int i = 0; i < MAX_DMG_SHOWED; i++)
		if(ShowDamage[i].is_finished == false){//if there's unfinished stuff to render..
			hitPointDmg << ShowDamage[i].m_damage;//get dmg to show
			GUI_Text.Print(&hitPointDmg, screen, ShowDamage[i].m_fontSize, ShowDamage[i].m_txtPos.x, ShowDamage[i].m_txtPos.y);//TODO CUSTOM COLOR FOR THINGS
			ShowDamage[i].m_counter--;
			if(ShowDamage[i].m_counter < 0)
				ShowDamage[i].is_finished = true;
		}
	
}
void C_GUI::Render_Lobby(bool isActive)
{
	if(isActive){
		//render lobby if ON
		std::stringstream output;
		output << "PLAYERS CONNECTED:";
		GUI_Text.Print(&output, screen, 18, 100, 100);










	}
}

//GET FUNCTIONS
float C_GUI::Get_mouseOffsetX()
{
	return m_mouseOffsetX;
}
float C_GUI::Get_mouseOffsetY()
{
	return m_mouseOffsetY;
}
float C_GUI::Get_zoomAxisX()
{
	return m_zoomAxisX;
}
float C_GUI::Get_zoomAxisY()
{
	return m_zoomAxisY;
}
int C_GUI::Get_mapPanX()
{
	return m_mapPanX;
}
int C_GUI::Get_mapPanY()
{
	return m_mapPanY;
}
//SET FUNCTION
void C_GUI::Set_mapPanX(int newValue)
{
	m_mapPanX = newValue;
}
void C_GUI::Set_mapPanY(int newValue)
{
	m_mapPanY = newValue;
}
void C_GUI::Set_playerPosition(SDL_Rect newPos)
{
	m_playerPosition.x = newPos.x;
	m_playerPosition.y = newPos.y;
}
void C_GUI::Set_initMap(bool onOff)
{
	m_initMap = onOff;
}

void C_GUI::StartDisplayDmg(SDL_Rect hitbox, int hitpoint)
{
	for(int i = 0; i < MAX_DMG_SHOWED; i++)
		if(ShowDamage[i].is_finished){//if there's an unused slot
			ShowDamage[i].Start(hitbox, hitpoint);
		}
}
#endif