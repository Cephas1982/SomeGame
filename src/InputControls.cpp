#ifndef INPUTCONTROLS_CPP
#define INPUTCONTROLS_CPP
#include "InputControls.h"
extern SDL_Event event;
extern bool MapEditorMenuActive;
C_InputControls::C_InputControls()
{
	up = false;
	down = false;
	left = false;
	right = false;
	space = false;
	key_A = false;
	key_S = false;
	key_D = false;
	key_C = false;
	key_V = false;
	key_M = false;
	key_P = false;

	key_F1 = false;
	key_F2 = false;
	key_F3 = false;
	key_F4 = false;
	key_F8 = false;

	lock_up = false;
	lock_down = false;
	lock_left = false;
	lock_right = false;
	lock_space = false;
	lock_key_A = false;
	lock_key_S = false;
	lock_key_D = false;
	lock_key_C = false;
	lock_key_V = false;
	lock_key_M = false;
	lock_key_P = false;
	
	lock_key_delete = false;
	lock_key_lctrl = false;
	lock_key_rctrl = false;

	lockMovement = false;
	lockControls = false;

	//MOUSE
	leftClick = false;
	rightClick = false;
	middleClick = false;
	lock_middleClick = false;
	lock_leftClick = false;
	lock_rightClick = false;


	wheelUp = false;
	wheelDown = false;
}

C_InputControls::~C_InputControls()
{
}

