#ifndef BASEENTITY_CPP
#define BASEENTITY_CPP
#include "BaseEntity.h"
extern C_Camera Camera;
extern SDL_Surface* screen;

C_BaseEntity::C_BaseEntity()
{

}

C_BaseEntity::~C_BaseEntity()
{
}		

void C_BaseEntity::HandleInputs(C_InputControls *)		//TODO: see inside function
{
	//get pointer to SDL_Event and do stuff like
	
	//if(spacebar pressed)
	//  AttackFunction() 
}
void C_BaseEntity::UpdateWeapons()
{
	for(int i = 0; i < Weapons.size(); i++){
		Weapons[i]->Update();
	}
}
void C_BaseEntity::Move()
{
}
void C_BaseEntity::MoveWeapons()
{	
	if(Weapons.size() > 0)//if weapons are in queue
		for(int i = 0; i < Weapons.size(); i++)
				Weapons[i]->Move();//move them
}
void C_BaseEntity::Render()
{

}
void C_BaseEntity::RenderWeapons()
{
	if(Weapons.size() > 0)//if weapons are in queue
		for(int i = 0; i < Weapons.size(); i++)			
			if(Weapons[i]->Get_isFinished()){//if weapon is done doing w/weapons do
				C_BaseWeapon* tempWeapon = Weapons[i];//DELETE MEMORY
				delete tempWeapon;
				Weapons.erase(Weapons.begin() + i );//erase it from the vector
			}
			else
				Weapons[i]->Render();
}
void C_BaseEntity::RenderHitPoints()
{

}
void C_BaseEntity::Weapon_Set_flags(int FlagToToggle, bool flagState, int element)
{
	Weapons[element]->Set_flags(FlagToToggle, flagState);
}

void C_BaseEntity::Debug()
{
	std::stringstream debug_output;
	debug_output << "Player position:   " << m_X << " / " << m_Y << "    Player tile position: " << m_X/16 << " / " << m_Y/16
				 << "   Camera x/y:  " << Camera.Get_XYWH()->x << " / " << Camera.Get_XYWH()->y
				 << "Hit Points: " << m_hitPoints; 

	DebugFont.Print(&debug_output, screen, 12, 0, 32);
}

void C_BaseEntity::Weapon_KillWeapon(int element)
{
	Weapons[element]->KillWeapon();
}
//GET FUNTIONS///////////////////
float C_BaseEntity::GetAngleFacing()
{
	int angle = 90;

	if(m_directionFacing == RIGHT)//0*
		angle = 0;
	else if(m_directionFacing == DOWN_RIGHT)
			angle = 45;
	else if(m_directionFacing == DOWN)//90*
			angle = 90;
	else if(m_directionFacing == DOWN_LEFT)
			angle = 135;
	else if(m_directionFacing == LEFT)//180*
			angle = 180;
	else if(m_directionFacing == UP_LEFT)
			angle = 225;
	else if(m_directionFacing == UP)//270*
			angle = 270;
	else if(m_directionFacing == UP_RIGHT)
			angle = 315;

	return angle;
}

int C_BaseEntity::Get_X()//returns value of m_X
{
	return m_X;
}
int* C_BaseEntity::Get_p_X()//returns pointer to m_X
{
	return &m_X;
}
int C_BaseEntity::Get_Y()//returns value of m_Y
{
	return m_Y;
}
int* C_BaseEntity::Get_p_Y()//returns pointer to m_Y
{
	return &m_Y;
}
float C_BaseEntity::Get_xvel()
{
	return m_xvel;
}
float C_BaseEntity::Get_yvel()
{
	return m_yvel;
}
float C_BaseEntity::Get_xSpeed()
{
	return m_xSpeed;
}
float C_BaseEntity::Get_ySpeed()
{
	return m_ySpeed;
}
float C_BaseEntity::Get_gravityOffset()
{
	return m_gravityOffset;
}
unsigned short int C_BaseEntity::Get_flags()
{
	return m_flags;
}
int C_BaseEntity::Get_hitPoints()
{
	return m_hitPoints;
}
int C_BaseEntity::Get_directionFacing()
{
	return m_directionFacing;
}
int C_BaseEntity::Get_ActiveWeaponCount(int typeOfWeapon)
{
	int number_of_active_weapons = 0;
	for(int i = 0; i < Weapons.size(); i++)
		if(Weapons[i]->Get_weaponType() == typeOfWeapon)//if the tpeOfWeapon is in the vector, add to the return value
			number_of_active_weapons++;	

	return number_of_active_weapons;
}
bool C_BaseEntity::Get_lockControls()
{
	return m_lockControls;
}
SDL_Rect* C_BaseEntity::Get_hitbox()
{
	m_hitBox.x = m_X;
	m_hitBox.y = m_Y;
	m_hitBox.w = PLAYER_WIDTH;
	m_hitBox.h = PLAYER_HEIGHT; //TODO: specificy height/width for each entity
	return &m_hitBox;
}
SDL_Rect* C_BaseEntity::Weapon_Get_hitbox(int element)
{
		return Weapons[element]->Get_hitbox();//TODO ERROR PROOF
}
int C_BaseEntity::Weapon_Get_count()
{
	return Weapons.size();
}
int C_BaseEntity::Weapon_Get_destructProperties(int element)
{
	return Weapons[element]->Get_destructProperties();
}
bool C_BaseEntity::Weapon_Get_finished(int element)
{
	return Weapons[element]->Get_isFinished();
}
void C_BaseEntity::Weapon_Reflect(float reflectSpeed, int element)
{
	Weapons[element]->Set_xSpeed(-Weapons[element]->Get_xSpeed());
	Weapons[element]->Set_ySpeed(-Weapons[element]->Get_ySpeed());
}
//SET FUNCTIONS////////////////////
void C_BaseEntity::SetAngleFacing(float newValue)
{
	//angle = newValue;
}
void C_BaseEntity::Set_X(int newValue)
{
	m_X = newValue;
}
void C_BaseEntity::Set_Y(int newValue)
{
	m_Y = newValue;
}
void C_BaseEntity::Set_xvel(float newValue)
{
	m_xvel = newValue;
}
void C_BaseEntity::Set_yvel(float newValue)
{
	m_yvel = newValue;
}
void C_BaseEntity::Set_xSpeed(float newValue)
{
	m_xSpeed = newValue;
}
void C_BaseEntity::Set_ySpeed(float newValue)
{
	m_ySpeed = newValue;
}
void C_BaseEntity::Set_gravityOffset(float newValue)
{
	m_gravityOffset = newValue;
}
void C_BaseEntity::Set_flags(unsigned int newValue)
{
	m_flags = newValue;
}
bool C_BaseEntity::Set_hitPoints(int newValue)
{
	bool functionCalled = true;
	m_hitPoints = newValue;


	return functionCalled;
}
void C_BaseEntity::Set_directionFacing(int newValue)
{
	m_directionFacing = newValue;
}

void C_BaseEntity::Set_lockControls(bool onOff)
{
	m_lockControls = onOff;
}
void C_BaseEntity::Set_hittingWall(bool onOff)
{
	m_hittingWall = onOff;
}


#endif