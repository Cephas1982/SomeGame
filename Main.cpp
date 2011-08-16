#ifndef MAIN_CPP
#define MAIN_CPP
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include <iostream>
#include <vector>
#include "Config.h"
#include "Camera.h"
#include "Collision.h"
#include "AudioManager.h"
#include "TileManager.h"
#include "ImageList.h"
#include "Timer.h"
#include "Player1.h"
#include "Enemy1.h"
#include "InputControls.h"
#include "BaseWeapon.h"
#include "Boomerang.h"
#include "MapEditor.h"
#include "CollisionManager.h"
#include "WarpZone.h"
#include "BackgroundManager.h"
#include "GUI.h"
#include "Particles.h"


//GLOBALS
SDL_Surface *screen; //extern screen      <- to make global
Timer GameTimer;
SDL_Event event;//The event structure
bool exitGame = false;
bool MapEditorActive = false;
bool MapEditorMenuActive = false;
C_InputControls Controls;
C_Camera Camera;
C_AudioManager AudioManager; 
C_TileManager TileManager;
C_BackgroundManager BackgroundManager;
C_Collision Collision;
C_CollisionManager CollisionManager;
std::vector <C_BaseParticles*> v_Particles;


//Global place holder for images/tileset
C_ImageList LevelAssets;
C_ImageList StaticBGs;
C_ImageList BackgroundImg;
C_ImageList GUI_Img;


//temp stuff
Uint32 TimePassed = 0;
Uint32 FPSCounter = 0;
int totalFrames = 0;
int frames = 0;
bool playMusic = false;
bool printScreenReady = false;
int printCount = 0;//used to track how many screens printed
bool mapMode  = false;//SUPER TEMP todo: move into a class 
SDL_Surface *zoomingMap = NULL;
float tempMouseX = 1;
float tempMouseY = 1;


