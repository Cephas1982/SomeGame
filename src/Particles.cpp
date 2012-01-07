#ifndef PARTICLES_CPP
#define PARTICLES_CPP
#include "Particles.h"
#include "Camera.h"
extern SDL_Surface *screen;
extern C_Camera Camera;

C_BaseParticles::C_BaseParticles()
{
	//srand(time(NULL));
	m_magnitudeX = .015 * ((rand() % 81) - 40);//get random number between -1 and 1
	m_magnitudeY = .015 * ((rand() % 101) - 90);
	
	m_red = rand()% 255;
	m_green = rand()% 256;
	m_blue = 50;//rand()% 255;
	m_alpha = rand()% 55 + 200;

	m_bounce = 0;
}

C_BaseParticles::~C_BaseParticles()
{
}

void C_BaseParticles::Start(int startX, int startY)
{
	m_X = startX;
	m_Y = startY;
	m_life = 0;
}
bool C_BaseParticles::Update()
{
	bool stillAlive = true;

	m_red = rand()% 255;
	m_green = rand()% 256;
	m_blue = 50;//rand()% 255;


	if(m_bounce < 4){
		m_X += m_magnitudeX;
		m_Y += m_magnitudeY;
		
	}
	
	m_life += 2;	//occolation speed
	
	if(m_life >  50){
		m_alpha-= 5;
		m_magnitudeX *= 1.05;
	}
	if(m_life == 50 && m_magnitudeY < 0)
		m_magnitudeY = -m_magnitudeY;
		
	

	if(m_alpha <= 0)
		stillAlive = false;

	return stillAlive;
}
void C_BaseParticles::Render()
{
	if(m_life > 0)
		pixelRGBA(screen, m_X - Camera.Get_XYWH()->x, m_Y - Camera.Get_XYWH()->y, m_red, m_green, m_blue, m_alpha);
}
#endif