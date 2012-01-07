#ifndef ENEMY1_CPP
#define ENEMY1_CPP
#include "Enemy1.h"
#include "InputControls.h"
#include "TileManager.h"
#include <time.h>

extern SDL_Surface *screen;
extern SDL_Event event;
extern C_InputControls Controls;
extern C_Camera Camera;
extern C_TileManager TileManager;

//temp?
extern bool mapMode;
C_Enemy1::C_Enemy1()
{
	//Set flags TODO
	//BitManip.Set_bit(m_flags, INDESTRUCTIBLE, ON);


	//Init stuff
	Set_X(356);
	Set_Y(721);
	Set_xvel(0);
	Set_yvel(0);
	Set_xSpeed(0);
	Set_ySpeed(0);
	Set_gravityOffset(0);
	Set_hitPoints(5);
	Set_flags(0);
	m_fixedXvel = .4;
	m_fixedYvel = .4;
	m_xMaxSpeed = 1;
	m_yMaxSpeed = 1;
	
	m_lockControls = false;//can move by default

	m_hittingWall = false;
	//ENEMY SPECIFICS
	m_startNewMove = true;
	m_directionMoving = 0;
	m_distanceTraveledX = 0;
	m_distanceTraveledY = 0;

	m_cage.x = 0;
	m_cage.y = 480;
	m_cage.w = 640;
	m_cage.w = 960;

	srand(time(NULL));//Get a random direction to travel in
}

C_Enemy1::~C_Enemy1()
{
	
}

void C_Enemy1::Render()
{
	SDL_Rect offset = {Get_X() - Camera.Get_XYWH()->x, Get_Y() - Camera.Get_XYWH()->y, PLAYER_WIDTH, PLAYER_HEIGHT};
	Uint32 color = SDL_MapRGB(screen->format,  255, 205, 0);
	SDL_FillRect(screen, &offset, color);
}
void C_Enemy1::HandleInputs(C_InputControls *Input)//This is automated since it's A.I.
{
	Set_xvel(0);
	Set_yvel(0);


	//start moving somewhere or where moving to under these conditions
	if(m_startNewMove == true || m_hittingWall == true){
		m_distanceTraveledX = 0;
		m_distanceTraveledY = 0;
		
		m_directionMoving = rand() % 8; //directions 1 - 8 seeded off of time
		m_startNewMove = false;
		m_hittingWall = false;
	}
	//randomly move up, down, left, or right using random number generator
	switch(m_directionMoving){
	case UP: m_yvel = -m_fixedYvel;
			 m_angleFacing = 270;
				break;
	case DOWN: m_yvel = m_fixedYvel;
			   m_angleFacing = 90;	
				break;
	case LEFT: m_xvel = -m_fixedXvel;
			   m_angleFacing = 180;
				break;
	case RIGHT: m_xvel = m_fixedXvel;
				m_angleFacing = 0;
				break;
	default:	m_xvel = 0;
				m_yvel = 0;
				m_startNewMove = true;
				break;
	}
	//move at least "---" pixels before looking for a new place to move


	m_xSpeed += m_xvel;//add velocity to speed
	m_ySpeed += m_yvel;//add velocity to speed

	Attack();
}
void C_Enemy1::Attack()
{
	//if(m_distanceTraveledX == 50 || m_distanceTraveledY == 50){
	//	int a  = 0;
	//}

	if(rand() % 500 == 1){
		C_BaseWeapon *B_Rang = new C_Boomerang(&m_X, &m_Y, &m_angleFacing);//create boomerang object
		B_Rang->Set_flags(DAMAGE_PLAYER, ON);
		B_Rang->Set_flags(DAMAGE_ENEMY, OFF);
		B_Rang->Set_flags(WEAK_VS_SHIELD, ON);

		Weapons.push_back(B_Rang);
	}
}
void C_Enemy1::Move()
{
	//MOVE

	//LIMITS to keep things from getting stupid
	if(Get_xSpeed() > m_xMaxSpeed)
		Set_xSpeed(m_xMaxSpeed);
	if(Get_xSpeed() < -m_xMaxSpeed)
		Set_xSpeed(-m_xMaxSpeed);

	if(Get_ySpeed() > m_yMaxSpeed)
		Set_ySpeed(m_yMaxSpeed);
	if(Get_ySpeed() < -m_yMaxSpeed)
		Set_ySpeed(-m_yMaxSpeed);

	//Update the position TODO: Make variable 'requested position' for future collision checking
	Set_X(Get_X() + Get_xSpeed());
	Set_Y(Get_Y() + Get_ySpeed());

	//CAGE ---temp for now?
	if(m_X < m_cage.x )
		m_X = 0;
	if(m_X > m_cage.w )
		m_X = 620;// change to minus enemy with TODO
	if(m_Y < m_cage.y)
		m_Y = 480;
	if(m_Y > m_cage.h)
		m_Y = 940;
/*
	//Window collision
	if(Get_X() + Get_xSpeed() < 0)
		Set_X(0);
	if(Get_X() + Get_xSpeed() > LEVEL_WIDTH - PLAYER_WIDTH)//factor in player height/width TODO
		m_X = (m_X - m_xSpeed);
	if(Get_Y() + Get_ySpeed() < 0)
		Set_Y(0);
	if(Get_Y() + Get_ySpeed() > LEVEL_HEIGHT - PLAYER_HEIGHT)
		m_Y = (m_Y - m_ySpeed);
	 
	//If there's a sword swing, move the shield
	for(int i = 0; i < Weapons.size(); i++)
		if(Weapons[i]->Get_weaponType() == SWORD)//if sword found
			Controls.key_S = true;//simulate shield


	//MOVE WEAPONS////////////////////////
	if(Weapons.size() > 0)//if weapons are in queue
		for(int i = 0; i < Weapons.size(); i++)
			if(Weapons[i]->Get_isFinished())//if weapon is done doing w/weapons do
				Weapons.erase(Weapons.begin() + i );//erase it from the vector
			else
				Weapons[i]->Move();
*/				

	m_distanceTraveledX += m_xSpeed;
	m_distanceTraveledY += m_ySpeed;

	if(abs(m_distanceTraveledX) > 100){
		m_startNewMove = true;
		m_xSpeed = 0;
	}
	if(abs(m_distanceTraveledY) > 100){
		m_startNewMove = true;
		m_ySpeed = 0;
	}
}
#endif