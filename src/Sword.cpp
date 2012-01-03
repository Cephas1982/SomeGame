#ifndef SWORD_CPP
#define SWORD_CPP
#include "Sword.h"
#include "InputControls.h"
#define SWORD_WIDTH 7.5
#define SWORD_HEIGHT 7.5
extern SDL_Surface *screen;
extern C_InputControls Controls;
extern C_Camera Camera;

C_Sword::C_Sword(int *Player_Xpointer, int *Player_Ypointer, int* playerAngle) : C_BaseWeapon(Player_Xpointer, Player_Ypointer, playerAngle)
{
	//FLAGS
	BitManip.Set_bit(m_flags, INDESTRUCTIBLE, ON);
	BitManip.Set_bit(m_flags, DAMAGE_ENEMY, ON);//by default these hurt enemies. Must turn off and on PLAYER DMG to swap

	//Init stuff
	mW_weaponType = SWORD;
	p_PlayerX = Player_Xpointer;
	p_PlayerY = Player_Ypointer;
	mW_X = *p_PlayerX + PLAYER_WIDTH/2 - SWORD_WIDTH;//origin of sword is center of character for now
	mW_Y = *p_PlayerY + PLAYER_HEIGHT/2 - SWORD_HEIGHT;//TODO -variable instead of '5'
	mW_xvel = 0;
	mW_yvel = 0;
	mW_xSpeed = 0;
	mW_ySpeed = 0;
	mW_xMaxSpeed = 4;
	mW_yMaxSpeed = 4;
	mW_distanceTraveled = 0;
	mW_maxDistance = 250;
	mW_currentAngle = *playerAngle * PI / 180;//this is which way player is facing TODO: write that 
	mW_originalAngle = mW_currentAngle;

	stop = false;
	Controls.lockMovement = true;//lock down movement during sword swings

	//TEMP?
	frameCounter = 15;
	m_maxReached = false;

	AudioManager.PlaySFX(SWORD_1, 1);

	//hitbox init
	mW_hitbox.x = 0;
	mW_hitbox.y = 0;
	mW_hitbox.w = 0;
	mW_hitbox.h = 0;
}

C_Sword::~C_Sword()
{
	
}
void C_Sword::Update()
{
	//start somewhere near -90* from where player is facing and 20 pixels out
	mW_currentAngle -= .08;//increase angle
	
	//get the starting angle
	mW_xvel = cos(mW_currentAngle + 30*PI/180); //starts 90 degrees to the right, but need to fine tune
	mW_yvel = sin(mW_currentAngle + 30*PI/180);
	float Degrees = mW_currentAngle * 180/PI;

	if(mW_currentAngle < mW_originalAngle - 180*PI/180){//end this after a 180 degree swing
		stop = true;
		Controls.lockMovement = false; // Unlock movement
	}
}
void C_Sword::Move()
{
	//calculate magnitude
	mW_xSpeed = mW_xvel;//SPEED IS VECTOR MAGNITUDE
	mW_ySpeed = mW_yvel;//TODO: RENAME VARIABLE =(
}

void C_Sword::Render()
{
	float swordSize = frameCounter;
	float swordLength = 3;
	int minX = mW_X - Camera.Get_XYWH().x;
	int minY = mW_Y - Camera.Get_XYWH().y;
	int maxX = mW_X - Camera.Get_XYWH().x;
	int maxY = mW_Y - Camera.Get_XYWH().y;
	for(int i = 0; i < swordSize; i ++){
		SDL_Rect offset = {(mW_X + mW_xSpeed * i * swordLength)- Camera.Get_XYWH().x, (mW_Y + mW_ySpeed * i * swordLength)- Camera.Get_XYWH().y, SWORD_WIDTH, SWORD_HEIGHT};
		Uint32 color = SDL_MapRGB(screen->format,  111, 0, 255);
		SDL_FillRect(screen, &offset, color);

		//min max calc
		if(offset.x < minX )
			minX = offset.x;
		if(offset.x > maxX)
			maxX = offset.x;
		if(offset.y < minY)
			minY = offset.y;
		if(offset.y > minY)
			maxY = offset.y;	
	}

	//control length of swing
	if(frameCounter < 30 && m_maxReached == false)
		frameCounter += 1;
	else
		m_maxReached = true;

	if(m_maxReached == true)
		frameCounter -= 1;


	mW_hitbox.x = minX + Camera.Get_XYWH().x;// correct for camera 
	mW_hitbox.y = minY + Camera.Get_XYWH().y;
	mW_hitbox.w = maxX - minX;
	mW_hitbox.h = maxY - minY;//fine tuning hit box


//	SDL_Rect debugHitbox = { mW_hitbox.x - Camera.Get_XYWH().x, mW_hitbox.y - Camera.Get_XYWH().y, mW_hitbox.w, mW_hitbox.h};
//	Uint32 color = SDL_MapRGB(screen->format,  111, 222, 255);
//	SDL_FillRect(screen, &debugHitbox, color);
}
#endif