#ifndef BG_OBJECTS_H
#define BG_OBJECTS_H
#include <SDL.h>
#include <string>

class C_BG_Objects{

private:
			SDL_Rect m_XYWH;		//Where to cut object from sprite sheet
			SDL_Rect m_position;			//where it's displayed on screen
			int m_type;				//Type is the element in list array. SHould only be 1 of each type
			int m_bgObjectPlane;				//used for different altitudes
			int m_bgObjectType;					//for type of m_bgObject. -1 is default for "no m_bgObject"
			int m_layer;			//which layer to render
			bool m_destructable;			//m_bgObject is destructable??
			bool m_solid;					//true if solid
			bool m_friendlyDamage;			//true if m_bgObject does damage to player
			bool m_enemyDamage;				//true if m_bgObject does damage to enemy
			std::string m_name;		//obj name is saved in file

			bool m_highlight;
public:
			C_BG_Objects();
			~C_BG_Objects();

			//SET Functions
			void Set_XYWH(SDL_Rect);
			void Set_position(SDL_Rect);
			void Set_type(int);
			void Set_plane(int);
			void Set_layer(int);
			void Set_destructable(bool);
			void Set_solid(bool);
			void Set_friendlyDamage(bool);
			void Set_enemyDamage(bool);
			void Set_name(std::string);
			void Set_highlight(bool);

			//GET Functions
			SDL_Rect Get_XYWH();
			SDL_Rect Get_Position();
			int Get_type();
			int Get_plane();
			int Get_layer();
			bool Get_destructable();
			bool Get_solid();
			bool Get_friendlyDamage();
			bool Get_enemyDamage();
			bool Get_highlight();
			std::string Get_name();
};



#endif