#ifndef TTF_H
#define TTF_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <string.h>
using std::stringstream;
class C_Font
{
      protected:
              SDL_Surface *message;
			  TTF_Font *font, *arial12, *arial14, *arial16, *arial18, *arial20, *arial22;//game font
              SDL_Color textColor;
              SDL_Rect txtPos;
              stringstream text;
                       
      public://TODO wtf why can't i pass/copy sstream pointer
             C_Font();
             ~C_Font();
             void Print(std::stringstream *, SDL_Surface *, int = 12, int = 0, int = 440, int = 255, int = 255, int = 250 ); //output, screen, x and y coordinate, R, G, B
};
#endif
