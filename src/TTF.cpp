//This will control text to screen output 
#ifndef TTF_CPP
#define TTF_CPP
#include "TTF.h"
//extern SDL_Surface *screen;
C_Font::C_Font()
{
	bool Goddamnit = false;

    //The font that's going to be used 
	if((arial12 = TTF_OpenFont("Fonts\\arial.ttf", 12)) == NULL)
		Goddamnit = true;
	arial14 = TTF_OpenFont("Fonts\\arial.ttf", 14);
	arial16 = TTF_OpenFont("Fonts\\arial.ttf", 16);
	arial18 = TTF_OpenFont("Fonts\\arial.ttf", 18);
	arial20 = TTF_OpenFont("Fonts\\arial.ttf", 20);
	arial22 = TTF_OpenFont("Fonts\\arial.ttf", 22);



    //Set text color
    textColor.r = 255;
    textColor.g = 255;
    textColor.b = 250;
    txtPos.x = 0;
    txtPos.y = 440;
    
}

C_Font::~C_Font()
{
	TTF_CloseFont(arial12);
	TTF_CloseFont(arial14);
	TTF_CloseFont(arial16);
	TTF_CloseFont(arial18);
	TTF_CloseFont(arial20);
	TTF_CloseFont(arial22);
}

void C_Font::Print(std::stringstream *output, SDL_Surface *screen, int fontType, int x, int y,  int red, int green, int blue)
{
	switch(fontType){
		case 12: font = arial12; break;
		case 14: font = arial14; break;
		case 16: font = arial16; break;
		case 18: font = arial18; break;
		case 20: font = arial20; break;
		case 22: font = arial22; break;
		default: font = arial12; break;
	}
	txtPos.x = x;
	txtPos.y = y;

	textColor.r = red;
    textColor.g = green;
    textColor.b = blue;

    message = TTF_RenderText_Solid( font , output->str().c_str() , textColor );
    SDL_BlitSurface( message, NULL, screen, &txtPos ); 

	//clear stream		
	output->clear();
	output->str("");		

	SDL_FreeSurface( message);
}


#endif