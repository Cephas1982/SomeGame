//This class will hold the all the loaded surfaces
#ifndef IMAGELIST_CPP
#define IMAGELIST_CPP
#include "ImageList.h"
C_ImageList::C_ImageList()
{
}

C_ImageList::~C_ImageList()
{
    //print();
	cleanup();
}

void C_ImageList::LoadImage(string filename)
{
    if(tempImage.load_files(filename))
        theImageList.push_back(tempImage);   

}

//takes an int, and returns the address of the image. Images are loaded in order
SDL_Surface* C_ImageList::getImage(int imageWanted)
{	
		
	if(imageWanted > int(theImageList.size()) - 1)//don't let array go out of bounds ---TODO: Make the first tile a visual that things are wrong
		imageWanted = 0;
	
	return theImageList[imageWanted].getImage();
}

void C_ImageList::print()
{
   outFile.open("Debug\\Image_List_ERROR.txt");
    for(int i = 0; i < int(theImageList.size()); i++){
        outFile << "#" << i+1 <<":  " << theImageList[i].getFileName() << std::endl;
    }
	outFile.close();
       
}
//Traverses through loaded surfaces and frees them
void C_ImageList::cleanup()
{
    for(int i = 0; i < int(theImageList.size()); i++){
        SDL_FreeSurface( theImageList[i].getImage() );//free surface  
	}
}
#endif

