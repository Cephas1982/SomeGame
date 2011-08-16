#ifndef CAMERA_CPP
#define CAMERA_CPP
#include "Camera.h"
C_Camera::C_Camera()
{
    theCamera.x = 0;
    theCamera.y = 0;
    theCamera.w =  SCREEN_WIDTH;
    theCamera.h =  SCREEN_HEIGHT;

	m_cameraX = NULL;
	m_cameraY = NULL;

	m_cameraLock = true;
}

C_Camera::~C_Camera()
{}


void C_Camera::UpdateCamera()
{
      //Center the camera
	if(m_cameraLock == true){
	    theCamera.x = ( *m_cameraX + PLAYER_WIDTH / 2 ) - SCREEN_WIDTH / 2; 
        theCamera.y = ( *m_cameraY + PLAYER_HEIGHT / 2 ) - SCREEN_HEIGHT /2; 
	}

    //Keep the camera in bounds. 
    if( theCamera.x < 0 ) 
    { 
        theCamera.x = 0; 
    } 
    if( theCamera.y < 0 ) 
    { 
        theCamera.y = 0; 
    } 
    if( theCamera.x > LEVEL_WIDTH - theCamera.w )
    { 
        theCamera.x = LEVEL_WIDTH - theCamera.w; 
    } 
    if( theCamera.y > LEVEL_HEIGHT - theCamera.h ) 
    { 
        theCamera.y = LEVEL_HEIGHT - theCamera.h; 
    }
} 

void C_Camera::SetCamera(int* newX, int* newY)
{
    m_cameraX = newX;
	m_cameraY = newY;

}

//SET FUNCTIONS
void C_Camera::Set_cameraLock(bool is_it_locked)
{
	m_cameraLock = is_it_locked;
}
void C_Camera::Set_cameraType(int cameraType)
{
	switch(cameraType){
	case OVERWORLD: theCamera.w = SCREEN_WIDTH;
					theCamera.h = SCREEN_HEIGHT;
					break;
	case DUNGEON:	theCamera.w = LEVEL_WIDTH;
					theCamera.h = LEVEL_HEIGHT;
					break;
	case CAVE:		theCamera.w = LEVEL_WIDTH;
					theCamera.h = LEVEL_HEIGHT;
					break;
	}
}

//GET FUNCTIONS
SDL_Rect C_Camera::Get_XYWH()
{
    return theCamera;         
}
int* C_Camera::Get_X()
{
	return m_cameraX;
}
int* C_Camera::Get_Y()
{
	return m_cameraY;
}
#endif
