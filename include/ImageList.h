// THIS WILL USE A STD::LIST TO HOLD THE LOADED IMAGES
#ifndef IMAGELIST_H
#define IMAGELIST_H
#include "LoadImage.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using std::ofstream;
using std::cin;
using std::string;

class C_ImageList
{

      private:
              C_LoadImage tempImage; 
              ofstream outFile;
			  std::vector <C_LoadImage> theImageList;
              
    
      public:      
             C_ImageList();
             ~C_ImageList();
             
             void LoadImage(string);//Add Image to the end of the list
             SDL_Surface* getImage(int);// Returns a pointer to the image. Argument determines which image. TODO: optimize this shit
             

             void cleanup();
             void print();
};

#endif
