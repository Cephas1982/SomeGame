#ifndef BASEWEAPON_CPP
#define BASEWEAPON_CPP
#include "BaseWeapon.h"
extern SDL_Surface *screen;
extern C_Camera Camera;
C_BaseWeapon::C_BaseWeapon(int *playerX, int *playerY, int* playerAngle)
{
	m_flags = 0; //NULL BY DEFAULT then set parameters
	
	
	/*
	//Init stuff
	p_BaseEntity = EntityPointer; //secks that pointer
	m_X = p_BaseEntity->Get_X();//getting world coords from Pointer
	m_Y = p_BaseEntity->Get_Y();
	m_xvel = 0;
	m_yvel = 0;
	m_xSpeed = 0;
	m_ySpeed = 0;
	m_xMaxSpeed = 3;
	m_yMaxSpeed = 3;
	m_distanceTraveled = 0;
	m_angle1 = p_BaseEntity->GetAngleFacing();//this is which way player is facing TODO: write that 
	m_damage = 1;
	//Set flags 
	ToggleFlags(DAMAGE_ENEMIES, true);//damage enemies (set this somewhere else later)
	ToggleFlags(WEAPON_ACTIVE, true);//make active	
	*/

	mW_hitbox.x = 0;
	mW_hitbox.y = 0;
	mW_hitbox.w = 1;
	mW_hitbox.h = 1;
}

C_BaseWeapon::~C_BaseWeapon()
{

}
void C_BaseWeapon::Update()
{
}
void C_BaseWeapon::Move()
{

}

void C_BaseWeapon::Set_flags(int FlagToToggle, bool onOff)
{
	BitManip.Set_bit(m_flags, FlagToToggle, onOff);
}
void C_BaseWeapon::Render()
{
	SDL_Rect offset = {mW_X - Camera.Get_XYWH()->x, mW_Y - Camera.Get_XYWH()->y , 20, 20};
	Uint32 color = SDL_MapRGB(screen->format,  255, 255, 0);
	SDL_FillRect(screen, &offset, color);
}
//GET FUNCTIONS///////////////////
bool C_BaseWeapon::Get_isFinished()
{
	return stop;
}

int C_BaseWeapon::Get_weaponType()
{
	return mW_weaponType;
}
SDL_Rect* C_BaseWeapon::Get_hitbox()
{
	return &mW_hitbox;
}
int C_BaseWeapon::Get_destructProperties()
{
	return m_flags;
}
float C_BaseWeapon::Get_xSpeed()
{
	return mW_xSpeed;
}
float C_BaseWeapon::Get_ySpeed()
{
	return mW_ySpeed;
}
//SET FUNCTIONS///////////////////
void C_BaseWeapon::Set_X(int newValue)
{
	mW_X = newValue;
}
void C_BaseWeapon::Set_Y(int newValue)
{
	mW_Y = newValue;
}
void C_BaseWeapon::KillWeapon()
{
	stop = true;
}
void C_BaseWeapon::Set_xSpeed(float newSpeed)
{
	mW_xSpeed = newSpeed;
}
void C_BaseWeapon::Set_ySpeed(float newSpeed)
{
	mW_ySpeed = newSpeed;
}
#endif