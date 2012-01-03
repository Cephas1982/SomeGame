#ifndef CAMERA_H
#define CAMERA_H
#include "SDL.h"
#include "Config.h"
class C_Camera
{
      private:
              SDL_Rect theCamera;
			  int* m_cameraX;
			  int* m_cameraY;

			  bool m_cameraLock;//stops camera lock on when false/transitioning
              
      public:
             C_Camera();
             ~C_Camera();      

             
             void UpdateCamera(); //Sets camera using SDL_Rect pointer		
             void SetCamera(int*, int*); //Used to update what the camera is tracking
			
			 //SET
			 void Set_cameraLock(bool);
			 void Set_cameraType(int); //For now there's indoor (camera off basically), and outdoor
			
			 //GET
			 SDL_Rect Get_XYWH(); //returns SDL_Rect of camera coordinates
			 int* Get_X();//return pointer
			 int* Get_Y();
}; 

#endif
