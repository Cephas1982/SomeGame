#ifndef SHOWDAMAGE_H
#define SHOWDAMAGE_H
#include <SDL.h>
class C_ShowDamage 
{
	public:
			int m_counter;
			int m_damage;
			int m_fontSize;
			SDL_Rect m_txtPos;//position
			bool is_finished;
	

	public:
			C_ShowDamage();
			~C_ShowDamage();

		//	bool DisplayDamage();//displays damage and will float up, damage & offset of enemy are the parameters

			//set functions
			void Start(SDL_Rect, int); //  x, y, hitpoint to show

			//get functions
		//	bool getIsFinished(); //returns is_finished (true == available)
};
#endif