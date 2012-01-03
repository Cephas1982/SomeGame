#ifndef SWORD_H
#define SWORD_H
#include "BaseWeapon.h"

class C_Sword : public C_BaseWeapon{
	
	private:
			int frameCounter;
			bool m_maxReached;

	public:
			C_Sword(int*, int*, int*);
			~C_Sword();


			void Update();
			void Move();
			void Render();

};
#endif