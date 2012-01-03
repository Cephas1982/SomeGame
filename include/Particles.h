#ifndef PARTICLES_H
#define PARTICLES_H
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <math.h>
#include <time.h>
#include "Config.h"

class C_BaseParticles{

	protected:
			float m_X, m_Y;//x/y coordinates
			float m_magnitudeX, m_magnitudeY;	
			float m_gravity;
			float m_life;
			int m_originX, m_originY;
			int m_bounce;
			int m_red, m_blue, m_green, m_alpha;
	

	public:
			C_BaseParticles();
			~C_BaseParticles();


			void Start(int, int);//starts particles at x/y position

			bool Update();//calculates stuff AND returns true when particle is alive, false when dead

			void Render();// =D

};



#endif