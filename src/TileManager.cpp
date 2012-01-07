#ifndef TILEMANAGER_CPP
#define TILEMANAGER_CPP
#include "TileManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "BackgroundManager.h"
extern SDL_Surface *screen;
extern C_AudioManager AudioManager;
extern C_ImageList LevelAssets;
extern C_ImageList StaticBGs;
extern C_Camera Camera;
extern C_Collision Collision;
extern C_CollisionManager CollisionManager;
extern C_BackgroundManager BackgroundManager;

C_TileManager::C_TileManager()
{
	m_mapTransition = false;
	offsetCounterX = 0;
	offsetCounterY = 0;
	p_mainTileBuffer = &v2d_tileList;
	p_secondTileBuffer = &v2d_tileListBuffer;

	p_mainOffset = &m_mainOffset;
	p_secondOffset = &m_bufferOffset;

	m_mapPosition.x = 4;
	m_mapPosition.y = 4;

	transitionDirection = -1;

	m_transitionCounter = 500;

	m_staticBackground = 1;

	m_newGame = true;//true until first load complete, or game reset

	m_transitionType = -1;

	m_warpUsed = -1;
}
C_TileManager::~C_TileManager()
{
}

void C_TileManager::LoadMainBuffer()
{
	Camera.Set_cameraType(OVERWORLD);
	m_staticBackground = 1;
	m_transitionType = FADE_IN;//fade in transition
	//m_transitionCounter = 500;//Rigged up counter, move inside function later
	p_mainTileBuffer->clear(); //clear both buffers when going to overworld
	p_secondTileBuffer->clear();//clear buffers

	std::fstream inFile;
	std::string fileName;
	
	/////LOAD SAVE DATA (FOR HARD RESETS AND NEW GAMES)   TODO: Get variables like health etc from here
	if(m_newGame){
		//RESET STUFF
		v_WarpZones.clear();
		m_mapPosition.x = 4;
		m_mapPosition.y = 4;//hard coded for now

		fileName = "Map Data\\save data.txt";
		inFile.open(fileName);
		//READ STUFF
		getline(inFile, fileName); //get which map to load
		inFile >> m_zoneType;		//get which type of zone
		inFile.close();
		inFile.open(fileName);//fileName);//open last save map

		//load backgrounds objects
		BackgroundManager.Load();

		m_newGame = false;
	}
	else{//PARSE ZONE TO LOAD FROM WARP ZONE
		
	//Parse Warp zone data
	m_mapPosition.x = v_WarpZones[m_warpUsed].Get_endMap().x;
	m_mapPosition.y = v_WarpZones[m_warpUsed].Get_endMap().y;
	m_zoneType = v_WarpZones[m_warpUsed].Get_warpType();
	if(m_zoneType == DUNGEON)
		m_staticBackground = 2;


	char A[3];
	char B[3];
	itoa(m_mapPosition.x, A, 10);
	itoa(m_mapPosition.y, B, 10);
	//FORMAT the first map value
	if(m_mapPosition.x < 10){
		A[2] = A[1];
		A[1] = A[0];
		A[0] = '0';
	}
	if(m_zoneType == OVERWORLD)
		fileName = "Map Data\\Over world\\";
	if(m_zoneType == DUNGEON)
		fileName = "Map Data\\Dungeons\\"; //CHANGE TO DUNGEON_1, DUNGEON_2, etc for zone types
	fileName += A;
	fileName += "-0";
	fileName += B;
	fileName += ".txt";
	inFile.open(fileName); //open file to read   FORMAT IS (mapX - mapY . txt)
	
	}
	v_WarpZones.clear();//vector not needed, clear it out
	

	C_Tiles currentTile;//Temp file to read data to
	currentTile.Set_tileWidth(TILE_SIZE);
	currentTile.Set_tileHeight(TILE_SIZE);

	//Variables to read from file
	int type, destruct, solid, friendlydmg, enemydmg;

	inFile >> m_mapWidth;
	inFile >> m_mapHeight;

	//push back tiles in row major order (left to right, top to bottom)
	for(int i = 0; i < m_mapWidth/TILE_SIZE; i++){
		(*p_mainTileBuffer).push_back( std::vector<C_Tiles>() ); //Push a C_Tiles vector
		for(int j = 0; j < m_mapHeight/TILE_SIZE; j++){
			currentTile.Set_tileX(i * TILE_SIZE);
			currentTile.Set_tileY(j * TILE_SIZE);
			inFile >> type;
			inFile >> destruct;
			inFile >> solid;
			inFile >> friendlydmg;
			inFile >> enemydmg;

			//TEMP TO TEST OUT COLLISION
			if(type != -1)
				solid = 1;
			else
				solid = 0;


			currentTile.Set_tileType(type);
			currentTile.Set_destructable(destruct);
			currentTile.Set_solid(solid);
			currentTile.Set_friendlyDamage(friendlydmg);
			currentTile.Set_enemyDamage(enemydmg);
			if(type == 12 || type == 46)
				currentTile.Set_solid(false);//TODO make a better system for tile attributes
			(*p_mainTileBuffer)[i].push_back(currentTile);///LOAD tiles starting top/left and move right
			//can use v2d[i][j] syntax now for reading tiles
		}
	}
	//LOAD WARP DATA
	int numberOfWarps, tempValue;
	SDL_Rect tempLoc;
	inFile >> numberOfWarps;
	C_WarpZone tempWarp;
	for(int i = 0; i < numberOfWarps; i++){
		inFile >> tempLoc.x >> tempLoc.y >> tempLoc. w >> tempLoc.h;
		tempWarp.Set_position(tempLoc);//load warp zone position
		
		inFile >> tempLoc.x >> tempLoc.y >> tempLoc. w >> tempLoc.h;
		tempWarp.Set_endPosition(tempLoc);//load where warp takes player (x/y)

		inFile >> tempLoc.x >> tempLoc.y;
		tempWarp.Set_endMap(tempLoc);//load which map warp takes player to (XX-YY)

		inFile >> tempValue;
		tempWarp.Set_active(tempValue);

		inFile >> tempValue;
		tempWarp.Set_visible(tempValue);

		inFile >> tempValue;
		tempWarp.Set_warpTo(tempValue);

		v_WarpZones.push_back(tempWarp);//add to vector
		}


	inFile.close();

	//Setup secondary buffer
	for(int i = 0; i < m_mapWidth/TILE_SIZE; i++){
		(*p_secondTileBuffer).push_back( std::vector<C_Tiles>() ); //Push a C_Tiles vector
		for(int j = 0; j < m_mapHeight/TILE_SIZE; j++)
			(*p_secondTileBuffer)[i].push_back(currentTile);///LOAD tiles starting top/left and move right
	}

	//LOAD BACKGROUNDS ---- TODO: put this outside Tilemanager. Make function "bool timeToLoad()" that tells when everything should load
	BackgroundManager.Load();


	
	//CHANGE/PLAY AUDIO
	while(!AudioManager.FadeOutMusic(2000) && AudioManager.Get_PlayingStatus())//wait until fade out is complete
		SDL_Delay(100);

	switch(m_zoneType){
		case OVERWORLD: //if(AudioManager.Get_CurrentTrack() != 1)//if overworld tune isn't already playing
							AudioManager.PlayMusic(1);
						break;
		case DUNGEON:	//if(AudioManager.Get_CurrentTrack() != 2)
							AudioManager.PlayMusic(2);
						break;

		case CAVE:		//if(AudioManager.Get_CurrentTrack() != 0)
							AudioManager.PlayMusic(0);
						break;
	}
}

