#ifndef BOOMERANG_CPP
#define BOOMERANG_CPP
#include "Boomerang.h"
extern SDL_Surface* screen;
extern C_Camera Camera;
//inherit from BaseWeapon
C_Boomerang::C_Boomerang(int* Player_Xpointer, int* Player_Ypointer, int* playerAngle) : C_BaseWeapon(Player_Xpointer, Player_Ypointer, playerAngle)
{
	//FLAGS
	BitManip.Set_bit(m_flags, INDESTRUCTIBLE, ON);//nothing kills boomerang
	BitManip.Set_bit(m_flags, DAMAGE_ENEMY, ON);//by default these hurt enemies. Must turn off and on PLAYER DMG to swap

	//Init stuff
	mW_weaponType = BOOMERANG;//VERY IMPORTANT TO INIT THIS VARIABLE
	p_PlayerX = Player_Xpointer;
	p_PlayerY = Player_Ypointer;
	mW_X = *p_PlayerX;//getting world coords from Pointer
	mW_Y = *p_PlayerY;
	mW_xvel = 0;
	mW_yvel = 0;
	mW_xSpeed = 0;
	mW_ySpeed = 0;
	mW_xMaxSpeed = 4;
	mW_yMaxSpeed = 4;
	mW_distanceTraveled = 0;
	mW_maxDistance = 250;
	mW_currentAngle = *playerAngle;//this is which way player is facing TODO: write that 
	spin = 0;

	comeBack = false;


	//TEMP??
	stop = false;

	//SOUND!!!
	AudioManager.PlaySFX(BOOMERANG, 5);


	mW_hitbox.x = 0;
	mW_hitbox.y = 0;
	mW_hitbox.w = 0;
	mW_hitbox.h = 0;
}

C_Boomerang::~C_Boomerang()
{

}
void C_Boomerang::Update()
{
		if(comeBack == false){
			//fly in direction of mW_currentAngle (pretend right -- 0 degrees)
			mW_xvel = cos(mW_currentAngle * PI / 180); //direction of x velocity is found by:  cosine(x/r)
			mW_yvel = sin(mW_currentAngle * PI / 180);   //y velocity is found by sine(y/r)

			//calculate speed
			mW_xSpeed += mW_xvel;
			mW_ySpeed += mW_yvel;

			//Cap the speed
			if(mW_xSpeed > 0 && mW_xSpeed > mW_xMaxSpeed)//if moving right and too fast
			  mW_xSpeed = mW_xMaxSpeed;		  //cap speed
			if(mW_xSpeed < 0 && mW_xSpeed < -mW_xMaxSpeed)//if moving left and too fast
			  mW_xSpeed = -mW_xMaxSpeed;		  //cap speed

			if(mW_ySpeed > 0 && mW_ySpeed > mW_yMaxSpeed)//if moving down and too fast
			  mW_ySpeed = mW_yMaxSpeed;		  //cap speed
			if(mW_ySpeed < 0 && mW_ySpeed < -mW_yMaxSpeed)//if moving up and too fast
			  mW_ySpeed = -mW_yMaxSpeed;		  //cap speed
	}


	//UPDATE DISTANCE TRAVELED
	//distance form is  D = sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
	mW_distanceTraveled = sqrt(float ((*p_PlayerX - mW_X)*(*p_PlayerX - mW_X)+(*p_PlayerY - mW_Y)*(*p_PlayerY - mW_Y)));
	//Add #defines for BOOMERANG


	//Once at a certain distance come back to player 
	if(mW_distanceTraveled > mW_maxDistance && comeBack == false) //after a distance it will start to return
		comeBack = true;
	//GET ANGLE BETWEEN PLAYER AND BOOMERANG AND MAKE BOOMERANG COME BACK
	//TODO BANDAID
	if(comeBack == true){
		if((*p_PlayerX - mW_X) == 0)
			mW_X += 1;
		mW_currentAngle = atan(float((*p_PlayerY - mW_Y)/(*p_PlayerX - mW_X)) ); //get angle in radians
		//mW_currentAngle = atan(float((200 - mW_Y)/(300 - mW_X))); //get angle in radians
		int shit = *p_PlayerX;
		mW_xvel = cos(mW_currentAngle);// * (PI/180));//same as above but with the new angle
		mW_yvel = sin(mW_currentAngle);// * (PI/180)); 

		//calcluate where player is
		if(*p_PlayerY < mW_Y)//if player is above boomerang
			mW_ySpeed -= abs(mW_yvel);//move towards player
		if(*p_PlayerY > mW_Y)//if player is below boomerang
			mW_ySpeed += abs(mW_yvel);//move towards player
		if(*p_PlayerX < mW_X)//if player is left of boomerang
			mW_xSpeed -= abs(mW_xvel);//move towards player
		if(*p_PlayerX > mW_X)//if player is right of boomerang
			mW_xSpeed += abs(mW_xvel);//move towards player
		
	
		
		//Cap the speed
		if(mW_xSpeed > 0 && mW_xSpeed > mW_xMaxSpeed)//if moving right and too fast
		  mW_xSpeed = mW_xMaxSpeed;		  //cap speed
		if(mW_xSpeed < 0 && mW_xSpeed < -mW_xMaxSpeed)//if moving left and too fast
		  mW_xSpeed = -mW_xMaxSpeed;		  //cap speed

		if(mW_ySpeed > 0 && mW_ySpeed > mW_yMaxSpeed)//if moving down and too fast
		  mW_ySpeed = mW_yMaxSpeed;		  //cap speed
		if(mW_ySpeed < 0 && mW_ySpeed < -mW_yMaxSpeed)//if moving up and too fast
		  mW_ySpeed = -mW_yMaxSpeed;		  //cap speed
	}
	//Destroy it when it's come back to player (distance 100+ AND within a certain distance)
	//distance form is  D = sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
	if(comeBack == true){ //if it's returning
		mW_distanceTraveled = sqrt(float ((*p_PlayerX - mW_X)*(*p_PlayerX - mW_X)+(*p_PlayerY - mW_Y)*(*p_PlayerY - mW_Y)));
		if(mW_distanceTraveled < 10){//if within 10 pixels of player
			stop = true;
		}
	}
	if(abs(mW_xSpeed) < .0001)
		mW_xSpeed = 0;
	if(abs(mW_ySpeed) < .0001)
		mW_ySpeed = 0;
}

