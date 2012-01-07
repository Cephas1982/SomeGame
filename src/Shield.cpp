#ifndef SHIELD_CPP
#define SHIELD_CPP
#include "Shield.h"
extern C_InputControls Controls;
extern SDL_Surface *screen;
extern C_Camera Camera;

C_Shield::C_Shield(int* Player_Xpointer, int* Player_Ypointer, int* Player_AnglePointer):C_BaseWeapon(Player_Xpointer, Player_Ypointer, Player_AnglePointer)
{
	//FLAGS --- set flag data
	BitManip.Set_bit(m_flags, INDESTRUCTIBLE, ON);

	BitManip.Set_bit(m_flags, STRONG_VS_LIGHT_PROJECTILES, ON);

	
	//Init stuff
	//mW_destructProperties = WEAK_VS_FIRE;
	mW_weaponType = SHIELD;
	p_PlayerX = Player_Xpointer;
	p_PlayerY = Player_Ypointer;
	//mW_X = *p_PlayerX + PLAYER_WIDTH/2;//origin is center of character 
	//mW_Y = *p_PlayerY + PLAYER_HEIGHT/2;//orgin is center of character
	mW_xvel = 0;
	mW_yvel = 0;
	mW_xSpeed = 0;
	mW_ySpeed = 0;
	mW_xMaxSpeed = 4;
	mW_yMaxSpeed = 4;
	mW_distanceTraveled = 0;
	mW_maxDistance = 250;
//	mW_currentAngle = float(*mW_p_playerAngle * PI / 180);//this is which way player is facing
	mW_p_playerAngle = Player_AnglePointer;//**** NEW FOR SHIELD ********
	mW_originalAngle = 90;

	stop = false;
	//Controls.lockMovement = true; //not needed for shield stuff?
	mW_turnShield = false;
	mW_strike = false;
	mW_returnShield = false;
	mW_shieldMaxed = false; //used for shield morph
	//TEMP?
	
	mW_hitbox.x = 0;
	mW_hitbox.y = 0;
	mW_hitbox.w = 0;
	mW_hitbox.h = 0;


	m_turnLeft = false;
	m_turnRight = false;

	//TESTING
	bool bob = false;
	bool bobX = .1;
	bool bobY = .1;
}

C_Shield::~C_Shield()
{
}
void C_Shield::Update()
{
	//Testing
	if(Controls.key_P)
		bob = !bob;//make shield bob up/down to simulate player walking


	//CALCULTE angle player is facing
	mW_currentAngle = float(*mW_p_playerAngle);//the always up to date angle that the player is facing

	//TURN CONTROLS
	if(Controls.key_S && mW_turnShield == false){//if a shield bash is requeted and not already bashing
		mW_turnShield = true;
		mW_originalAngle = *mW_p_playerAngle;//the direction player is facing when turn was requested
	}

/*
	//STRIKE CONTROLS
	if(Controls.key_D && mW_strike == false){//if a shield bash is requeted and not already bashing
		mW_strike = true;
		mW_originalAngle = float(*mW_p_playerAngle * PI / 180);//the direction player is facing
	}
	if(mW_strike == true && mW_currentAngle < mW_originalAngle - 45 * PI/180)//Return shield after x degrees
		mW_returnShield = true;
*/
	//FINISH CONDITIONS///////////////
	float wtf = *mW_p_playerAngle;
	float wtf2 = mW_currentAngle * 180/PI;
	float wtf3 = *mW_p_playerAngle * PI/180;


	if(mW_returnShield == true && mW_currentAngle == *mW_p_playerAngle){//Stop shield bash once returned
		mW_returnShield = false;
		mW_turnShield = false;
		mW_strike = false;
		mW_shieldMaxed = false;

		m_turnLeft = false;
	}

	//Calculate Origin
	mW_X = *p_PlayerX + PLAYER_WIDTH/2 -3;//origin is center of character 
	mW_Y = *p_PlayerY + PLAYER_HEIGHT/2 - 3;//orgin is center of character

	//temp math
	float tempAngle = *mW_p_playerAngle;
	float tempTurnAngle = mW_originalAngle;
	if( tempAngle == 0 && mW_originalAngle > 180)
		tempAngle = 360;
	if(*mW_p_playerAngle > 180 && tempTurnAngle == 0 )
		tempTurnAngle = 360;
	

	//IF TURNING SHIELD
	if(mW_turnShield && !m_turnRight && (mW_originalAngle != *mW_p_playerAngle)){//Turn LEFT
		if( m_turnLeft || tempAngle - tempTurnAngle < 0){//if turning left is faster. turn left
			mW_originalAngle -= 5;
			m_turnLeft = true;
		}
		if(mW_originalAngle < 0)//if at 0
			mW_originalAngle += 360;//reset angle
	}
	if(mW_turnShield && !m_turnLeft && (mW_originalAngle != *mW_p_playerAngle)){//Turn RIGHT
		mW_originalAngle += 5;
		m_turnRight = true;
		if(mW_originalAngle == 360)//if over 360 degrees
			mW_originalAngle = 0;//reset to 0
				
		}
//		mW_currentAngle = mW_originalAngle;
	

/*
	if(mW_returnShield && mW_turnShield){
		mW_originalAngle = mW_originalAngle - 3 * PI/180;
		mW_currentAngle = mW_originalAngle;
	}

	//IF STRIKING
	if(mW_strike && !mW_returnShield){
		mW_originalAngle = mW_originalAngle + 2 * PI/180;
		mW_currentAngle = mW_originalAngle;
	}
	if(mW_returnShield && mW_strike){
		mW_originalAngle = mW_originalAngle - 2 * PI/180;
		mW_currentAngle = mW_originalAngle;
	}
*/
	//END OF TURN REQUEST
	if(mW_turnShield == true && mW_currentAngle == mW_originalAngle && (m_turnLeft || m_turnRight)){//Stop turning shield once it's back home
		mW_turnShield = false;
		m_turnLeft = false;
		m_turnRight = false;
	}


	//get the starting angle
	float currentAngle_Radians = mW_originalAngle * PI/180;
	mW_xvel = cos(currentAngle_Radians);
	mW_yvel = sin(currentAngle_Radians);
}

