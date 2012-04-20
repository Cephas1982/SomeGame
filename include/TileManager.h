#ifndef TILEMANAGER_H
#define TILEMANAGER_H
#include <vector>
#include <SDL_gfxPrimitives.h>
#include "Config.h"
#include "ImageList.h"
#include "Tiles.h"
#include "BG_Objects.h"
#include "WarpZone.h"


class C_TileManager
{
	private:
			std::vector< std::vector<C_Tiles> > v2d_tileList; //Complete list of tiles using a 2d vector
			std::vector< std::vector<C_Tiles> > v2d_tileListBuffer;
			std::vector< std::vector<C_Tiles> >* p_mainTileBuffer;
			std::vector< std::vector<C_Tiles> >* p_secondTileBuffer;
			int m_mapHeight, m_mapWidth;//read these from a file.  This is the height/width in tiles, not pixels.  TILE_SIZE * mapheight would give height in pixels
			SDL_Rect m_mapPosition;
			
			std::vector<C_WarpZone> v_WarpZones;

			bool m_mapTransition;

			/***new render test***/
			bool tileRendered_complete, bufferBG_done;
			SDL_Surface* p_bufferBG_alternate;
			SDL_Surface* p_bufferBG;

			SDL_Surface* p_primaryBuffer;
			SDL_Surface* p_secondaryBuffer;
			
			
			int m_transitionCounter;
			int m_transitionType;

			SDL_Rect m_mainOffset, m_bufferOffset;
			SDL_Rect* p_mainOffset;
			SDL_Rect* p_secondOffset;
			float offsetCounterX, offsetCounterY;
			int transitionDirection;
			int m_staticBackground;
			
			int m_zoneType;//Overworld, dungeon, cave

			bool m_newGame;//used when resetting stuff

			//Warp stuff
			int m_warpUsed;//stores which warp player is taking. Parse data from this during loads
	public:
			C_TileManager();
			~C_TileManager();

			//LOAD & SAVE
			void LoadMainBuffer();//todo: load by file name  and do error checking
			void LoadBuffer();//temp?!?! TODO look over
			void SaveMap();//saves current map

			void LoadCave();

			void RenderStaticBackground();
			void Render();

			//SET FUNCTIONS
			void Set_tileType(int, int, int);
			void SetPlane(int, int, int);
			void Set_mapPosition(SDL_Rect);
			void Set_tileSolid(int, int, bool);

			//GET FUNCTIONS
			SDL_Rect* Get_tileOffset(int, int);
			int Get_transitionDirection();//returns -1 if not transitioning
			int GetPlane();
			int* Get_tileType(int, int);
			bool Get_tileSolid(int, int);
			SDL_Rect Get_tileBufferOffset();//returns m_mainOffset... probably used for map transitions
			int Get_offsetCounterX();//used to track where player goes and screen position
			int Get_offsetCounterY();//used to track where player goes
			SDL_Rect Get_mapPosition();//which map is loaded
			int Get_mapType();//returns map type (cave, dungeon, overworld)

			//MISC
			void MapTransition(int);//handles map transitions... TODO: Take parameters for user input
			void SwapBuffers();
			void SwapOffsets();

			//Zoning transitions
			void Transition_Start(int);//this only needs to be called once per transition. parameter is the type to use
			void Transitions_Render();//fadein, fadeout etc
			void Transition_FadeIn();// fade in to game
			void Transition_FadeOut();// fade to black
			bool Transition_GetStatus();//returns true if transition is in progress

			//WARPS
			void Create_Warp();//creates an object
			void Delete_Warp(int);//removes object from vector
			void Render_Warp();//draws object if their flag is on
			void Set_WarpType(int, int);//set type of warp (warp object, newtype)
			int Get_WarpType(int);
			SDL_Rect Get_WarpEndMap(int);//returns which file warp takes you to
			SDL_Rect Get_WarpStartMap(int);//gets where warps origin file
			void Set_WarpPosition(int, SDL_Rect);//coordinates of physical warp zone on map
			void Set_WarpEndMap(int, SDL_Rect);
			void Set_WarpStartMap(int, SDL_Rect);
			void Set_WarpEndPosition(int, SDL_Rect);//where player ends up on map after warp
			

			SDL_Rect Get_WarpDestination(int);
			SDL_Rect Get_WarpPosition(int);
			int Get_WarpCount();//returns num of warps in vector

			void ToggleWarpHighlight(int, bool);

			//RESET GAME
			void StartNewGame();//puts m_newGame to false forcing load from save file


			//ZONE STUFF TESTING
			void Set_warpUsed(int);
		

			
};
#endif