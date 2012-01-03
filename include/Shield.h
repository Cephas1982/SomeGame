#ifndef SHIELD_H
#define SHIELD_H
#include "BaseWeapon.h"

class C_Shield : public C_BaseWeapon{


	private:
			int* mW_p_playerAngle;
			bool mW_turnShield;//true while animation is happening
			bool mW_strike;//true when using shield strike
			bool mW_returnShield; //used when shield needs to come back
			bool mW_shieldMaxed;// used for shield morphs
			bool m_turnLeft, m_turnRight;

			//testing 
			bool bob;
			float bobX, bobY;
			bool bobReturnX, bobReturnY;
	

	public:
			C_Shield(int*, int*, int*); //pointer to the players x, y, and angle
			~C_Shield();

			void Update();
			void Move();
			void Render();
			
			void Animation_Turn();
			void Animation_Strike();
	
};
#endif