#ifndef WARPZONE_H//TODO: make virtual base object class
#define WARPZONE_H
#include <SDL.h>
#include "Config.h"

class C_WarpZone{

		private:	//stuff
					SDL_Rect m_startPosition;//holds position/width/height
					SDL_Rect m_endPosition;//where to end up after warp complete
					bool m_active, m_visible;//true when active (example, false when covered by something)
					int m_warpType;//used to determine if warping to overworld, cave, etc

					SDL_Rect m_startMap, m_endMap;

					//render stuff
					bool m_highlighted;
					bool m_colorIncrease;
					int m_red, m_green, m_blue;
		public:	
				C_WarpZone();
				~C_WarpZone();
				
				void Render();//for debug

				//get
				SDL_Rect Get_position();//returns m_position
				SDL_Rect Get_endPosition();//returns where player will end up
				SDL_Rect Get_startMap();
				SDL_Rect Get_endMap();//returns ending map coordinates for this warp
				int Get_warpType();
				bool Get_active();
				bool Get_visible();

				//set
				void Set_position(SDL_Rect);
				void Set_endPosition(SDL_Rect);
				void Set_startMap(SDL_Rect);
				void Set_endMap(SDL_Rect); //(map x, map y) // .txt
				void Set_warpTo(int);
				void Set_active(bool);
				void Set_visible(bool);

				//toggle
				void ToggleHighlight(bool);//flips highlight on/off


};
#endif