void C_Boomerang::Move()//TODO Break down into Update position for better collision detection
{
	//update boomerang position
	mW_X += mW_xSpeed;//TODO: Break these two into the move function and put the rest into "update"
	mW_Y += mW_ySpeed;
	
}

void C_Boomerang::Render()
{
	SDL_Rect offset = {mW_X - Camera.Get_XYWH().x, mW_Y - Camera.Get_XYWH().y , 2, 2};
	Uint32 color = SDL_MapRGB(screen->format,  255, 255, 0);
	//SDL_FillRect(screen, &offset, color);
	int ogx = offset.x;
	int ogy = offset.y;
	float angle = 0;
	float magX, magY;
	float b = offset.x * offset.x;
	float a = offset.y * offset.y;
	
	int minX = ogx;
	int minY = ogy;
	int maxX = ogx;
	int maxY = ogy;

	for(float angle = 0; angle < 180; angle+=.1){
		
		
		offset.x = ogx;
		offset.y = ogy;

			magX = cos((spin + angle) * PI/180) * 12;
			magY = sin((spin + angle) * PI/180) * 3;
			
			offset.x += magX;
			offset.y += magY;
		SDL_FillRect(screen, &offset, color);

		//min calc
		if(offset.x < minX)
			minX = offset.x;
		if(offset.x > maxX)
			maxX = offset.x;
		if(offset.y < minY)
			minY = offset.y;
		if(offset.y > minY)
			maxY = offset.y;	


	}
	mW_hitbox.x = minX + Camera.Get_XYWH().x;
	mW_hitbox.y = minY + Camera.Get_XYWH().y;
	mW_hitbox.w = maxX - minX;
	mW_hitbox.h = maxY - minY +5;//fine tuning hit box
	spin+= 15;


}
#endif
