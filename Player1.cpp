#ifndef PLAYER1_CPP
#define PLAYER1_CPP
#include "Player1.h"
#include "InputControls.h"
#include "TileManager.h"
extern SDL_Surface *screen;
extern SDL_Event event;
extern C_InputControls Controls;
extern C_Camera Camera;
extern C_TileManager TileManager;

//temp?
extern bool mapMode;
C_Player1::C_Player1()
{
	//FLAGS
	BitManip.Set_bit(m_flags, ENTITY_PLAYER_TYPE, ON);//ONLY USE 'g_entityFlags' FROM CONFIG

	//Init stuff
	Set_X(256);
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
	m_xMaxSpeed = 2;
	m_yMaxSpeed = 2;

	m_hitPoints = 100;
	m_takingDamage = false;
	damagedTimePassed = 0;

	m_lockControls = false;//can move by default


	PlayerTimer.start();

	Weapons.clear();//wtf why needed
}

C_Player1::~C_Player1()
{
}

void C_Player1::Render()
{
	SDL_Rect offset = {Get_X() - Camera.Get_XYWH().x, Get_Y() - Camera.Get_XYWH().y, PLAYER_WIDTH, PLAYER_HEIGHT};
	Uint32 color = SDL_MapRGB(screen->format,  255, 0, 0);
	SDL_FillRect(screen, &offset, color);
}

