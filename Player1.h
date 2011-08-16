#ifndef PLAYER1_H
#define PLAYER1_H
#include "BaseEntity.h"


class C_Player1 : public C_BaseEntity{

	protected: 

			Uint32 damagedTimePassed;
		

	public:
			C_Player1();
			~C_Player1();

			void Render();//draws player
			void HandleInputs(C_InputControls *);//deals with input
			void Move();

			//Custom set functions
			bool Set_hitPoints(int);
};

#endif