void C_TileManager::LoadBuffer()
{	
	//Clear Warp Data (because taking a warp causes a load using mainBuffer
	v_WarpZones.clear();

	//Load Requested Map by parsing m_mapPosition
	std::fstream inFile;
	char A[3];
	char B[3];
	itoa(m_mapPosition.x, A, 10);
	itoa(m_mapPosition.y, B, 10);
	//FORMAT the first map value
	if(m_mapPosition.x < 10){
		A[2] = A[1];
		A[1] = A[0];
		A[0] = '0';
	}

	std::string fileName;
	if(m_zoneType == OVERWORLD)
		fileName = "Map Data\\Over world\\";
	if(m_zoneType == DUNGEON)
		fileName = "Map Data\\Dungeons\\";
	fileName += A;
	fileName += "-0";
	fileName += B;
	fileName += ".txt";
	inFile.open(fileName); //open file to read   FORMAT IS (mapX - mapY . txt)
	
	

	C_Tiles currentTile;//Temp file to read data to
	currentTile.Set_tileWidth(TILE_SIZE);
	currentTile.Set_tileHeight(TILE_SIZE);

	//Variables to read from file
	int type, destruct, solid, friendlydmg, enemydmg;

	inFile >> m_mapWidth;
	inFile >> m_mapHeight;

	//push back tiles in row major order (left to right, top to bottom)
	for(int x = 0; x < m_mapWidth/TILE_SIZE; x++)
		for(int y = 0; y < m_mapHeight/TILE_SIZE; y++){
			
			inFile >> type >> destruct >> solid >> friendlydmg >> enemydmg;
			//TEMP TO TEST OUT COLLISION
			if(type != -1)
				solid = 1;
			else
				solid = 0;

			(*p_secondTileBuffer)[x][y].Set_tileX(x * TILE_SIZE);
			(*p_secondTileBuffer)[x][y].Set_tileY(y * TILE_SIZE);
			(*p_secondTileBuffer)[x][y].Set_tileType(type);
			(*p_secondTileBuffer)[x][y].Set_solid(solid);
			//Set destruct, etc
			if(type == 12 || type == 46)
				(*p_secondTileBuffer)[x][y].Set_solid(false);
		}

	//LOAD WARP DATA
	int numberOfWarps, tempValue;
	SDL_Rect tempLoc;
	inFile >> numberOfWarps;
	C_WarpZone tempWarp;
	for(int i = 0; i < numberOfWarps; i++){
		inFile >> tempLoc.x >> tempLoc.y >> tempLoc. w >> tempLoc.h;
		tempWarp.Set_position(tempLoc);//load warp zone position
		
		inFile >> tempLoc.x >> tempLoc.y >> tempLoc. w >> tempLoc.h;
		tempWarp.Set_endPosition(tempLoc);//load where warp takes player (x/y)

		inFile >> tempLoc.x >> tempLoc.y;
		tempWarp.Set_endMap(tempLoc);//load which map warp takes player to (XX-YY)

		inFile >> tempValue;
		tempWarp.Set_active(tempValue);

		inFile >> tempValue;
		tempWarp.Set_visible(tempValue);

		inFile >> tempValue;
		tempWarp.Set_warpTo(tempValue);

		v_WarpZones.push_back(tempWarp);//add to vector
	}
	inFile.close();

	//BACKGROUND OBJECTS
	BackgroundManager.Load();
}
void C_TileManager::SaveMap()
{
	//make sure we're saving from the correct buffer
	//open file
	std::ofstream outFile;
	char A[3];
	char B[3];
	itoa(m_mapPosition.x, A, 10);
	itoa(m_mapPosition.y, B, 10);
	//FORMAT the first map value
	if(m_mapPosition.x < 10){
		A[2] = A[1];
		A[1] = A[0];
		A[0] = '0';
	}
	std::string fileName;
	switch(m_zoneType){
	case OVERWORLD: fileName = "Map Data\\Over world\\";
					break;
	case DUNGEON:   fileName = "Map Data\\Dungeons\\";
					break;
	case CAVE:      fileName = "Map Data\\Caves\\";
					break;
	}
	fileName += A;
	fileName += "-0";
	fileName += B;
	fileName += ".txt";
	outFile.open(fileName); //open file for write only   FORMAT IS (mapX - mapY . txt)

	outFile << m_mapWidth << std::endl;
	outFile << m_mapHeight << std::endl;
	
	//read through every tile
	for(int x = 0; x < m_mapWidth/TILE_SIZE; x++)
		for(int y = 0; y < m_mapHeight/TILE_SIZE; y++){
	
			//for each tile save this stuff
	
			//tile height/width are static
			outFile << (*p_mainTileBuffer)[x][y].Get_tileType() << " ";//save tile type
			outFile << (*p_mainTileBuffer)[x][y].Get_destructable() << " ";//save destructible
			outFile << (*p_mainTileBuffer)[x][y].Get_solid() << " ";//save solid
			outFile << (*p_mainTileBuffer)[x][y].Get_friendlyDamage() << " ";//save friendly dmg
			outFile << 0 /*(*p_mainTileBuffer)[x][y].Get_enemyDamage() */ << std::endl;//save enemy dmg
	
		}//end of tile parse/forloop

	//END OF SAVING TILES

	//SAVING WARP DATA
		outFile << v_WarpZones.size() << std::endl;
		for(int i = 0; i < v_WarpZones.size(); i++){
			//for each object save all data
			outFile //save position of the warp zone
					<< v_WarpZones[i].Get_position().x << " "
					<< v_WarpZones[i].Get_position().y << " "
					<< v_WarpZones[i].Get_position().w << " "
					<< v_WarpZones[i].Get_position().h << " "
					//save x/y position object takes you to
					<< v_WarpZones[i].Get_endPosition().x << " "
					<< v_WarpZones[i].Get_endPosition().y << " "
					<< v_WarpZones[i].Get_endPosition().w << " "
					<< v_WarpZones[i].Get_endPosition().h << " "
					//save which map warp takes you to
					<< v_WarpZones[i].Get_endMap().x << " "
					<< v_WarpZones[i].Get_endMap().y << " "
					//misc stuff
					<< v_WarpZones[i].Get_active() << " " 
					<< v_WarpZones[i].Get_visible() << " "
					<< v_WarpZones[i].Get_warpType() << std::endl;
		}



	outFile.close();
}
void C_TileManager::LoadCave()
{
	//todo GET RID OF THIS FUNCTION LATER
	m_zoneType = CAVE;
	Camera.Set_cameraType(CAVE);//setup camera
	p_secondTileBuffer->clear();//empty buffer
	m_staticBackground = 2;//background is black
	m_transitionType = FADE_IN;//start transition
	std::fstream inFile;
	std::string fileName;

	//PARSE ZONE TO LOAD FROM WARP ZONE
	m_mapPosition.x = v_WarpZones[m_warpUsed].Get_endMap().x;
	m_mapPosition.y = v_WarpZones[m_warpUsed].Get_endMap().y;
	m_zoneType = v_WarpZones[m_warpUsed].Get_warpType();

	char A[3];
	char B[3];
	itoa(m_mapPosition.x, A, 10);
	itoa(m_mapPosition.y, B, 10);
	//FORMAT the first map value
	if(m_mapPosition.x < 10){
		A[2] = A[1];
		A[1] = A[0];
		A[0] = '0';
	}
	fileName = "Map Data\\Caves\\";
	
	fileName += A;
	fileName += "-0";
	fileName += B;
	fileName += ".txt";
	inFile.open(fileName); //open file to read   FORMAT IS (mapX - mapY . txt)
	
	v_WarpZones.clear();//vector not needed, clear it out
	

	C_Tiles currentTile;//Temp file to read data to
	currentTile.Set_tileWidth(TILE_SIZE);
	currentTile.Set_tileHeight(TILE_SIZE);

	//Variables to read from file
	int type, destruct, solid, friendlydmg, enemydmg;

	inFile >> m_mapWidth;
	inFile >> m_mapHeight;

	//push back tiles in row major order (left to right, top to bottom)
	for(int i = 0; i < m_mapWidth/TILE_SIZE; i++){
		(*p_secondTileBuffer).push_back( std::vector<C_Tiles>() ); //Push a C_Tiles vector
		for(int j = 0; j < m_mapHeight/TILE_SIZE; j++){
			currentTile.Set_tileX(i * TILE_SIZE);
			currentTile.Set_tileY(j * TILE_SIZE);
			inFile >> type;
			inFile >> destruct;
			inFile >> solid;
			inFile >> friendlydmg;
			inFile >> enemydmg;

			//TEMP TO TEST OUT COLLISION
			if(type != -1)
				solid = 1;
			else
				solid = 0;

			currentTile.Set_tileType(type);
			currentTile.Set_destructable(destruct);
			currentTile.Set_solid(solid);
			currentTile.Set_friendlyDamage(friendlydmg);
			currentTile.Set_enemyDamage(enemydmg);

			(*p_secondTileBuffer)[i].push_back(currentTile);///LOAD tiles starting top/left and move right
			//can use v2d[i][j] syntax now for reading tiles
		}
	}
	//END OF LOADING TILES


	//LOAD WARP DATA
	int numberOfWarps, tempValue;
	SDL_Rect tempLoc;
	inFile >> numberOfWarps;
	C_WarpZone tempWarp;
	for(int i = 0; i < numberOfWarps; i++){
		inFile >> tempLoc.x >> tempLoc.y >> tempLoc. w >> tempLoc.h;
		tempWarp.Set_position(tempLoc);//load warp zone position
		
		inFile >> tempLoc.x >> tempLoc.y >> tempLoc. w >> tempLoc.h;
		tempWarp.Set_endPosition(tempLoc);//load where warp takes player (x/y)

		inFile >> tempLoc.x >> tempLoc.y;
		tempWarp.Set_endMap(tempLoc);//load which map warp takes player to (XX-YY)

		inFile >> tempValue;
		tempWarp.Set_active(tempValue);

		inFile >> tempValue;
		tempWarp.Set_visible(tempValue);

		inFile >> tempValue;
		tempWarp.Set_warpTo(tempValue);

		v_WarpZones.push_back(tempWarp);//add to vector
		}
	//END OF LOADING WARP DATA

	//BACKGROUND OBJECTS
	BackgroundManager.Load();

	inFile.close();//close file
	SwapBuffers();//swap to new buffer
	SwapOffsets();

}

