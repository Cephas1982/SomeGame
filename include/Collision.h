#ifndef COLLISION_H
#define COLLISION_H
#include "SDL.h"
#include "Config.h"
class C_Collision
{
    private:
            
            
    public:
           C_Collision();
           ~C_Collision();
           bool Check(SDL_Rect* , SDL_Rect* );//Returns true if there's no collisions. Currently gets x,y from character(A)  
         
};
#endif