void C_Shield::Move()
{

	//calculate magnitude**********NOT NEEDED FOR THIS
	mW_xSpeed = mW_xvel;//SPEED IS VECTOR MAGNITUDE
	mW_ySpeed = mW_yvel;//TODO: RENAME VARIABLE =(

}


void C_Shield::Render()
{
	
	float SHIELD_SIZE = 7;//size of each dot that creates the shield
	int distanceFromPlayer = 20;//increases magnitude of shield

	//TESTING FOR SHIELD STRIKE
	if(mW_strike && mW_returnShield){//increase shield size on hit
		if(SHIELD_SIZE < 15 && mW_shieldMaxed == false){
			mW_shieldMaxed = true;
			SHIELD_SIZE += 11;
		}
		else
			SHIELD_SIZE--;
		mW_xSpeed *= 1.3;
		mW_ySpeed *= 1.3;
	}
	
		
	//Get the shield origin
	int shieldOriginX = mW_X + mW_xSpeed * distanceFromPlayer;
	int shieldOriginY = mW_Y + mW_ySpeed * distanceFromPlayer;

	//From shield origin turn 90*
	//mW_currentAngle -= 90;
	float currentAngle_Radians = 0;
	if(mW_turnShield)
		currentAngle_Radians = (mW_originalAngle - 90) * PI/180;//R A D I A N S 
	else
		currentAngle_Radians = (mW_currentAngle - 90) * PI/180;//R A D I A N S 
	
	//get the magnitude
	if(abs(mW_xvel = cos(currentAngle_Radians)) < .001)
		mW_xvel = 0;
	if(abs(mW_yvel = sin(currentAngle_Radians)) < .001)
		mW_yvel = 0;


	//hitbox variables
	int minX = shieldOriginX - Camera.Get_XYWH()->x;
	int minY = shieldOriginY - Camera.Get_XYWH()->y;
	int maxX = shieldOriginX - Camera.Get_XYWH()->x;
	int maxY = shieldOriginY - Camera.Get_XYWH()->y;

	//TESTING-----
	if(bob && ( Controls.up || Controls.down)){
		if(bobX < 5 && !bobReturnX)
			bobX += .3;
		else
		{
			bobX -= .3;
			bobReturnX = true;
			if(bobX < -5)
				bobReturnX = false;
		}
		shieldOriginX += bobX;
		if(bobX < 2 || bobX < -2)
			SHIELD_SIZE -= bobX/2;
	}
	if(bob && ( Controls.left || Controls.right)){
		if(bobY < 5 && !bobReturnY)
			bobY += .3;
		else
		{
			bobY -= .3;
			bobReturnY = true;
			if(bobY < -5)
				bobReturnY = false;
		}
		shieldOriginY += bobY;
		if(bobY < 2 || bobY < -2)
			SHIELD_SIZE -= bobY/2;
	}

	//////// END BOBBLE STUFF

	//render
	for(int i = -2; i < 2; i++){
		SDL_Rect offset = {(shieldOriginX + mW_xvel * i * 4) - Camera.Get_XYWH()->x, (shieldOriginY + mW_yvel * i * 4) - Camera.Get_XYWH()->y, SHIELD_SIZE, SHIELD_SIZE};
		Uint32 color = SDL_MapRGB(screen->format,  111, 212, 255);
		SDL_FillRect(screen, &offset, color);


		//min & max calc
		if(offset.x < minX)
			minX = offset.x;
		if(offset.x > maxX)
			maxX = offset.x;
		if(offset.y < minY)
			minY = offset.y;
		if(offset.y > minY)
			maxY = offset.y;	
	}


	mW_hitbox.x = minX + Camera.Get_XYWH()->x ;// correct for camera 
	mW_hitbox.y = minY + Camera.Get_XYWH()->y;
	mW_hitbox.w = maxX - minX + 10;
	mW_hitbox.h = maxY - minY + 10;
	
	SDL_Rect debugHitbox = { mW_hitbox.x - Camera.Get_XYWH()->x, mW_hitbox.y - Camera.Get_XYWH()->y, mW_hitbox.w, mW_hitbox.h};
	Uint32 color = SDL_MapRGB(screen->format,  111, 222, 255);
	//SDL_FillRect(screen, &debugHitbox, color);
}


