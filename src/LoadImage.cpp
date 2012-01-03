#ifndef LOADIMAGE_CPP
#define LOADIMAGE_CPP
#include "LoadImage.h"

C_LoadImage::C_LoadImage()
{                                                       
    image = NULL;                      
}

C_LoadImage::~C_LoadImage()
{
}


bool C_LoadImage::load_files(string file)
{
    //Store filename for debug
    filename = file;       
    //Load the sprite sheet
    image = Load( filename );

    //If there was a problem in loading the sprite
    if( image == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

SDL_Surface *C_LoadImage::Load( string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 88, 82, 114 ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

string C_LoadImage::getFileName()
{
    return filename;
}       

SDL_Surface *C_LoadImage::getImage()
{
    return image;
}
#endif
