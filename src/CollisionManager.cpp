#ifndef COLLISION_MANAGER_CPP
#define COLLISION_MANAGER_CPP
#include "CollisionManager.h"
#include "TileManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "WarpZone.h"
#include "Particles.h"

extern C_Camera Camera;
extern C_TileManager TileManager;
extern C_AudioManager AudioManager;
extern C_WarpZone WarpZone;//Hard coded for now
extern std::vector<C_BaseParticles*> v_Particles;
C_CollisionManager::C_CollisionManager()
{
}

C_CollisionManager::~C_CollisionManager()
{
}

void C_CollisionManager::Check_EntityVsTile(std::vector<C_BaseEntity*>* v_Entities)
{
	SDL_Rect playerPosition = {0,0,0,0};
	SDL_Rect checkHere;


	for(int i = 0; i < (*v_Entities).size(); i++){
			//temp variables to for diagonal collision detection
			float newSpeedX = (*v_Entities)[i]->Get_xSpeed();
			float newSpeedY = (*v_Entities)[i]->Get_ySpeed();
			float tempX = (*v_Entities)[i]->Get_xSpeed();
			float tempY = (*v_Entities)[i]->Get_ySpeed();

			//adjust positive values for rounding
			if(newSpeedX < 0)
				tempX = (*v_Entities)[i]->Get_xSpeed() - 1;
			if(newSpeedY < 0)
				tempY = (*v_Entities)[i]->Get_ySpeed() - 1;


			//find where player is (X, Y) and check all tiles around him
			playerPosition.x = (*v_Entities)[i]->Get_X() + (*v_Entities)[i]->Get_xSpeed();//use position of where player is trying to go
			playerPosition.y = (*v_Entities)[i]->Get_Y() + (*v_Entities)[i]->Get_ySpeed(); //repeat with another tile to make sure bottom of player isn't clipping
			if(playerPosition.x < 0 )//bounds check
				playerPosition.x = 0;
			if(playerPosition.y < 0 )//bounds check
				playerPosition.y = 0;
			playerPosition.w = PLAYER_WIDTH; //TODO  change to get_width() function so it's accurate for each entity
			playerPosition.h = PLAYER_HEIGHT;

			checkHere.x = playerPosition.x - PLAYER_WIDTH*2 - 1;
			checkHere.y = playerPosition.y - PLAYER_HEIGHT*2 - 1;
			if(checkHere.x < 0 )//bounds check
				checkHere.x = 0;
			if(checkHere.x < 0 )//bounds check
				checkHere.x = 0;
			for(int x = 0; x < 4; x++){
				checkHere.x += 16;
				checkHere.y = playerPosition.y - PLAYER_HEIGHT*2 - 1;
				for(int y = 0; y < 4; y++){		
					checkHere.y += 16;
					if(checkHere.x >= LEVEL_WIDTH)
						checkHere.x = LEVEL_WIDTH - 1;//LEVEL_WIDTH;
					if(checkHere.x < 0)
						checkHere.x = 0;
					if(checkHere.y >= LEVEL_HEIGHT)
						checkHere.y = LEVEL_HEIGHT - 1;//LEVEL_HEIGHT;
					if(checkHere.y < 0)
						checkHere.y = 0;//0
					if(CollisionChecker.Check(&playerPosition, TileManager.Get_tileOffset(checkHere.x/TILE_SIZE, checkHere.y/TILE_SIZE)) && (TileManager.Get_tileSolid(checkHere.x/TILE_SIZE, checkHere.y/TILE_SIZE) == true) ){//if there is a collision while moving right
						//IF PLAYER IS MOVING IN A DIAGONAL
						if((*v_Entities)[i]->Get_xSpeed() != 0 && (*v_Entities)[i]->Get_ySpeed() != 0){

							//CHECK UP & DOWN
							float axisOffset = (*v_Entities)[i]->Get_X() + (*v_Entities)[i]->Get_xSpeed();//(playerPosition.x);
							playerPosition.x = axisOffset - tempX; //subtract X magnitude and see if there's a collision on Y axis				
							if(CollisionChecker.Check(&playerPosition, TileManager.Get_tileOffset(checkHere.x/TILE_SIZE, checkHere.y/TILE_SIZE)) && (TileManager.Get_tileSolid(checkHere.x/TILE_SIZE, checkHere.y/TILE_SIZE) == true) ){
								newSpeedY = 0;	
								(*v_Entities)[i]->Set_hittingWall(true);//if there's a collision let A.I. know it hit a wall
							}
							playerPosition.x = axisOffset;					//reset Where player is trying to go

							//CHECK LEFT & RIGHT
							axisOffset = (*v_Entities)[i]->Get_Y() + (*v_Entities)[i]->Get_ySpeed();
							playerPosition.y = axisOffset - tempY; //subtract Y magnitude and see if there's a collision on X axis
							if(CollisionChecker.Check(&playerPosition, TileManager.Get_tileOffset(checkHere.x/TILE_SIZE, checkHere.y/TILE_SIZE)) && (TileManager.Get_tileSolid(checkHere.x/TILE_SIZE, checkHere.y/TILE_SIZE) == true) ){
								newSpeedX = 0;
								(*v_Entities)[i]->Set_hittingWall(true);//if there's a collision let A.I. know it hit a wall
							}
							playerPosition.y = axisOffset;					//reset Where player is trying to go
						}
						else{
							//IF PLAYER IS NOT MOVING DIAGONAL
							if((*v_Entities)[i]->Get_xSpeed() != 0 || (*v_Entities)[i]->Get_ySpeed() != 0){
								newSpeedX = 0;
								newSpeedY = 0;	
								(*v_Entities)[i]->Set_hittingWall(true);//if there's a collision let A.I. know it hit a wall
							}
						}
					}
				}
			}	
			(*v_Entities)[i]->Set_xSpeed(newSpeedX);
			(*v_Entities)[i]->Set_ySpeed(newSpeedY);	
	}//End of big loop... TODO clean up
			
}

