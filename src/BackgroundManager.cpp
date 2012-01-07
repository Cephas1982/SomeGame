#ifndef BACKGROUNDMANAGER_CPP
#define BACKGROUNDMANAGER_CPP
#include "BackgroundManager.h"
#include "TileManager.h"
#include "Camera.h"
extern SDL_Surface* screen;
extern C_ImageList BackgroundImg;
extern C_Camera Camera;
extern C_TileManager TileManager;

C_BackgroundManager::C_BackgroundManager()
{
	m_red = 0;
	m_green = 200;
	m_blue = 200;
	m_colorIncrease = true;
	LoadImageData();
}

C_BackgroundManager::~C_BackgroundManager()
{
}
void C_BackgroundManager::LoadImageData()
{
	std::fstream inFile;
	inFile.open("BG data\\BG data.txt");

	
	SDL_Rect tempXYWH;
	std::string tempString;
	C_BG_Objects tempObject;
	while(!inFile.eof()){//while not at end of file
		inFile >> tempXYWH.x >> tempXYWH.y >> tempXYWH.w >> tempXYWH.h;
		getline(inFile, tempString );
		tempString.erase(0,3);//remove the //

		tempObject.Set_XYWH(tempXYWH);
		tempObject.Set_name(tempString);

		v_bgObjectList.push_back(tempObject);
	}
	inFile.close();
}

void C_BackgroundManager::Create(int type, int layer, SDL_Rect worldOffset)
{
	//get real position by adding camera value back.... worldoffset is mouse+camera and only mouse x/y is needed
	SDL_Rect truePosition ={ worldOffset.x + Camera.Get_XYWH()->x, worldOffset.y + Camera.Get_XYWH()->y};
	
	C_BG_Objects tempObj;
	tempObj.Set_type(type);							   //type of object
	tempObj.Set_layer(layer);						   //set layer
	tempObj.Set_XYWH(v_bgObjectList[type].Get_XYWH()); //set sprite sheet coordinates
	tempObj.Set_position(truePosition);				   //set world coodrinates
	tempObj.Set_solid(v_bgObjectList[type].Get_solid());//set whether solid or not
	//TODO: Set more stuff later

	v_bgObjectsActive.push_back(tempObj);
}
void C_BackgroundManager::Remove(int eraseThis)
{
	if(eraseThis < v_bgObjectsActive.size())
		v_bgObjectsActive.erase(v_bgObjectsActive.begin() + eraseThis);
}
void C_BackgroundManager::RenderList(int element, SDL_Rect offset)
{
	SDL_Rect clip	= {v_bgObjectList[element].Get_XYWH().x, v_bgObjectList[element].Get_XYWH().y, v_bgObjectList[element].Get_XYWH().w, v_bgObjectList[element].Get_XYWH().h};
	//Offset is mouse coordinates
	SDL_BlitSurface(BackgroundImg.getImage(0), &clip, screen, &offset);
}

