//LoadImage
#ifndef LOADIMAGE_H
#define LOADIMAGE_H
#include "SDL.h"
#include "SDL_image.h"
#include <string>
using std::string;
class C_LoadImage
{
      protected:
              SDL_Surface *image;//hold image here for now
              string filename; // holds file name
                           
      public:
             C_LoadImage();
             ~C_LoadImage();
             
             bool load_files(string);//loads a image
             
             SDL_Surface* Load(std::string);
             SDL_Surface* getImage();// returns image  
             
             string getFileName();// returns file name
};
#endif