void C_CollisionManager::Check_EntityVsObjects(std::vector<C_BaseEntity*>* v_Entities)
{
	SDL_Rect playerPosition = {0,0,0,0};
	SDL_Rect checkHere;
	//find where player is (X, Y) and check all tiles around him
	playerPosition.x = (*v_Entities)[0]->Get_X() + (*v_Entities)[0]->Get_xSpeed();//use position of where player is trying to go
	playerPosition.y = (*v_Entities)[0]->Get_Y() + (*v_Entities)[0]->Get_ySpeed(); //repeat with another tile to make sure bottom of player isn't clipping
	if(playerPosition.x < 0 )//bounds check
		playerPosition.x = 0;
	if(playerPosition.y < 0 )//bounds check
		playerPosition.y = 0;
	playerPosition.w = PLAYER_WIDTH;
	playerPosition.h = PLAYER_HEIGHT;

	checkHere.x = playerPosition.x - PLAYER_WIDTH*2 - 1;
	checkHere.y = playerPosition.y - PLAYER_HEIGHT*2 - 1;
	if(checkHere.x < 0 )//bounds check
		checkHere.x = 0;
	if(checkHere.x < 0 )//bounds check
		checkHere.x = 0;
	for(int x = 0; x < 4; x++){//START CHECKING COLLISIONS
		checkHere.x += 16;
		checkHere.y = playerPosition.y - PLAYER_HEIGHT*2 - 1;
		for(int y = 0; y < 4; y++){		
			checkHere.y += 16;
			if(checkHere.x >= LEVEL_WIDTH)
				checkHere.x = 0;//LEVEL_WIDTH;
			if(checkHere.y >= LEVEL_HEIGHT)
				checkHere.y = 0;//LEVEL_HEIGHT;
			for(int i = 0; i < TileManager.Get_WarpCount(); i++)
				if(CollisionChecker.Check(&playerPosition, &TileManager.Get_WarpPosition(i)) ){//if player touches a warp zone
					//lock player controls


					//if not already started... start a transition
					if(TileManager.Transition_GetStatus()== false && m_waitingForTransition == false){
						TileManager.Transition_Start(FADE_OUT);//start fading out
						m_waitingForTransition = true;//keep transition from relooping
					}


					//zone somewhere
					if(TileManager.Transition_GetStatus() == false){//once transition is over load new map
						(*v_Entities)[0]->Set_X(TileManager.Get_WarpDestination(i).x);//read where player will end up
						(*v_Entities)[0]->Set_Y(TileManager.Get_WarpDestination(i).y);
						
						switch(TileManager.Get_WarpType(i)){
							case OVERWORLD: TileManager.Set_warpUsed(i);
											TileManager.LoadMainBuffer();
											break;
							case DUNGEON:	TileManager.Set_warpUsed(i);
											TileManager.LoadMainBuffer();//TODO CREATE LOAD DUNGEON
											break;
							case CAVE:		TileManager.Set_warpUsed(i);
											TileManager.LoadCave();
											break;
						}
					
						m_waitingForTransition = false;
					}
				}
		}
	}			
			
}