//These render functions also handle highlights 
void C_BackgroundManager::RenderLayer1()
{
	for(int i = 0; i < v_bgObjectsActive.size(); i++){
		if(v_bgObjectsActive[i].Get_layer() == 1){//don't do anything if object is not on layer 1
			//correct for camera
			SDL_Rect truePosition ={ v_bgObjectsActive[i].Get_Position().x - Camera.Get_XYWH()->x, v_bgObjectsActive[i].Get_Position().y - Camera.Get_XYWH()->y};
			SDL_BlitSurface(BackgroundImg.getImage(0), &v_bgObjectsActive[i].Get_XYWH(), screen, &truePosition);
		
			//IF the object is currently selected w/map editor
			if(v_bgObjectsActive[i].Get_highlight()){
					boxRGBA(screen, truePosition.x, truePosition.y, truePosition.x + v_bgObjectsActive[i].Get_XYWH().w, truePosition.y + v_bgObjectsActive[i].Get_XYWH().h, m_red, m_green, m_blue, 150);
				if(m_blue < 255 && m_colorIncrease){
					m_blue += 3;
					m_green +=3;
				}
				else{
					if(m_colorIncrease){
						m_blue = 255;
						m_green = 255;
						m_colorIncrease = false;
					}
				}

				if(m_blue > 0 && !m_colorIncrease){
					m_blue -= 3;
					m_green -=3;
				}
				else{
					if(!m_colorIncrease){
						m_blue = 0;
						m_green = 0;
						m_colorIncrease = true;
					}
				}
			}//end HIGHTLIGHTED
		}//JUMP HERE IF NOT THE CORRENT LAYER
	}//End forlooop
}
void C_BackgroundManager::RenderLayer2()
{
	for(int i = 0; i < v_bgObjectsActive.size(); i++){
		if(v_bgObjectsActive[i].Get_layer() == 2){//don't do anything if object is not on layer 3
			//correct for camera
			SDL_Rect truePosition ={ v_bgObjectsActive[i].Get_Position().x - Camera.Get_XYWH()->x, v_bgObjectsActive[i].Get_Position().y - Camera.Get_XYWH()->y};
			SDL_BlitSurface(BackgroundImg.getImage(0), &v_bgObjectsActive[i].Get_XYWH(), screen, &truePosition);
		
			//IF the object is currently selected w/map editor
			if(v_bgObjectsActive[i].Get_highlight()){
					boxRGBA(screen, truePosition.x, truePosition.y, truePosition.x + v_bgObjectsActive[i].Get_XYWH().w, truePosition.y + v_bgObjectsActive[i].Get_XYWH().h, m_red, m_green, m_blue, 150);
				if(m_blue < 255 && m_colorIncrease){
					m_blue += 3;
					m_green +=3;
				}
				else{
					if(m_colorIncrease){
						m_blue = 255;
						m_green = 255;
						m_colorIncrease = false;
					}
				}

				if(m_blue > 0 && !m_colorIncrease){
					m_blue -= 3;
					m_green -=3;
				}
				else{
					if(!m_colorIncrease){
						m_blue = 0;
						m_green = 0;
						m_colorIncrease = true;
					}
				}
			}//end HIGHTLIGHTED
		}//JUMP HERE IF NOT THE CORRENT LAYER
	}//End forlooop
}
void C_BackgroundManager::RenderLayer3()
{
	for(int i = 0; i < v_bgObjectsActive.size(); i++){
		if(v_bgObjectsActive[i].Get_layer() == 3){//don't do anything if object is not on layer 3
			//correct for camera
			SDL_Rect truePosition ={ v_bgObjectsActive[i].Get_Position().x - Camera.Get_XYWH()->x, v_bgObjectsActive[i].Get_Position().y - Camera.Get_XYWH()->y};
			SDL_BlitSurface(BackgroundImg.getImage(0), &v_bgObjectsActive[i].Get_XYWH(), screen, &truePosition);
		
			//IF the object is currently selected w/map editor
			if(v_bgObjectsActive[i].Get_highlight()){
					boxRGBA(screen, truePosition.x, truePosition.y, truePosition.x + v_bgObjectsActive[i].Get_XYWH().w, truePosition.y + v_bgObjectsActive[i].Get_XYWH().h, m_red, m_green, m_blue, 150);
				if(m_blue < 255 && m_colorIncrease){
					m_blue += 3;
					m_green +=3;
				}
				else{
					if(m_colorIncrease){
						m_blue = 255;
						m_green = 255;
						m_colorIncrease = false;
					}
				}

				if(m_blue > 0 && !m_colorIncrease){
					m_blue -= 3;
					m_green -=3;
				}
				else{
					if(!m_colorIncrease){
						m_blue = 0;
						m_green = 0;
						m_colorIncrease = true;
					}
				}
			}//end HIGHTLIGHTED
		}//JUMP HERE IF NOT THE CORRENT LAYER
	}//End forlooop
}
//SET FUNCTIONS
void C_BackgroundManager::Set_layer(int element, int newLayer)
{
	if( v_bgObjectsActive.size() > element)
		v_bgObjectsActive[element].Set_layer(newLayer);
}
//GET FUNCTIONS
int C_BackgroundManager::Get_layer(int element)
{
	int layer = 1;
	if(v_bgObjectsActive.size() > element)
		layer = v_bgObjectsActive[element].Get_layer();

	return layer;

}
SDL_Rect C_BackgroundManager::Get_position(int element)
{
	return v_bgObjectsActive[element].Get_Position();
}
//MISC
int C_BackgroundManager::ActiveSize()
{
	return v_bgObjectsActive.size();
}
int C_BackgroundManager::ListSize()
{
	return v_bgObjectList.size();
}
void C_BackgroundManager::Set_highlight(int element, bool onOff)
{
	if(v_bgObjectsActive.size() > element)
		v_bgObjectsActive[element].Set_highlight(onOff);
}
//SAVE & LOAD
void C_BackgroundManager::Save()
{
	std::ofstream outFile;

	char A[3];
	char B[3];
	itoa(TileManager.Get_mapPosition().x, A, 10);
	itoa(TileManager.Get_mapPosition().y, B, 10);
	//FORMAT the first map value
	if(TileManager.Get_mapPosition().x < 10){
		A[2] = A[1];
		A[1] = A[0];
		A[0] = '0';
	}
	std::string fileName;
	switch(TileManager.Get_mapType()){
	case OVERWORLD: fileName = "BG Data\\Over world\\";
					break;
	case DUNGEON:   fileName = "BG Data\\Dungeons\\";
					break;
	case CAVE:      fileName = "BG Data\\Caves\\";
					break;
	}
	fileName += A;
	fileName += "-0";
	fileName += B;
	fileName += ".txt";
	outFile.open(fileName); //open file for write only   FORMAT IS (mapX - mapY . txt)

	//SAVE ACTIVE BG INFO
	for(int i = 0; i < v_bgObjectsActive.size(); i++){
		//Save which type of obj (sprite sheet clip)  X, Y, W, H coordinates
		outFile << v_bgObjectsActive[i].Get_XYWH().x << " ";
		outFile << v_bgObjectsActive[i].Get_XYWH().y << " ";
		outFile << v_bgObjectsActive[i].Get_XYWH().w << " ";
		outFile << v_bgObjectsActive[i].Get_XYWH().h << " ";

		//Save world coordinates X, Y
		outFile << v_bgObjectsActive[i].Get_Position().x << " ";
		outFile << v_bgObjectsActive[i].Get_Position().y << " ";

		//Save layer (LAST STEP TO SAVE)
		if(i == v_bgObjectsActive.size() -1)//if on the very last object
			outFile << v_bgObjectsActive[i].Get_layer();//do not use endl;
		else
			outFile << v_bgObjectsActive[i].Get_layer() << std::endl;
	}

		outFile.close();
}
void C_BackgroundManager::Load()
{
	v_bgObjectsActive.clear();//clear out objects before loading new ones

	std::fstream inFile;
	std::string fileName;
	char A[3];
	char B[3];
	itoa(TileManager.Get_mapPosition().x, A, 10);
	itoa(TileManager.Get_mapPosition().y, B, 10);
	//FORMAT the first map value
	if(TileManager.Get_mapPosition().x < 10){
		A[2] = A[1];
		A[1] = A[0];
		A[0] = '0';
	}
	switch(TileManager.Get_mapType()){
	case OVERWORLD: fileName = "BG Data\\Over world\\";
					break;
	case DUNGEON:   fileName = "BG Data\\Dungeons\\";
					break;
	case CAVE:      fileName = "BG Data\\Caves\\";
					break;
	}
	fileName += A;
	fileName += "-0";
	fileName += B;
	fileName += ".txt";
	inFile.open(fileName); //open file to read   FORMAT IS (mapX - mapY . txt)

	if(inFile){//if it actually opened
		//LOAD BG INFO
		SDL_Rect temploc;
		C_BG_Objects tempObj;
		int tempLayer = 1;
		//make sure empty files are not loaded. TODO: make better
		long begin, end;
		begin = inFile.tellg();
		inFile.seekg(0, std::ios::end);//seek to end of file
		end = inFile.tellg();//save file size here
		inFile.seekg(0, std::ios::beg);//reset getstream pointer


		while(end != 0 && !inFile.eof()){
			//Load X,Y,W,H coordinates (sprite size)
			inFile >> temploc.x >> temploc.y >> temploc.w >> temploc.h;
			tempObj.Set_XYWH(temploc);//set which object to use from sprite sheet

			//Load world coordinates for current map
			inFile >> temploc.x >> temploc.y;
			tempObj.Set_position(temploc);

			//Load layer
			inFile >> tempLayer;
			tempObj.Set_layer(tempLayer);


			//Push object onto list
			v_bgObjectsActive.push_back(tempObj);//load up object list
		}

		inFile.close();
	}
}
#endif