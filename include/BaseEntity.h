#ifndef BASEENTITY_H
#define BASEENTITY_H
#include <SDL.h>
#include <cmath>
#include <vector>
#include <sstream>
#include "Config.h"
#include "Camera.h"
#include "InputControls.h"
#include "BaseWeapon.h"
#include "Boomerang.h"
#include "Sword.h"
#include "Shield.h"
#include "TTF.h"
#include "Timer.h"
#include "BitManipulation.h"



/*
can turn multiple bits on at once w/bitwise operators 
*/


class C_BaseEntity{//pure virtual class for all things that move

	protected:
		//MOVEMENT VARIABLES
		SDL_Rect m_hitBox; //alternate holder for m_X, m_Y, height, width returns as a pointer
		int m_X, m_Y;//world coordinates
		float m_xvel, m_yvel; //velocities for amount of accelleration and maximum
		float m_fixedXvel, m_fixedYvel;//set value for velocities (easier to change later)
		float m_xSpeed, m_ySpeed; //velocity is continously added  to the speed until max velocity is reached
		float m_xMaxSpeed, m_yMaxSpeed;//duh easy
		float m_gravityOffset;//this can be used to modify gravity
		int m_angleFacing;// holds angle player is facing
		bool m_hittingWall;//true when collision w/wall occurs

		//MISC
		C_BitManip BitManip;//used to handle flags
		unsigned int m_flags;//4bytes to store flag states ***********
		Timer PlayerTimer;

		//STATUS
		int m_hitPoints;	//tracks how much life remains
		bool m_takingDamage;
		int m_directionFacing;//   UP, DOWN, etc  m_facingX, m_facingY; //these track which direction player is facing. x/y can be -1, 1
		bool m_lockControls;//true when player cannot move

		//WEAPONS
		std::vector<C_BaseWeapon*> Weapons;
		

		//Debug
		C_Font DebugFont;//make global in main so not multiple loads? TODO
	public:
		C_BaseEntity();
		virtual ~C_BaseEntity();//virtual destructor
		
		virtual void HandleInputs(C_InputControls *);//handles inputs

		virtual void UpdateWeapons();

		virtual void Move();	//handles Entity movement

		virtual void MoveWeapons();//moves weapons

		virtual void Render();//draws Entity to screen

		virtual void RenderWeapons();//duh

		virtual void RenderHitPoints();

		virtual void Debug();

		virtual void Weapon_KillWeapon(int);//stops weapon.. do particals etc later?
		
		//GET FUNCTIONS
		float GetAngleFacing();//returns which way player is facing
		int Get_X();//returns X position
		int Get_Y();//same
		int* Get_p_X();//return pointer to X. 
		int* Get_p_Y();//return pointer to Y. 
		float Get_xvel();
		float Get_yvel();
		float Get_xSpeed();
		float Get_ySpeed();
		float Get_gravityOffset();
		SDL_Rect* Get_hitbox();//returns x/y height and width of player
		SDL_Rect* Weapon_Get_hitbox(int);
		int Weapon_Get_count();//returns how many weapons in vector
		int Weapon_Get_destructProperties(int);// idestructible, weak vs ice, etc
		bool Weapon_Get_finished(int);//returns if weaopn is done or not
		unsigned short int Get_flags();

		int Get_hitPoints();
		int Get_directionFacing();
		int Get_ActiveWeaponCount(int);
		bool Get_lockControls();

		//SET FUNCTIONS
		void SetAngleFacing(float);
		void Set_X(int);
		void Set_Y(int);
		void Set_xvel(float);
		void Set_yvel(float);
		void Set_xSpeed(float);
		void Set_ySpeed(float);
		void Set_gravityOffset(float);

		void Set_flags(unsigned int);
		void Weapon_Set_flags(int, bool, int);//flag to change, on/off, element to change
		void Weapon_Reflect(float, int);//reflect a weapon, parameter is how fast is is reflected and the element to do this to

		virtual bool Set_hitPoints(int);
		void Set_directionFacing(int);
		void Set_lockControls(bool);
		void Set_hittingWall(bool);

		
	
	
		//classes built on this will have animation functions like:
		//void run();  <- this will contain all data to display running and the type is 		//based on the velocity

};

#endif