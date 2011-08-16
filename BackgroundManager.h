#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <fstream>
#include <string>
#include <vector>
#include "BG_Objects.h"
#include "ImageList.h"


class C_BackgroundManager{

	private:			
			bool m_colorIncrease;
			int m_red, m_green, m_blue;
			std::vector <C_BG_Objects> v_bgObjectList;
			std::vector <C_BG_Objects> v_bgObjectsActive;
			

	public:
			C_BackgroundManager();
			~C_BackgroundManager();

			
			//RENDER
			void RenderList(int, SDL_Rect);//renders objects in list
			void RenderActive();//renders all active objects in 
			void RenderLayer1();
			void RenderLayer2();
			void RenderLayer3();
			
			//CREATE/REMOVE
			void Create(int, int, SDL_Rect);//copys everything from list object and creates world coordinate (TYPE, LAYER, POSITION)
			void Remove(int);

			//SET
			void Set_layer(int, int);
			//GET
			int Get_layer(int);
			SDL_Rect Get_position(int);

			//MISC
			int ActiveSize();//returns size of active objs
			int ListSize();//returns size of object list (pool whatever)
			void Set_highlight(int, bool);//true highlights object


			//SAVE & LOAD
			void Save();
			void Load();
			void LoadImageData();//parses txt file for coordinates of BG images
};
#endif