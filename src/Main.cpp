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
#include "Server.h"
#include "Client.h"



//GLOBALS
SDL_Surface *screen; //extern screen      <- to make global
Timer GameTimer, g_fpsTimer, g_fpsRegulator;
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
int g_ClientorServer = 1;



//inits SDL, etc.
int Main_initAll(C_Client &Client, C_Server &Server, std::vector<C_BaseEntity*> &v_Entities)
{
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

	//Initialize Network
	bool clientOn = false;
	bool serverOn = false;
	if(g_ClientorServer == SERVER)
		serverOn = true;
	if(g_ClientorServer == CLIENT)
		clientOn = true;
	Client.isActive(clientOn);
	Server.isActive(serverOn);


	//Load Entity vector
	C_BaseEntity *Player = new C_Player1;//make 1 player for now
	v_Entities.push_back(Player);
	//Make enemies
	for(int i = 0; i < 1; i++){
		C_BaseEntity *Enemy = new C_Enemy1;
		v_Entities.push_back(Enemy);
	}

	//INIT CAMERA
	Camera.SetCamera(v_Entities[0]->Get_p_X(), v_Entities[0]->Get_p_Y());//set it to track the player

	
	
	return 1;//todo: return different value based on error
}

int Main_loadImages()
{
	//LOAD IMAGES FOR WHATEVER NEEDED
	LevelAssets.LoadImage("Tiles\\Tiles.png");

	StaticBGs.LoadImage("Tiles\\Static Backgrounds\\OG_overworldBG.png");
	StaticBGs.LoadImage("Tiles\\Static Backgrounds\\Grass.png");
	StaticBGs.LoadImage("Tiles\\Static Backgrounds\\Darkness.png");

	BackgroundImg.LoadImage("Tiles\\Background Objects\\bgObjects.png");

	GUI_Img.LoadImage("Screenshots\\world map.png");
	GUI_Img.LoadImage("Screenshots\\YoureHere_down.png");
	

	
	//LOAD BACKGROUND LAYERS
	//BackgroundManager.Load();//  loading from tilemanager for now

	return 1;//todo: return value based on error
}

void Main_keyCommands(std::vector<C_BaseEntity*> &v_Entities, C_GUI &GUI, C_MapEditor &MapEditor)//this is for who knows what. Debug and laziness
{
	//KEYS COMMANDS**********************************

	//Start PRINT SCREEN  - This takes a snapshot of an entire zone. Grid 1-4 (clockwise)
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

	//Display Map
	if(Controls.key_M){
		SDL_Rect playerPos = {(TileManager.Get_mapPosition().x - 1)* 160 + (v_Entities[0]->Get_X() / 8), (TileManager.Get_mapPosition().y - 1)* 120  + (v_Entities[0]->Get_Y() / 6)};
		GUI.Set_playerPosition(playerPos);			
		mapMode = !mapMode;
		GUI.Set_initMap(ON);
	}
	//SPAWN 10 Enemies**********************
	if(Controls.key_lctrl){
		for(int i = 0; i < 10; i ++){
			C_BaseEntity* Enemy = new C_Enemy1;
			v_Entities.push_back(Enemy);
		}
	}
	//ERASE ALL ENEMIES******************
	if(Controls.key_lctrl && Controls.key_lshift){
		for(int i = 1; i < v_Entities.size(); i++){
			C_BaseEntity* tempEntity = v_Entities[i];
			delete tempEntity;
		}
		v_Entities.erase(v_Entities.begin()+1, v_Entities.end() );//erase all
	}
	//PARTICLE TEST
	if(Controls.key_P)
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
	//TODO: FIX SCREEN CAPTURE


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
}

