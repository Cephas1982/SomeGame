#ifndef ENEMY1_H
#define ENEMY1_H
#include "Boomerang.h"
#include "BaseEntity.h"

class C_Enemy1 : public C_BaseEntity{

	protected: 
				float m_distanceTraveledX, m_distanceTraveledY;//track how long enemy has moved in a direction
				
				int m_directionMoving;

				SDL_Rect m_cage;//limit where enemy can move to this area
		
				bool m_startNewMove;

				
	public:
			C_Enemy1();
			~C_Enemy1();

			void Render();//draws player
			void HandleInputs(C_InputControls *);//deals with input
			void Attack();//runs inside HandleInputs and determines how to attack
			void Move();
};

#endif