int main( int argc, char* args[] )
{
	//Local variables
	

    screen = new SDL_Surface;
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    } 
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }   
	//Initialize SDL_mixer 
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){ 
			return false;
	}

	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set window caption

	//Display Window txt	
	char fpsTxt[4];
		 
	SDL_WM_SetCaption( fpsTxt, NULL );

	//LOAD IMAGES FOR WHATEVER NEEDED
	LevelAssets.LoadImage("Tiles\\Tiles.png");

	StaticBGs.LoadImage("Tiles\\Static Backgrounds\\OG_overworldBG.png");
	StaticBGs.LoadImage("Tiles\\Static Backgrounds\\Grass.png");
	StaticBGs.LoadImage("Tiles\\Static Backgrounds\\Darkness.png");

	BackgroundImg.LoadImage("Tiles\\Background Objects\\bgObjects.png");

	GUI_Img.LoadImage("Screenshots\\world map.png");
	GUI_Img.LoadImage("Screenshots\\YoureHere_down.png");
	
	//INIT SOUND 
	AudioManager.LoadMusic(); //loads music need to make this better once able to change maps
	AudioManager.LoadSFX();

	//LOAD TILE DATA
	TileManager.LoadMainBuffer();//hard coded for now, will change later
	 
	//LOAD BACKGROUND LAYERS
	//BackgroundManager.Load();//  loading from tilemanager for now

	//MAKE ENTITY VECTOR
	std::vector<C_BaseEntity*> v_Entities;//CREATE PLAYER OBJECT
		//make 1 player for now
		C_BaseEntity *Player = new C_Player1;
		v_Entities.push_back(Player);
		//Make enemies
		for(int i = 0; i < 1; i++){
			C_BaseEntity *Enemy = new C_Enemy1;
			v_Entities.push_back(Enemy);
		}
	//INIT GUI
	C_GUI GUI;

	//INIT CAMERA
	Camera.SetCamera(v_Entities[0]->Get_p_X(), v_Entities[0]->Get_p_Y());//set it to track the player


	//INIT MAP EDITOR
	C_MapEditor MapEditor(&TileManager, &v_Entities, &GUI);



	// THE GAME LOOP
	GameTimer.start();
	while(!exitGame){
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			exitGame = true;

		//SETUP PRINT SCREEN
		if(Controls.key_print || printScreenReady){
			int x, y;
			switch(printCount){
			case 0:	x = 0;
					y = 0;
					break;
			case 1:	x = 1280;
					y = 0;
					break;
			case 2:	x = 0;
					y = 960;
					break;
			case 3:	x = 1280;
					y = 960;
					break;
			}
			Camera.SetCamera(&x, &y);
			printScreenReady = true;
		}


		//HANDLE INPUTS AND A.I. AND UPDATE MISC THINGS
		Controls.CheckInputs();
		if(MapEditorMenuActive || mapMode )//lock controls under certain conditions
			for(int i = 0; i < v_Entities.size(); i++)
				v_Entities[i]->Set_lockControls(true);//lock controls for everything
		else
			for(int i = 0; i < v_Entities.size(); i++)
				v_Entities[i]->Set_lockControls(false);//unlock controls for everything
		
		for(int i = 0; i < v_Entities.size(); i++)
			if(v_Entities[i]->Get_lockControls() == false){
				v_Entities[i]->HandleInputs(&Controls);	//handle inputs if controls are not locked
				v_Entities[i]->UpdateWeapons();
			}

		for(int i = 0; i < v_Particles.size(); i++)
			if(!v_Particles[i]->Update()){//update particles and check if alive
				C_BaseParticles* tempParticle= v_Particles[i];//IF DEAD erase memory and delete from vector
				delete tempParticle;
				v_Particles.erase(v_Particles.begin() + i);
			}
		
		//CHECK COLLISIONS
		///player collisions
		if(MapEditorActive == false){ // do not check collisions when editor is on
			CollisionManager.Check_EntityVsTile(&v_Entities);//Player vs tiles
			CollisionManager.Check_EntityVsObjects(&v_Entities);//Player vs warps
			CollisionManager.Check_PlayerVsEnemy(&v_Entities, &GUI);
			CollisionManager.Check_WeaponVsEntity(&v_Entities);
			CollisionManager.Check_WeaponVsWeapon(&v_Entities);
	}
		///enemy collisions--------
		
		//UPDATE CAMERA/////////////////////
		Camera.UpdateCamera();	

		//MOVE ENTITIES  && their weapons
		for(int i = 0; i < v_Entities.size(); i++){
			v_Entities[i]->Move();
			v_Entities[i]->MoveWeapons();
		}


		
		
		
		//DRAW STUFF// ***Higher layers are further down this list**
		TileManager.RenderStaticBackground();							//image under tiles
		TileManager.Render();											//Render background Tiles
		TileManager.Render_Warp();										//Render non-tile objects		
		BackgroundManager.RenderLayer1();								//LAYER 1


		for(int i = 0; i < v_Entities.size(); i++){						//Render players and enemies AND weapons
			v_Entities[i]->Render();	
			v_Entities[i]->RenderWeapons();
		}
					
		BackgroundManager.RenderLayer2();								//LAYER 2


		BackgroundManager.RenderLayer3();								//LAYER 3 super foreground stuff (rain, etc)

		for(int i = 0; i < v_Particles.size(); i++)
			v_Particles[i]->Render();										//Render particles TODO: make other functions to change layer

		GUI.Render_HitPoints();											//RENDER

		if(mapMode){
			GUI.Render_Map();											//Draw world map when 'M' is pressed
			tempMouseX = GUI.Get_mouseOffsetX();
			tempMouseY = GUI.Get_mouseOffsetY();
		}

		if(!mapMode && !MapEditorActive)								//GUI when not viewing map or editing GUI
			GUI.Render_PlayerStatus(&v_Entities);

		TileManager.Transitions_Render();								//Handle transitions ***KEEP AT TOP**


		//SOUNDS//////////////////
		//Insert something here to handle audio on the fly

		//EXTRAS *** Map Editor and debug
		if(Controls.tab == true)
			MapEditorActive = !MapEditorActive;//Toggle map editor

		if(MapEditorActive){
			MapEditor.Start();
			v_Entities[0]->Debug();

			//HOT KEYS
			if(Controls.key_F1){//Save map data
				TileManager.SaveMap();
				BackgroundManager.Save();
			}
			if(Controls.key_F4){
				v_Entities[0]->Set_X(256);
				v_Entities[0]->Set_Y(721);
				TileManager.StartNewGame();
				TileManager.LoadMainBuffer();
				BackgroundManager.Load();
			}
		}
		else
			MapEditor.Stop();
		
		//KEYS COMMANDS**********************************
		if(Controls.key_M){
			SDL_Rect playerPos = {(TileManager.Get_mapPosition().x - 1)* 160 + (v_Entities[0]->Get_X() / 8), (TileManager.Get_mapPosition().y - 1)* 120  + (v_Entities[0]->Get_Y() / 6)};
			GUI.Set_playerPosition(playerPos);			
			mapMode = !mapMode;
			GUI.Set_initMap(ON);
		}
		if(Controls.key_lctrl){//SPAWN 10 Enemies**********************
			for(int i = 0; i < 10; i ++){
				C_BaseEntity* Enemy = new C_Enemy1;
				v_Entities.push_back(Enemy);
			}
		}
		if(Controls.key_lctrl && Controls.key_lshift){//ERASE ALL ENEMIES******************
			for(int i = 1; i < v_Entities.size(); i++){
				C_BaseEntity* tempEntity = v_Entities[i];
				delete tempEntity;
			}
			v_Entities.erase(v_Entities.begin()+1, v_Entities.end() );//erase all
		}
		if(Controls.key_P)//PARTICLE TEST
			for(int i = 0; i < 300; i++){
				C_BaseParticles* tempParticle = new C_BaseParticles;
				tempParticle->Start(300, 300);
				v_Particles.push_back(tempParticle);
			}
		//SCREEN CAPTURE  >>>>> TODO MAKE CLASS for this
		if(printScreenReady){
			switch(printCount){
			case 0: SDL_SaveBMP(screen, "Screenshots\\test1.bmp"); 
					break;
			case 1: SDL_SaveBMP(screen, "Screenshots\\test2.bmp"); 
					break;
			case 2: SDL_SaveBMP(screen, "Screenshots\\test3.bmp"); 
					break;
			case 3: SDL_SaveBMP(screen, "Screenshots\\test4.bmp"); 
					break;
			}
			Camera.SetCamera(v_Entities[0]->Get_p_X(), v_Entities[0]->Get_p_Y());
			if(printCount++ > 3){
				printScreenReady = false;
				printCount = 0;
			}				
		}//END SCREEN CAP
		
		//Display FPS
		SDL_WM_SetCaption( fpsTxt, NULL);
		//convert timer to txt
		itoa(totalFrames, fpsTxt, 10);

		//Throttle FPS
		if(TimePassed + 1000/FPS > GameTimer.get_ticks()){//if less than 1/60 sec has passed
			frames++;//add to amount of frames passed
			//wait until 1/60th has gone by
			SDL_Delay(1000/FPS - (GameTimer.get_ticks() - TimePassed)); 
		}
		else //once 1/60 second has passed
			TimePassed = GameTimer.get_ticks();//update Time passed	

		//calculate FPS every second
		if(FPSCounter + 1000 < GameTimer.get_ticks()){
			totalFrames = frames;   //calculate frames that passed in 1 second	
			frames = 0;		//restart frame count
			FPSCounter = GameTimer.get_ticks();
		}
		
		//Flip screen buffer
		SDL_Flip(screen);
		//Clear screen
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );

	}//end game loop
	
	return 1;
}//end main

#endif