void C_Shield::Animation_Turn()
{
/*
	int SHIELD_SIZE = 7;//size of each dot that creates the shield
	int distanceFromPlayer = 30;//increases magnitude of shield

	//GET ORIGIN  FOR SHIELD
	SDL_Rect offset = {(mW_X + mW_xSpeed * distanceFromPlayer), (mW_Y + mW_ySpeed * distanceFromPlayer), SHIELD_SIZE, SHIELD_SIZE};
	Uint32 color = SDL_MapRGB(screen->format,  111, 212, 255);
	SDL_FillRect(screen, &offset, color);
		
	//Get the shield origin
	int shieldOriginX = mW_X + mW_xSpeed * distanceFromPlayer;
	int shieldOriginY = mW_Y + mW_ySpeed * distanceFromPlayer;

	//From shield origin turn 90*
	mW_currentAngle -= 90 * PI/180;

	float Degrees = mW_currentAngle * 180/PI;//FOR Watch only cause I hate radians
	
	//get the magnitude
	if(abs(mW_xvel = cos(mW_currentAngle)) < .001)
		mW_xvel = 0;
	if(abs(mW_yvel = sin(mW_currentAngle)) < .001)
		mW_yvel = 0;

	//increase in both directions
	//render
	for(int i = -4; i < 4; i++){
		SDL_Rect offset = {(shieldOriginX + mW_xvel * i * 4), (shieldOriginY + mW_yvel * i * 4), SHIELD_SIZE, SHIELD_SIZE};
		Uint32 color = SDL_MapRGB(screen->format,  111, 212, 255);
		SDL_FillRect(screen, &offset, color);
	}
	*/
}
void C_Shield::Animation_Strike()
{
	/*
		int SHIELD_SIZE = 7;//size of each dot that creates the shield
	int distanceFromPlayer = 30;//increases magnitude of shield

	//GET ORIGIN  FOR SHIELD
	SDL_Rect offset = {(mW_X + mW_xSpeed * distanceFromPlayer), (mW_Y + mW_ySpeed * distanceFromPlayer), SHIELD_SIZE, SHIELD_SIZE};
	Uint32 color = SDL_MapRGB(screen->format,  111, 212, 255);
	SDL_FillRect(screen, &offset, color);
		
	//Get the shield origin
	int shieldOriginX = mW_X + mW_xSpeed * distanceFromPlayer;
	int shieldOriginY = mW_Y + mW_ySpeed * distanceFromPlayer;

	//From shield origin turn 90*
	mW_currentAngle -= 90 * PI/180;

	float Degrees = mW_currentAngle * 180/PI;//FOR Watch only cause I hate radians
	
	//get the magnitude
	if(abs(mW_xvel = cos(mW_currentAngle)) < .001)
		mW_xvel = 0;
	if(abs(mW_yvel = sin(mW_currentAngle)) < .001)
		mW_yvel = 0;

	//increase in both directions
	//render
	for(int i = -4; i < 4; i++){
		SDL_Rect offset = {(shieldOriginX + mW_xvel * i * 4), (shieldOriginY + mW_yvel * i * 4), SHIELD_SIZE, SHIELD_SIZE};
		Uint32 color = SDL_MapRGB(screen->format,  111, 212, 255);
		SDL_FillRect(screen, &offset, color);
	}
	*/
}


#endif