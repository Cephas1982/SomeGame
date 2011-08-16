#ifndef INPUTCONTROLS_H
#define INPUTCONTROLS_H
#include <SDL.h>
#include "Config.h"

class C_InputControls
{

	public:

			bool up, down, left, right, space, key_A, key_S, key_D, key_C, key_V, key_M, key_P;
			bool lock_up, lock_down, lock_left, lock_right, lock_space, lock_key_A, lock_key_S, lock_key_D, lock_key_C, lock_key_V, lock_key_M, lock_key_P;

			bool tab, tilde, key_1, key_2, key_3, key_4, key_5, key_6, key_7, key_8, key_9, key_0;
			bool lock_tab, lock_tilde, lock_key1, lock_key2, lock_key3, lock_key4, lock_key5, lock_key6, lock_key7, lock_key8, lock_key9, lock_key0; 

			bool key_F1, key_F2, key_F3, key_F4, key_F5, key_F6, key_F7, key_F8, key_F9;
			bool lock_F1, lock_F2, lock_F3, lock_F4, lock_F5, lock_F6, lock_F7, lock_F8, lock_F9;

			bool enter, escape, key_delete, key_lshift, key_rshift, key_print, key_lctrl, key_rctrl;
			bool lock_enter, lock_escape, lock_key_delete, lock_key_lshift, lock_key_rshift, lock_key_lctrl, lock_key_rctrl, lock_key_print;

			//MOUSE
			bool leftClick, rightClick, middleClick;
			bool lock_leftClick, lock_rightClick, lock_middleClick;
			bool wheelUp, wheelDown;
			bool lock_wheelUp, lock_wheelDown;

			bool lockMovement, lockControls;

			int keySim;//vaule for simulated key presses

	//public: TODO Make stuff private and access functions. Don't be lazy
		C_InputControls();
		~C_InputControls();

		void CheckInputs();

		void SimPress(int);//Simulates a quick press and release
		void SimHold(int);//Simulates a hold  TODO: have a time variable

};
#endif