void C_InputControls::CheckInputs()
{
	//To get fresh input values every pass need to reset these variables

	space = false;
	enter = false;
	escape = false;
	key_print = false;
	key_A = false;
	key_S = false;
	key_D = false;
	key_C = false;
	key_V = false;
	key_M = false;
	key_P = false;
	key_F8 = false;

	tab =	false;
	key_lctrl = false;
	key_rctrl = false;
	//mouse refresh
	middleClick = false;
	if(MapEditorMenuActive && !key_lshift){
		leftClick = false;//clicks are false because i want to spam tiles outside map mode
		rightClick = false;
		
		up = false;
		down = false;
		left = false;
		right = false;
	}
/*	lock_leftClick = false;
	lock_rightClick = false;
*/
//	while(SDL_PollEvent(&event)){//while events are in queue
		switch(event.type){
			//KEYBOARD CONTROLS
			case SDL_KEYDOWN: 
							  if(!lock_up && event.key.keysym.sym == SDLK_UP){//UP
								  up = true;
								  lock_up = true;
							  }
			                  if(!lock_down && event.key.keysym.sym == SDLK_DOWN){//DOWN
								  down = true;
								  lock_down = true;
							  }
			                  if(!lock_left && event.key.keysym.sym == SDLK_LEFT){//LEFT
								  left = true;
								  lock_left = true;
							  }
			                  if(!lock_right && event.key.keysym.sym == SDLK_RIGHT){//RIGHT
								  right = true;
								  lock_right = true;
							  }
							  if(!lock_space && event.key.keysym.sym == SDLK_SPACE){//SPACE BAR
								  space = true;
								  lock_space = true;
							  }
							  if(!lock_key_A && event.key.keysym.sym == SDLK_a){//A Key
								  key_A = true;
								  lock_key_A = true;
							  }
							  if(!lock_key_S && event.key.keysym.sym == SDLK_s){//S Key
								  key_S = true;
								  lock_key_S = true;
							  }
							  if(!lock_key_D && event.key.keysym.sym == SDLK_d){//D Key
								  key_D = true;
								  lock_key_D = true;
							  }
							  if(!lock_key_C && event.key.keysym.sym == SDLK_c){//C Key
								  key_C = true;
								  lock_key_C = true;
							  }
							  if(!lock_key_V && event.key.keysym.sym == SDLK_v){//V Key
								  key_V = true;
								  lock_key_V = true;
							  }
							  if(!lock_key_M && event.key.keysym.sym == SDLK_m){//M Key
								  key_M = true;
								  lock_key_M = true;
							  }
							  if(!lock_key_P && event.key.keysym.sym == SDLK_p){//P Key
								  key_P = true;
								  lock_key_P = true;
							  }
							  if(!lock_F1 && event.key.keysym.sym == SDLK_F1){//F1 Key
								  key_F1 = true;
								  lock_F1 = true;
							  }
							  if(!lock_F2 && event.key.keysym.sym == SDLK_F2){//F2 Key
								  key_F2 = true;
								  lock_F2 = true;
							  }
							  if(!lock_F3 && event.key.keysym.sym == SDLK_F3){//F3 Key
								  key_F3 = true;
								  lock_F3 = true;
							  }
							  if(!lock_F4 && event.key.keysym.sym == SDLK_F4){//F4 Key
								  key_F4 = true;
								  lock_F4 = true;
							  }
							  if(!lock_F8 && event.key.keysym.sym == SDLK_F8){//F8 Key
								  key_F8 = true;
								  lock_F8 = true;
							  }
   							  if(!lock_tab && event.key.keysym.sym == SDLK_TAB){//TAB key
								  tab = true;
								  lock_tab = true;
							  }
							  if(!lock_enter && event.key.keysym.sym == SDLK_RETURN){//ENTER key
								  enter = true;
								  lock_enter = true;
							  }
							  if(!lock_escape && event.key.keysym.sym == SDLK_ESCAPE){//ESCAPE key
								  escape = true;
								  lock_escape = true;
							  }
							  if(!lock_key_delete && event.key.keysym.sym == SDLK_DELETE){//DELETE key
								  key_delete = true;
								  lock_key_delete = true;
							  }
							  if(!lock_key_lshift && event.key.keysym.sym == SDLK_LSHIFT){//LEFT SHIFT key
								  key_lshift = true;
								  lock_key_lshift = true;
							  }
							  if(!lock_key_lctrl && event.key.keysym.sym == SDLK_LCTRL){//LEFT CTRL key
								  key_lctrl = true;
								  lock_key_lctrl = true;
							  }
							  if(!lock_key_rctrl && event.key.keysym.sym == SDLK_RCTRL){//RIGHT CTRL key
								  key_rctrl = true;
								  lock_key_rctrl = true;
							  }
							  if(!lock_key_rshift && event.key.keysym.sym == SDLK_DELETE){//RIGHT SHIFT key
								  key_rshift = true;
								  lock_key_rshift = true;
							  }
							  if(!lock_key_print && event.key.keysym.sym == SDLK_PRINT){//PRINT SCREEN key
								  key_print = true;
								  lock_key_print = true;
							  }
							  break;

							  

			case SDL_KEYUP:   
							  if(event.key.keysym.sym == SDLK_UP){//UP
								  up = false;
								  lock_up = false;
							  }
			                  if(event.key.keysym.sym == SDLK_DOWN){//DOWN
								  down = false;
								  lock_down = false;
							  }
			                  if(event.key.keysym.sym == SDLK_LEFT){//LEFT
								  left = false;
								  lock_left = false;
							  }
			                  if(event.key.keysym.sym == SDLK_RIGHT){//RIGHT
								  right = false;
								  lock_right = false;
							  }
							  if(event.key.keysym.sym == SDLK_SPACE){//SPACE BAR
								  space = false;
								  lock_space = false;
							  }
							  if(event.key.keysym.sym == SDLK_a){//A Key
								  key_A = false;
								  lock_key_A = false;
							  }
							  if(event.key.keysym.sym == SDLK_s){//S Key
								  key_S = false;
								  lock_key_S = false;
							  }
							  if(event.key.keysym.sym == SDLK_d){//D Key
								  key_D = false;
								  lock_key_D = false;
							  }
							  if(event.key.keysym.sym == SDLK_c){//C Key
								  key_C = false;
								  lock_key_C = false;
							  }
							  if(event.key.keysym.sym == SDLK_v){//V Key
								  key_V = false;
								  lock_key_V = false;
							  }
							  if(event.key.keysym.sym == SDLK_m){//M Key
								  key_M = false;
								  lock_key_M = false;
							  }
							  if(event.key.keysym.sym == SDLK_p){//P Key
								  key_P = false;
								  lock_key_P = false;
							  }
							  if(event.key.keysym.sym == SDLK_F1){//F1 Key
								  key_F1 = false;
								  lock_F1 = false;
							  }
							  if(event.key.keysym.sym == SDLK_F2){//F2 Key
								  key_F2 = false;
								  lock_F2 = false;
							  }
							  if(event.key.keysym.sym == SDLK_F3){//F3 Key
								  key_F3 = false;
								  lock_F3 = false;
							  }
							  if(event.key.keysym.sym == SDLK_F4){//F4 Key
								  key_F4 = false;
								  lock_F4 = false;
							  }
							  if(event.key.keysym.sym == SDLK_F8){//F8 Key
								  key_F8 = false;
								  lock_F8 = false;
							  }
							  if(event.key.keysym.sym == SDLK_TAB){//TAB Key
								  tab = false;
								  lock_tab = false;
							  }
							  if(event.key.keysym.sym == SDLK_RETURN){//ENTER Key
								  enter = false;
								  lock_enter = false;
							  }
							  if(event.key.keysym.sym == SDLK_ESCAPE){//ESCAPE Key
								  escape = false;
								  lock_escape = false;
							  }
							  if(event.key.keysym.sym == SDLK_DELETE){//DELETE Key
								  key_delete = false;
								  lock_key_delete = false;
							  }
							  if(event.key.keysym.sym == SDLK_LSHIFT){//LEFT SHIFT Key
								  key_lshift = false;
								  lock_key_lshift = false;
							  }					
							  if(event.key.keysym.sym == SDLK_RSHIFT){//RIGHT SHIFT Key
								  key_rshift = false;
								  lock_key_rshift = false;
							  }
							  if(event.key.keysym.sym == SDLK_LCTRL){//LEFT CTRL Key
								  key_lctrl = false;
								  lock_key_lctrl = false;
							  }
							  if(event.key.keysym.sym == SDLK_RCTRL){//RIGHT CTRL Key
								  key_rctrl = false;
								  lock_key_rctrl = false;
							  }
							  if(event.key.keysym.sym == SDLK_PRINT){//PRINT SCREEN Key
								  key_print = false;
								  lock_key_print = false;
							  }
							break;

			//MOUSE CONTROLS
			case SDL_MOUSEBUTTONDOWN:
							  if(!lock_leftClick && event.button.button == SDL_BUTTON_LEFT ){//LEFT CLICK
								  leftClick = true;
								  lock_leftClick = true;
							  }
							  if(!lock_rightClick && event.button.button == SDL_BUTTON_RIGHT){//RIGHT CLICK
								  rightClick = true;
								  lock_rightClick = true;
							  }
							  if(!lock_middleClick && event.button.button == SDL_BUTTON_MIDDLE){//MIDDLE CLICK
								  middleClick = true;
								  lock_middleClick = true;
							  }
   							  if(!lock_wheelUp && event.button.button == SDL_BUTTON_WHEELUP){//WHEEL UP
								  wheelUp = true;
								  lock_wheelUp = true;
							  }
							  if(!lock_wheelUp && event.button.button == SDL_BUTTON_WHEELDOWN){//WHEEL DOWN
								  wheelDown = true;
								  lock_wheelDown = true;
							  }
   							  break;

			case SDL_MOUSEBUTTONUP:
 							  if(event.button.button == SDL_BUTTON_LEFT){//LEFT CLICK
									leftClick = false;
									lock_leftClick = false;
							  }
						      if(event.button.button == SDL_BUTTON_RIGHT){//RIGHT CLICK
									rightClick = false;
									lock_rightClick = false;
							  }
							  if(event.button.button == SDL_BUTTON_MIDDLE){//MIDDLE CLICK
									middleClick = false;
									lock_middleClick = false;
							  }
							  if(event.button.button == SDL_BUTTON_WHEELUP){//WHEEL UP
								  wheelUp = false;
								  lock_wheelUp = false;
							  }
							  if(event.button.button == SDL_BUTTON_WHEELDOWN){//WHEEL DOWN
								  wheelDown = false;
								  lock_wheelDown = false;
							  }
   							  break;
		}//END SWITCH


		if(lockMovement){
			up = false;
			down = false;
			left = false;
			right = false;
		}
//	}
}

void C_InputControls::SimPress(int keyToPress)
{
	//will finish later
}


void C_InputControls::SimHold(int keyToHold)
{
}

#endif