void C_Player1::HandleInputs(C_InputControls *Input)
{
	Set_xvel(0);
	Set_yvel(0);

	//Check for inputs
	if(Input->up){
		Set_yvel(-m_fixedYvel);

			Controls.key_S = true;//simulate shield
	}
	if(Input->down){
		Set_yvel(m_fixedYvel);//increase velocity down direction by .5 pixels/sec

			Controls.key_S = true;//simulate shield
	}
	if(Input->left){
		Set_xvel(-m_fixedXvel);

		Controls.key_S = true;//simulate shield
	}
	if(Input->right){
		Set_xvel(m_fixedXvel);

		Controls.key_S = true;//simulate shield
	}

	//UP
	if(!Input->up && Get_ySpeed() < 0){//if not pressing up but still moving up
			Set_yvel(m_fixedYvel/2);//create gradual slowdown
		if(Get_ySpeed() + Get_yvel() > 0){//if starting to move the other way or just really slow
				Set_yvel(0);//stop velocity
				Set_ySpeed(0);
		}
	}
	//DOWN
	else{
		if(!Input->down && Get_ySpeed() > 0){//if not pressing down but still moving down
			if(Get_ySpeed() > 0)//if still moving right after letting go of right
			  Set_yvel(-m_fixedYvel/2);//create gradual slowdown
			if(Get_ySpeed() + Get_yvel() < 0){//if starting to move the other way or just really slow
				Set_yvel(0);//stop velocity
				Set_ySpeed(0);
			}
		}
	}
	
	//LEFT
	if(!Input->left && Get_xSpeed() < 0){//if not pressing left but still moving left		
		if(m_xvel >= -.2)
			Set_xvel(m_fixedXvel/2);//create gradual slowdown
		if(Get_xSpeed() + Get_xvel() > 0){//if starting to move the other way or just really slow
				Set_xvel(0);//stop velocity
				Set_xSpeed(0);
		}
	}
	//RIGHT
	if(!Input->right && Get_xSpeed() > 0 || m_xvel <= .2)//if not pressing right but still moving right
		if(Get_xSpeed() > 0){//if still moving right after letting go of right
			Set_xvel(-m_fixedXvel/2);//create gradual slowdown
		if(Get_xSpeed() + Get_xvel() < 0){//if starting to move the other way or just really slow
			Set_xvel(0);//stop velocity
			Set_xSpeed(0);
		}
	}
	

	//FIND ANGLE PLAYER IS FACING
	m_angleFacing = GetAngleFacing();

	//WEAPONS TESTING********************************
	//BOOMERANG
	if(Input->space)// && Weapons.size() < 1){// if space is pressed AND no boomerangs are on screen
		if(Get_ActiveWeaponCount(BOOMERANG) < 5){//check how many boomerangs are active
			C_BaseWeapon *B_Rang = new C_Boomerang(&m_X, &m_Y, &m_angleFacing);//create boomerang object
			Weapons.push_back(B_Rang);
		}
	//SWORD
	if(Input->key_A)//Left shift does sword attack
		if(Get_ActiveWeaponCount(SWORD) < 1){//Active sword attacks
			C_BaseWeapon *Sword = new C_Sword(&m_X, &m_Y, &m_angleFacing);//create sword object
			Weapons.push_back(Sword);
		}
	//SHIELD
	if(Get_ActiveWeaponCount(SHIELD) < 1){//1 shield, always keep out when active
		C_BaseWeapon *Shield = new C_Shield(&m_X, &m_Y, &m_angleFacing);//create sword object
		Weapons.push_back(Shield);
		}


	

	m_xSpeed += m_xvel;//add velocity to speed
	m_ySpeed += m_yvel;//add velocity to speed

	//Calculate direction facing
	if(m_xSpeed > 0)
		m_directionFacing = RIGHT;
	if(m_ySpeed > 0)
		m_directionFacing = DOWN;
	if(m_xSpeed < 0)
		m_directionFacing = LEFT;
	if(m_ySpeed < 0)
		m_directionFacing = UP;
	
	if(m_xSpeed > 0 && m_ySpeed > 0)
		m_directionFacing = DOWN_RIGHT;	
	if(m_ySpeed > 0 && m_xSpeed < 0)
		m_directionFacing = DOWN_LEFT;
	if(m_ySpeed < 0 && m_xSpeed < 0)
		m_directionFacing = UP_LEFT;
	if(m_ySpeed < 0 && m_xSpeed > 0)
		m_directionFacing = UP_RIGHT;

	//TAKING DAMAGE----------------
	
	if(PlayerTimer.get_ticks() - damagedTimePassed > 250 && m_takingDamage){//stop taking dmg after this
		m_xSpeed = 0;
		m_ySpeed = 0;
		m_takingDamage = false;		
		Controls.lockControls = false;
	}

	if(m_takingDamage){
		Controls.lockControls = true;
		m_xSpeed = -m_xSpeed;
		m_ySpeed = -m_ySpeed;

		int knockBack = m_directionFacing + 4;
		if(knockBack > 7)
			knockBack -= 7; //this just calculates the opposite direction

		switch(knockBack){
		case UP:		 m_ySpeed = 2;
						 m_directionFacing = UP;
				 break;
		case UP_RIGHT:	m_xSpeed = -2;
						m_ySpeed = 2;
						m_directionFacing = UP_RIGHT;
				 break;
		case RIGHT:		m_xSpeed = -2;
						m_directionFacing = RIGHT;
				 break;
		case DOWN_RIGHT:m_xSpeed = -2;
						m_ySpeed = -2;
						m_directionFacing = DOWN_RIGHT;
				 break;
		case DOWN:		m_ySpeed = -2;
						m_directionFacing = DOWN;
				 break;
		case DOWN_LEFT: m_xSpeed = 2;
						m_ySpeed = -2;
						m_directionFacing = DOWN_LEFT;
				 break;
		case LEFT:		m_xSpeed = 2;
						m_directionFacing = LEFT;
				 break;
		case UP_LEFT:	m_xSpeed = 2;
						m_ySpeed = 2;
						m_directionFacing = UP_LEFT;
				 break;



		}
	}


	
}
void C_Player1::Move()
{
	//MOVE PLAYER/////////////////////
	

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



				

	//***************************************************ZONING STUFF
			//  16 X 8  sized map!!!!


	if(m_Y < 10 && TileManager.Get_transitionDirection() == -1)//ZONE UP if not already zoning  // THE -1 means no transition is happening
		if(TileManager.Get_mapPosition().y != 1){//if not at the top
			TileManager.MapTransition(UP);	
			SDL_Rect updateMapPosition = {TileManager.Get_mapPosition().x, TileManager.Get_mapPosition().y - 1};
			TileManager.Set_mapPosition(updateMapPosition);
		}
	if(m_Y > 930 && TileManager.Get_transitionDirection() == -1)//ZONE DOWN if not already zoning
		if(TileManager.Get_mapPosition().y != 4){//if not at the bottom (16 x 8 map)
			TileManager.MapTransition(DOWN);	
			SDL_Rect updateMapPosition = {TileManager.Get_mapPosition().x, TileManager.Get_mapPosition().y + 1};
			TileManager.Set_mapPosition(updateMapPosition);
		}
	if(m_X < 10 && TileManager.Get_transitionDirection() == -1)//zone LEFT if not already zoning
		if(TileManager.Get_mapPosition().x != 1){//if not at far left
			TileManager.MapTransition(LEFT);	
			SDL_Rect updateMapPosition = {TileManager.Get_mapPosition().x - 1, TileManager.Get_mapPosition().y};
			TileManager.Set_mapPosition(updateMapPosition);
		}
	if(m_X == 1250 && TileManager.Get_transitionDirection() == -1)//zone RIGHT if not already zoning
		if(TileManager.Get_mapPosition().x != 8){//if not at far right
			TileManager.MapTransition(RIGHT);
			SDL_Rect updateMapPosition = {TileManager.Get_mapPosition().x + 1, TileManager.Get_mapPosition().y};
			TileManager.Set_mapPosition(updateMapPosition);
		}


	if(TileManager.Get_transitionDirection() == UP){
		m_Y = TileManager.Get_offsetCounterY();
		if(m_Y > SCREEN_HEIGHT - PLAYER_HEIGHT )//keep player from going into another transition
			m_Y = LEVEL_HEIGHT - PLAYER_HEIGHT - 11;
	}
	if(TileManager.Get_transitionDirection() == DOWN){
		m_Y = LEVEL_HEIGHT + TileManager.Get_offsetCounterY();
		if(m_Y < LEVEL_HEIGHT - SCREEN_HEIGHT + PLAYER_HEIGHT/2)
			m_Y = 11;
	}
	if(TileManager.Get_transitionDirection() == LEFT){
		m_X = TileManager.Get_offsetCounterX();
		if(m_X > SCREEN_WIDTH - PLAYER_WIDTH/2)
			m_X = LEVEL_WIDTH - PLAYER_WIDTH -11;
	}
	if(TileManager.Get_transitionDirection() == RIGHT){
		m_X = LEVEL_WIDTH + TileManager.Get_offsetCounterX();
		if(m_X < LEVEL_WIDTH - SCREEN_WIDTH + PLAYER_WIDTH/3)
			m_X = 11;
	}
	

}
bool C_Player1::Set_hitPoints(int newValue)
{
	if(m_takingDamage == false){
		m_hitPoints = newValue;
		if(m_hitPoints < 0)
			m_hitPoints = 0;

		m_takingDamage = true;
		PlayerTimer.start();//reset timer
		damagedTimePassed = PlayerTimer.get_ticks();

		AudioManager.PlaySFX(2, 0);
	}

	return m_takingDamage;
}
#endif