void C_CollisionManager::Check_PlayerVsEnemy(std::vector<C_BaseEntity*>* v_Entities, C_GUI* p_GUI)
{
	for(int i = 1; i < (*v_Entities).size(); i++){
		//check player vs enemy collisions
		if(CollisionChecker.Check((*v_Entities)[0]->Get_hitbox(), (*v_Entities)[i]->Get_hitbox()))//if player an enemy collide
			if((*v_Entities)[0]->Set_hitPoints((*v_Entities)[0]->Get_hitPoints() - 1)) //take some damage ( - 1 dmg for now)
				p_GUI->StartDisplayDmg( *(*v_Entities)[0]->Get_hitbox(),  -1);//if taking dmg show the dmg being taken
	}
}

void C_CollisionManager::Check_WeaponVsEntity(std::vector<C_BaseEntity*>* v_Entities)
{

	for(int i = 1; i < (*v_Entities)[0]->Weapon_Get_count(); i++)//look through each weapon player has out
		for(int j = 1; j < (*v_Entities).size(); j++)//then for each enemy
 			if(CollisionChecker.Check((*v_Entities)[0]->Weapon_Get_hitbox(i), (*v_Entities)[j]->Get_hitbox())){//testing!if player weapon kills enemies
 				C_BaseEntity* tempEntity = (*v_Entities)[j];//FREE MEMORY for enemy being deleted
				delete tempEntity;
				(*v_Entities).erase((*v_Entities).begin() + j);
			}
				//check if weapon is friendly dmg or not


	//assign dmg accordingly and let individual classes handle how entities react
}
void C_CollisionManager::Check_WeaponVsWeapon(std::vector<C_BaseEntity*>* v_Entities)
{

	for(int i = 0; i < (*v_Entities).size(); i++)//For each player
		for(int j = 0; j < (*v_Entities)[i]->Weapon_Get_count(); j++)//Look through each of their weapons
		//AND COMPARE AGAINST THE SAME
			for(int ii = 0; ii < (*v_Entities).size(); ii++)//For each player
				for(int jj = 0; jj < (*v_Entities)[ii]->Weapon_Get_count(); jj++)//Look through each of their weapons
				//THEN COMPARE TWO WEAPONS IF THEY COLLIDE
					if(i != ii || j != jj)//don't compare a weapon against itself
 						if(CollisionChecker.Check((*v_Entities)[i]->Weapon_Get_hitbox(j), (*v_Entities)[ii]->Weapon_Get_hitbox(jj))){//weapon vs weapon collision test
							//**** NEED TO CHECK WEAPON PROPERTIES
							
							if((*v_Entities)[i]->Weapon_Get_finished(j) == false || (*v_Entities)[ii]->Weapon_Get_finished(jj) == false)//do nothing if rechecking the same weapon
								switch(CompareWeaponProperties((*v_Entities)[i]->Weapon_Get_destructProperties(j), (*v_Entities)[ii]->Weapon_Get_destructProperties(jj))){
								
								case DO_NOTHING:		break;//do nothing
								case DESTROY_WEAPON1: (*v_Entities)[i]->Weapon_KillWeapon(j);
														for(int p = 0; p < 150; p++){//MAKE PARTICLES
															C_BaseParticles* tempParticle = new C_BaseParticles;
															tempParticle->Start((*v_Entities)[i]->Weapon_Get_hitbox(j)->x, (*v_Entities)[i]->Weapon_Get_hitbox(j)->y);
															v_Particles.push_back(tempParticle);
														}
														break;
								case DESTROY_WEAPON2: (*v_Entities)[ii]->Weapon_KillWeapon(jj);
														for(int p = 0; p < 150; p++){//MAKE PARTICLES
															C_BaseParticles* tempParticle = new C_BaseParticles;
															tempParticle->Start((*v_Entities)[ii]->Weapon_Get_hitbox(jj)->x, (*v_Entities)[i]->Weapon_Get_hitbox(j)->y);
															v_Particles.push_back(tempParticle);
														}

								case DESTROY_BOTH:	(*v_Entities)[i]->Weapon_KillWeapon(j);
														for(int p = 0; p < 150; p++){//MAKE PARTICLES
															C_BaseParticles* tempParticle = new C_BaseParticles;
															tempParticle->Start((*v_Entities)[i]->Weapon_Get_hitbox(j)->x, (*v_Entities)[i]->Weapon_Get_hitbox(j)->y);
															v_Particles.push_back(tempParticle);
														}  
													(*v_Entities)[ii]->Weapon_KillWeapon(jj);
														for(int p = 0; p < 150; p++){//MAKE PARTICLES
															C_BaseParticles* tempParticle = new C_BaseParticles;
															tempParticle->Start((*v_Entities)[ii]->Weapon_Get_hitbox(jj)->x, (*v_Entities)[i]->Weapon_Get_hitbox(j)->y);
															v_Particles.push_back(tempParticle);
														}
														break;
								//case CM_DEFLECT_NORMAL_W1: (*v_Entities)[i]->

								}
														
						}
						//check if weapon is friendly dmg or not


	//assign dmg accordingly and let individual classes handle how entities react
}
int C_CollisionManager::CompareWeaponProperties(int weapon1, int weapon2)
{
	
	int resolution = DO_NOTHING;//starts as this and changes when things are compared
	//Read flags from weapon 1 & 2


	//Compare flags for situations
	//if both are indestructible
	if(BitManip.Get_bit(weapon1, INDESTRUCTIBLE) && BitManip.Get_bit(weapon2, INDESTRUCTIBLE))
		resolution = DO_NOTHING;

	if(BitManip.Get_bit(weapon1, ALWAYS_DESTRUCTIBLE) && BitManip.Get_bit(weapon2, INDESTRUCTIBLE))
		resolution = DESTROY_WEAPON1;

	if(BitManip.Get_bit(weapon1, INDESTRUCTIBLE) && BitManip.Get_bit(weapon2, ALWAYS_DESTRUCTIBLE))
		resolution = DESTROY_WEAPON2;

	if(BitManip.Get_bit(weapon1, ALWAYS_DESTRUCTIBLE) && BitManip.Get_bit(weapon2, ALWAYS_DESTRUCTIBLE))
		resolution = DESTROY_BOTH;

	if(BitManip.Get_bit(weapon1, WEAK_VS_SHIELD) && BitManip.Get_bit(weapon2, STRONG_VS_LIGHT_PROJECTILES))
		resolution = DESTROY_WEAPON1;
	
	if(BitManip.Get_bit(weapon1, STRONG_VS_LIGHT_PROJECTILES) && BitManip.Get_bit(weapon2, WEAK_VS_SHIELD))
			resolution = DESTROY_WEAPON2;
	
	if(BitManip.Get_bit(weapon1, DEFLECTABLE) && BitManip.Get_bit(weapon2, DEFLECT_BACK_NORMAL))
		resolution = CM_DEFLECT_NORMAL_W1;
	
	if(BitManip.Get_bit(weapon1, DEFLECT_BACK_NORMAL) && BitManip.Get_bit(weapon2, DEFLECTABLE))
			resolution = CM_DEFLECT_NORMAL_W2;
	//DO NOTHING, DESTROY_WEAPON, DAMAGE_WEAPON, BOUNCE_BACK
	return resolution;
}
int C_CollisionManager::Compare_WeaponVsEntity(int weapon, int entity)
{
	
	int resolution = DO_NOTHING;//starts as this and changes when things are compared
	//Read flags from weapon 1 & player

	//if weapon type hurts player
	if(BitManip.Get_bit(weapon, DAMAGE_PLAYER) && BitManip.Get_bit(entity, ENTITY_PLAYER_TYPE))
		resolution = DAMAGE_PLAYER;


	//DO NOTHING, DESTROY_WEAPON, DAMAGE_WEAPON, BOUNCE_BACK
	return resolution;
}
#endif