int main( int argc, char* args[] )
{

	//Networking stuff  - Debugging for when user is client TODO: cleanup!!!!!
//	s_gamePacket tempPack;
//	std::cout << "size: " << sizeof(tempPack);
	//Setup network
	//std::cout << "Server = 1, Client = 2\n Which are you?: ";
//	while(g_ClientorServer < 1 || g_ClientorServer > 2)
//		std::cin >> g_ClientorServer;


	//local variables
    screen = new SDL_Surface;	
	C_Client Client;//net client
	C_Server Server;//net server
	std::vector<C_BaseEntity*> v_Entities;//Entity vector (holds active players and enemies)

	//INIT everything
	Main_initAll(Client, Server, v_Entities);
	//INIT GUI
	C_GUI GUI;

	//Load image dataz
	Main_loadImages();

	//Load sfx and music
	AudioManager.LoadMusic(); //loads music need to make this better once able to change maps
	AudioManager.LoadSFX();

	//Fill main tile buffer
	TileManager.LoadMainBuffer();//hard coded for now, will change later

	//INIT MAP EDITOR
	C_MapEditor MapEditor(&TileManager, &v_Entities, &GUI);


	//window caption
	char fpsTxt[4]; 
	SDL_WM_SetCaption( fpsTxt, NULL );	
	
	// THE GAME LOOP
	GameTimer.start();
	g_fpsTimer.start();

	while(!exitGame){
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			exitGame = true;

		g_fpsRegulator.start();


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
			v_Particles[i]->Render();									//Render particles TODO: make other functions to change layer

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
		//Insert something here to handle audio


		//Handles key presses, mostly for debug right now
		Main_keyCommands(v_Entities, GUI, MapEditor);
		
		//NETWORKING*******************************************************************
		//---Connect
		if(g_ClientorServer == SERVER){//if in server mode, do this
			if(!Server.m_connected)
				if(Server.FindConnection())
					printf("Server connected to client!\n\n", stdout);	
		}
		else{//If it's in client mode, do this
			if(!Client.m_connected)
				if(Client.FindConnection())
					printf("Client connected to server!\n\n", stdout);
		}
		//--Update Packet data if this is a server
		if(g_ClientorServer == SERVER){//If this is a server - update packet w/info about the HOST
			Server.s_outPacket.m_playerPosition.x = v_Entities[0]->Get_X();
			Server.s_outPacket.m_playerPosition.y = v_Entities[0]->Get_Y();
		}
		if(g_ClientorServer == CLIENT){//If this is the client - Update packet w/info about the Client (TODO: add entity when a server is detected)
			Client.s_outPacket.m_playerPosition.x = v_Entities[0]->Get_X();
			Client.s_outPacket.m_playerPosition.y = v_Entities[0]->Get_Y();
		}
		//---Send/receive packets
		if(g_ClientorServer == SERVER)
			if(Server.m_connected){
				Server.UpdateClient();
				Server.Receive();
			}
		if(g_ClientorServer == CLIENT)
			if(Client.m_connected){
				Client.ReceivePacket();
				Client.Send();
			}
		
		//Pretend to see a client(if you're the host) (player1 +20 pixels to the right, and 20 up);
		if(g_ClientorServer == CLIENT){//draw the SERVER
			SDL_Rect offset = {Client.s_inPacket.m_playerPosition.x - Camera.Get_XYWH().x, Client.s_inPacket.m_playerPosition.y - Camera.Get_XYWH().y, PLAYER_WIDTH, PLAYER_HEIGHT};
			Uint32 color = SDL_MapRGB(screen->format,  255, 255, 0);
			SDL_FillRect(screen, &offset, color);		
		}
		//Pretend to see a host (if you're the client) (player1 +20 pixels to the right, and 20 up);
		if(g_ClientorServer == SERVER){//draw the SERVER
			SDL_Rect offset = {Server.s_inPacket.m_playerPosition.x - Camera.Get_XYWH().x, Server.s_inPacket.m_playerPosition.y - Camera.Get_XYWH().y, PLAYER_WIDTH, PLAYER_HEIGHT};
			Uint32 color = SDL_MapRGB(screen->format,  255, 255, 0);
			SDL_FillRect(screen, &offset, color);		
		}


		

		//Flip screen buffer
		SDL_Flip(screen);
		//Clear screen
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );

		totalFrames++;//Increment the frame counter



		//Regulate FPS
		if(g_fpsRegulator.get_ticks() < 1000 / FPS)
			SDL_Delay( (1000 / FPS) - g_fpsRegulator.get_ticks() );

		//If a second has passed since the caption was updated
		if(g_fpsTimer.get_ticks() > 1000){
			std::stringstream caption;

			//calc fps
			caption << "Average FPS: " << totalFrames / ( g_fpsTimer.get_ticks() / 1000.f);

			//Reset caption
			SDL_WM_SetCaption( caption.str().c_str(), NULL );

			//Reset fps timer
			g_fpsTimer.start();
			totalFrames = 0;

		}

	}//end game loop
	
	return 1;
}//end main

#endif