void C_TileManager::Render()
{
	//TEMP
	int DrawBufferX = 1;
	int DrawBufferY = 1;
	SDL_Rect tileClip={0,0, TILE_SIZE, TILE_SIZE};

	
	p_mainOffset->x = 0;
	p_mainOffset->y = 0;

	//END TEMP
	for(int y = Camera.Get_XYWH()->y/TILE_SIZE; y < (Camera.Get_XYWH()->y + SCREEN_HEIGHT)/TILE_SIZE + DrawBufferY; y++)
		for(int x = Camera.Get_XYWH()->x/TILE_SIZE; x < (Camera.Get_XYWH()->x + SCREEN_WIDTH)/TILE_SIZE + DrawBufferX; x++){
				if( x == m_mapWidth/16 ){//Keeps from over drawing IE: vector bound check
					x = 79;//tiles in an overworld range from 0 to 79.  Overflow check
					DrawBufferX = 0;
				}
				if( y == m_mapHeight/16 ){//bounds check **if drawing 2nd half of screen dont add buffer
					y = 59;//tiles in an overworld range from 0 to 79.  Overflow check
					DrawBufferY = 0;
				}
				if(Collision.Check(Camera.Get_XYWH(), (*p_mainTileBuffer)[x][y].Get_tileSDL_Rect()) && *(*p_mainTileBuffer)[x][y].Get_tileType() != -1 )//{//if tiles are on screen  DO NOT DRAW -1				
					p_mainOffset->x = *(*p_mainTileBuffer)[x][y].Get_tileX() - Camera.Get_XYWH()->x + offsetCounterX;
					p_mainOffset->y = *(*p_mainTileBuffer)[x][y].Get_tileY() - Camera.Get_XYWH()->y + offsetCounterY;
				
					//ADD DEBUG TO SEE INVISIBLE TILES
					tileClip.x = 16 *  (*(*p_mainTileBuffer)[x][y].Get_tileType()) - (*((*p_mainTileBuffer)[x][y].Get_tileType())/12 * 16 * 12);
					tileClip.y = 16 *  ((*(*p_mainTileBuffer)[x][y].Get_tileType())/12); //12 tiles per row in the sprite sheet.
				
					//AND finally draw the tiles
					if(m_mapTransition == true)
						MapTransition(-99);
					
					SDL_BlitSurface(LevelAssets.getImage(0), &tileClip, screen, p_mainOffset);
			

		}
	//buffer starts here when transitioning UP---------------------------------------
	p_secondOffset->x = 0;
	p_secondOffset->y = 0;
	/********* TRANSITION STUFF *****************/
	int transitionX, transitionY, transitionMaxY, transitionMaxX;
	int transitionOffsetX = 0;
	int transitionOffsetY = 0;
	if(transitionDirection == UP){
		transitionX = Camera.Get_XYWH()->x/TILE_SIZE;
		transitionY = 30;//40 tiles down
		transitionMaxX = transitionX + 40;
		transitionMaxY = transitionY + 30;
		transitionOffsetX = -Camera.Get_XYWH()->x;
		transitionOffsetY = -960;
	}
	if(transitionDirection == DOWN){
		transitionX = Camera.Get_XYWH()->x/TILE_SIZE;
		transitionY = 0;//40 tiles down
		transitionMaxX = transitionX + 40;
		transitionMaxY = transitionY + 30;
		transitionOffsetX = -Camera.Get_XYWH()->x;
		transitionOffsetY = 480;
	}
	if(transitionDirection == LEFT){
		transitionX = 40;//start at middle of map
		transitionY =  Camera.Get_XYWH()->y/TILE_SIZE;//start where camera is
		transitionMaxX = transitionX+40;
		transitionMaxY = transitionY + 30;
		transitionOffsetX = -1280;//move map left 2 screens before showing it
		transitionOffsetY = -Camera.Get_XYWH()->y;
	}
	if(transitionDirection == RIGHT){
		transitionX = 0;//start at middle of map
		transitionY =  Camera.Get_XYWH()->y/TILE_SIZE;//start where camera is
		transitionMaxX = transitionX+40;
		transitionMaxY = transitionY + 30;
		transitionOffsetX = 640;//move map right 1 screen before showing it
		transitionOffsetY = -Camera.Get_XYWH()->y;
	}

	//WHEN TRANSITION STARTS RENDER HERE
		if(m_mapTransition == true)
			for(int y = transitionY  /*Camera.Get_XYWH().y)/TILE_SIZE*/; y < transitionMaxY /*(Camera.Get_XYWH().y + SCREEN_HEIGHT)/TILE_SIZE*/ + DrawBufferY; y++)
				for(int x = transitionX; x < transitionMaxX + DrawBufferX; x++)	{
						if( x == LEVEL_WIDTH/16 -1 )//Keeps from over drawing IE: vector bound check
							DrawBufferX = 0;
						if( y == LEVEL_HEIGHT/16 -1 )//bounds check
							DrawBufferY = 0;			
					
						//Update Offsets	
						p_secondOffset->x = *(*p_secondTileBuffer)[x][y].Get_tileX() + offsetCounterX + transitionOffsetX;// 
						p_secondOffset->y = *(*p_secondTileBuffer)[x][y].Get_tileY() + offsetCounterY + transitionOffsetY;
				
						//ADD DEBUG TO SEE INVISIBLE TILES
						tileClip.x = 16 * (*(*p_secondTileBuffer)[x][y].Get_tileType()) - ((*(*p_secondTileBuffer)[x][y].Get_tileType())/12 * 16 * 12);
						tileClip.y = 16 * ((*(*p_secondTileBuffer)[x][y].Get_tileType())/12); //12 tiles per row in the sprite sheet.
	
						//AND finally draw the tiles
						if(m_mapTransition == true)
							MapTransition(-99);
						SDL_BlitSurface(LevelAssets.getImage(0), &tileClip, screen, p_secondOffset);
			
				}
}
void C_TileManager::RenderStaticBackground()
{
	SDL_Rect tempOffset = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_BlitSurface(StaticBGs.getImage(m_staticBackground), &tempOffset, screen, &tempOffset);
}
//SET FUNCTIONS
void C_TileManager::Set_tileType(int x, int y, int newTileType)
{
	(*p_mainTileBuffer)[x][y].Set_tileType(newTileType);
}
void C_TileManager::SetPlane(int x, int y, int newPlaneValue)
{
	(*p_mainTileBuffer)[x][y].Set_tilePlane(newPlaneValue);
}
void C_TileManager::Set_mapPosition(SDL_Rect newPosition)
{
	m_mapPosition.x = newPosition.x;
	m_mapPosition.y = newPosition.y;
	
	LoadBuffer();//load new map 
}
void C_TileManager::Set_tileSolid(int x, int y, bool newValue)
{
	(*p_mainTileBuffer)[x][y].Set_solid(newValue);
}
//GET FUNCTIONS
SDL_Rect* C_TileManager::Get_tileOffset(int x, int y)
{
	return (*p_mainTileBuffer)[x][y].Get_tileSDL_Rect();
}
bool C_TileManager::Get_tileSolid(int x, int y)
{
	return (*p_mainTileBuffer)[x][y].Get_solid();
}
int C_TileManager::GetPlane()
{
	return 1;//NOT DONE TODO
}
int* C_TileManager::Get_tileType(int newX, int newY)
{
	return (*p_mainTileBuffer)[newX][newY].Get_tileType();
}
SDL_Rect C_TileManager::Get_tileBufferOffset()//enumerate somethiing
{
		return *p_secondOffset;
}
int C_TileManager::Get_offsetCounterX()
{
	return int(offsetCounterX);
}
int C_TileManager::Get_offsetCounterY()
{
	return int(offsetCounterY);
}
SDL_Rect C_TileManager::Get_mapPosition()
{
	//TODO: error checking needed??
	SDL_Rect mapPos = {m_mapPosition.x, m_mapPosition.y};
	return mapPos;
}
int C_TileManager::Get_mapType()
{
	return m_zoneType;
}
//MISC FUNCTIONS-----------------
//Map transition
void C_TileManager::MapTransition(int direction)
{
	m_mapTransition = true;
	if(direction != -1 && direction != -99)//if(m_mapTransition == false)		
		transitionDirection = direction;
		
	if(direction == -99)
		direction = transitionDirection;

	switch(direction){
		case UP:if(offsetCounterY < 480){
					offsetCounterY += .0025;//.0025;//increase offset
					Camera.Set_cameraLock(false);
				}
				else{
					m_mapTransition = false;
					offsetCounterY = 0;
					SwapBuffers();
					SwapOffsets();
					Camera.Set_cameraLock(true);
					transitionDirection = -1;					
				}
				break;

		case DOWN:if(offsetCounterY > -480){
					offsetCounterY -= .0025;//increase offset
  					Camera.Set_cameraLock(false);
					}
				else{
					m_mapTransition = false;
					offsetCounterY = 0;
					SwapBuffers();
					SwapOffsets();
					Camera.Set_cameraLock(true);
					transitionDirection = -1;
				}
				break;

		case LEFT:if(offsetCounterX < 640){
					offsetCounterX += .0025;//increase offset
					Camera.Set_cameraLock(false);
					}
				else{
					m_mapTransition = false;
					offsetCounterX = 0;
					SwapBuffers();
					SwapOffsets();
					Camera.Set_cameraLock(true);
					transitionDirection = -1;
				}
				break;

		case RIGHT:if(offsetCounterX > -640){
					offsetCounterX -= .0025;//increase offset
					Camera.Set_cameraLock(false);
					}
				else{
					m_mapTransition = false;
					offsetCounterX = 0;
					SwapBuffers();
					SwapOffsets();
					Camera.Set_cameraLock(true);
					transitionDirection = -1;
				}
				break;

	}//end switch

	
}
int C_TileManager::Get_transitionDirection()
{
	int theDirection = transitionDirection;
	if(!m_mapTransition)
		theDirection = -1;

	return theDirection;
}
void C_TileManager::SwapBuffers()
{
	//Just swaps pointers to the buffers
	if(p_mainTileBuffer == &v2d_tileList){//swap to second buffer
		p_mainTileBuffer = &v2d_tileListBuffer;
		p_secondTileBuffer = &v2d_tileList;
	}

	else{
		p_mainTileBuffer = &v2d_tileList;//swap back to first buffer
		p_secondTileBuffer = &v2d_tileListBuffer;
	}
} //DONE
void C_TileManager::SwapOffsets()
{
	//Just swaps pointers to the buffers
	if(p_mainOffset == &m_mainOffset){//swap to second buffer
		p_mainOffset = &m_bufferOffset;
		p_secondOffset = &m_mainOffset;
	}

	else{
		p_mainOffset = &m_mainOffset;//swap back to first buffer
		p_secondOffset = &m_bufferOffset;
	}
} //DONE

