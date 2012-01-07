#ifndef GUI_H
#define GUI_H
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include <sstream>
#include <vector>
#include "TTF.h"
#include "BaseEntity.h"
#include "ShowDamage.h" //Container to hold show dmg variables, make a struct instead???
#define MAX_DMG_SHOWED 20

class C_GUI{

	private: 
			bool m_initMap;
			float m_mouseOffsetX, m_mouseOffsetY;//offset for mouse position when maps are being displayed
			float m_zoomAxisX, m_zoomAxisY;
			int m_zoomAmount; //tracks how much to zoom
			int m_mapPanX, m_mapPanY;
			int m_mapMarkerHighlight;

			SDL_Surface* p_zoomedScreen;
			SDL_Rect m_playerPosition;

			C_Font GUI_Text;

			C_ShowDamage ShowDamage[MAX_DMG_SHOWED];


	public:
			C_GUI();
			~C_GUI();

			void Render_Map();
			void Render_PlayerStatus(std::vector<C_BaseEntity*>*);
			void Render_HitPoints();
			void Render_Lobby(bool ON_or_OFF);//for netplay

			
			//GET
			float Get_mouseOffsetX();
			float Get_mouseOffsetY();
			float Get_zoomAxisX();
			float Get_zoomAxisY();
			int Get_mapPanX();
			int Get_mapPanY();

			//SET
			void Set_mapPanX(int);
			void Set_mapPanY(int);
			void Set_playerPosition(SDL_Rect);
			void Set_initMap(bool);

			//Display Damage gained/lost
			void StartDisplayDmg(SDL_Rect, int);//x/y, hitpoint
};
#endif