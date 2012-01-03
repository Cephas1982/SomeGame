#ifndef MAPEDITOR_H
#define MAPEDITOR_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <sstream>
#include <vector>
#include "Config.h"
#include "Player1.h"
#include "TTF.h"
#include "TileManager.h"
#include "GUI.h"
#define MENU_WIDTH 300
#define MENU_HEIGHT 80


class C_MapEditor//This class should manage the TILE class. Example: when going from zone to zone this can handle tileset/screen transitions
{
	private:
			int m_mouseX, m_mouseY;
			bool m_menuActive;
			C_Font Font;
			C_TileManager* p_tileManager;//will hold pointer to the tile manager.
			std::vector<C_BaseEntity*>* pv_Entities;
			C_GUI* p_GUI;//gui pointer

			//Mouse variables
			SDL_Rect highlightStart, highlightStop, highlightArea;
			bool m_highlight, m_showHighlight, m_showPaste;


			//Setting tile variables
			int m_tileType;
			int m_plane; //0 is a hole, 1+ keeps getting higher
			bool m_destructable, m_solid, m_friendlyDmg, m_enemyDmg;
			Uint32 m_tileColor;
			int m_colorR, m_colorG, m_colorB, m_colorA;

			//Menu Variables
			SDL_Rect menuPosition;
			std::stringstream row1, row2, row3, row4, row5, row6, row7, row8, row9;
			bool lock_row1, lock_row2, lock_row3, lock_row4, lock_row5, lock_row6, lock_row7, lock_row8, lock_row9;
			int m_menuX, m_menuY;
			int m_rowSelected;
			int m_numOfRows;
			int m_currentMenu;
			int m_tileBuildSize; //for 2x2, 4x4 etc used to disply build grid

			//Warp Menu variables
			int warpSelected;

			//BG object variables
			int m_bgSelected;// Which active bg is being used
			int m_bgTypeShowing;//which bg list item is being displayed

			//Camera Tracking
			int m_tempTrackX;
			int m_tempTrackY;
			int m_trackToX, m_trackToY;
			int m_playerX, m_playerY;
			int* ptempX;
			int* ptempY;
			bool m_trackPlayer;//false when tracking stuff besides player

			//HANDLE IMAGES AND CUSTOM TILE SETS
			C_ImageList TileSprites;
			std::vector< std::vector<C_Tiles> > v2d_tileBuffer;

			//Map Menus
			int m_mapAlphaCounter;

			//m_currentMenu can be any of these below
			enum menuTypes { 
				MENU_MAIN, MENU_BACK, MENU_ROOT,
				TILE_MAIN, TILE_CHANGE, TILE_PLANE, TILE_DESTRUCTABLE, TILE_SOLID, TILE_FRIENDLYDAMAGE, TILE_ENEMYDAMAGE, TILE_BUILD_TILE_SET,
				WARPS_MAIN,
				BG_OBJECTS_MAIN

			};

	public:
			C_MapEditor(C_TileManager*, std::vector<C_BaseEntity*>*, C_GUI*);
			~C_MapEditor();

			void Start();//goes into map editor mode
			void Stop();//exits editor
			void Update();//handles what's going on while running
			void NavigateMenus();
			void HandleMouse();//handle mouse inputs & actions
			void HighlightOptions();//things you can do while highlighting


			//MENUS
			void MainMenu();//menu for map editor
			void TilesMenu();
				void Tiles_EditType();
				void Tiles_EditPlane();
				void Tiles_EditDestructable();
				void Tiles_EditSolid();
				void Tiles_EditFriendlyDmg();
				void Tiles_EditEnemyDmg();
				void Tiles_EditTileSet();
			void WarpMenu();
				void Objects_PlaceWarp();
			void BG_ObjectsMenu();

			//WORLD MAP STUFF
			void WorldMapNav();
				

};
#endif