//TRANSITIONS
void C_TileManager::Transition_Start(int startThis)
{
	m_transitionType = startThis;
}
void C_TileManager::Transitions_Render()
{
	switch(m_transitionType){
	case FADE_IN:	Transition_FadeIn();
					break;
	case FADE_OUT:	Transition_FadeOut();
					break;
	default:		m_transitionType = -1;//no transition if value is invalid
	}
}

void C_TileManager::Transition_FadeIn()
{
	m_transitionCounter-= 4;
	if(m_transitionCounter < 1){
		m_transitionCounter = 1;
		m_transitionType = -1;
	}

	SDL_Rect offset = {0, 0, 640, 480};
	Uint32 color = SDL_MapRGB(screen->format,  0, 0, 0);
	boxRGBA(screen, 0, 0, 640, 480, 0, 0, 0, m_transitionCounter/2);
	
}

void C_TileManager::Transition_FadeOut()
{
	m_transitionCounter+= 4;
	if(m_transitionCounter > 500){
		m_transitionCounter = 500;
		m_transitionType = -1;
	}

	SDL_Rect offset = {0, 0, 640, 480};
	Uint32 color = SDL_MapRGB(screen->format,  0, 0, 0);
	boxRGBA(screen, 0, 0, 640, 480, 0, 0, 0, m_transitionCounter/2);
	
}
bool C_TileManager::Transition_GetStatus()
{
	bool t_status = true;
	if(m_transitionType == -1)
		t_status = false;

	return t_status;
}
void C_TileManager::Set_warpUsed(int newWarpUsed)
{
	m_warpUsed = newWarpUsed;
}
//OBJECTS
void C_TileManager::Create_Warp()
{
	C_WarpZone newWarp;
	v_WarpZones.push_back(newWarp);
}
void C_TileManager::Delete_Warp(int object_to_delete)
{
	v_WarpZones.erase(v_WarpZones.begin() + object_to_delete);
}
void C_TileManager::Render_Warp()
{
	if(v_WarpZones.size() > 0)
		for(int i = 0; i < v_WarpZones.size(); i++)
			if(v_WarpZones[i].Get_visible())
				v_WarpZones[i].Render();
}
SDL_Rect C_TileManager::Get_WarpDestination(int element)
{
	if(v_WarpZones.size() > element)
		return	v_WarpZones[element].Get_endPosition();
}
SDL_Rect C_TileManager::Get_WarpPosition(int element)
{
	if(v_WarpZones.size() > element)
		return	v_WarpZones[element].Get_position();
}
SDL_Rect C_TileManager::Get_WarpEndMap(int element)
{
	if(v_WarpZones.size() > element)
		return	v_WarpZones[element].Get_endMap();
}
SDL_Rect C_TileManager::Get_WarpStartMap(int element)
{
	if(v_WarpZones.size() > element)
		return	v_WarpZones[element].Get_startMap();
}
void C_TileManager::Set_WarpPosition(int element, SDL_Rect newPosition)
{
	if(v_WarpZones.size() > element)
		v_WarpZones[element].Set_position(newPosition);
}
void C_TileManager::Set_WarpEndMap(int element, SDL_Rect newEnd)
{
	if(v_WarpZones.size() > element)
		v_WarpZones[element].Set_endMap(newEnd);
}
void C_TileManager::Set_WarpStartMap(int element, SDL_Rect newStart)
{
	if(v_WarpZones.size() > element)
		v_WarpZones[element].Set_startMap(newStart);
}
void C_TileManager::Set_WarpEndPosition(int element, SDL_Rect newPosition)
{
	if(v_WarpZones.size() > element)
		v_WarpZones[element].Set_endPosition(newPosition);
}
void C_TileManager::Set_WarpType(int element, int warpType)
{
	if(v_WarpZones.size() > element)
		v_WarpZones[element].Set_warpTo(warpType);
}
int C_TileManager::Get_WarpType(int element)
{
	if(v_WarpZones.size() > element)
		return	v_WarpZones[element].Get_warpType();
}
int C_TileManager::Get_WarpCount()
{
	return v_WarpZones.size();
}

void C_TileManager::ToggleWarpHighlight(int element, bool highlight)
{
	v_WarpZones[element].ToggleHighlight(highlight);
}
//RESET
void C_TileManager::StartNewGame()
{
	m_newGame = true;
}
#endif