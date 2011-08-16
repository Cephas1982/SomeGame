#ifndef BASEWEAPON_H
#define BASEWEAPON_H
#include <SDL.h>
#include <math.h>
#include "Config.h"
#include "AudioManager.h"
#include "InputControls.h"
#include "Camera.h"
#include "BitManipulation.h"
extern C_AudioManager AudioManager;
extern C_InputControls Controls;


//FLAGS
#define DAMAGE_ENEMIES		1 //if on, this should inflict damage on enemies
#define DAMAGE_FRIENDLIES	2 //if on, this should damage players
#define WEAPON_ACTIVE		3 //if on, weapon is active


class C_BaseWeapon{

	protected:
		//MOVEMENT
		int mW_X, mW_Y;//world position
		float mW_xvel, mW_yvel;//how many pixels per second for velocity
		float mW_xSpeed, mW_ySpeed; //stores speeds from combined velocities
		float mW_xMaxSpeed, mW_yMaxSpeed; //used to cap speeds
		float mW_currentAngle, mW_originalAngle;//angle used for polar coordinates
		float mW_distanceTraveled; //track how far the boomerang has gone
		int mW_maxDistance; // how far it can go until returning

		//MISC VARIABLES
		unsigned int m_flags;//used to set flag status	(toggle bits)
		int mW_weaponType;//INIT this with a #define vaule matching the weapon
		int mW_damage;
		bool stop;//temp until flags are implemented
		SDL_Rect mW_hitbox;//area of weapon used to calc damages
		//Data sources
		//C_BaseEntity *p_BaseEntity;//keep a pointer to manipulate stuff
		int *p_PlayerX;
		int *p_PlayerY;


		C_BitManip BitManip;
		
	public:
		C_BaseWeapon(int *, int *, int* );//On init it gets a pointer to the BaseEntity class **********change this to get pointer to m_X, m_Y variables*************
		virtual ~C_BaseWeapon();//virtual destructor


		virtual void Update();
		virtual void Move();
		virtual void Set_flags(int, bool);// 1st parameter is which flag to toggle, 2nd is to toggle on/off
		virtual void Render();

		//GET FUNCTIONS
		int Get_X();//returns X position
		int Get_Y();//same
		bool Get_isFinished();//returns 'stop' variable for now. Later will make Get_flags() 
		int Get_weaponType();//returns #defined (see config.h) vaule of weapon. used to track count etc
		SDL_Rect Get_hitbox();//returns weapon hitbox
		int Get_destructProperties();
		float Get_xSpeed();
		float Get_ySpeed();

		//SET FUNCTIONS
		void Set_X(int);
		void Set_Y(int);
		void Set_xSpeed(float);
		void Set_ySpeed(float);
		void KillWeapon();//sets m_stop to true which eventually calls destructor
};
#endif