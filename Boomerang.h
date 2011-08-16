#ifndef BOOMERANG_H
#define BOOMERANG_H
#include <SDL.h>
#include "BaseWeapon.h"

/*
#define PI 3.1415926535
//FLAGS
#define DAMAGE_ENEMIES		1 //if on, this should inflict damage on enemies
#define DAMAGE_FRIENDLIES	2 //if on, this should damage players
#define WEAPON_ACTIVE		3 //if on, weapon is active

*/
class C_Boomerang : public C_BaseWeapon {

	private:
			bool comeBack;// this is true when it comes back
			int spin;


	public:
			C_Boomerang(int *, int *, int* );//gets pointer to x/y player position, and 3: angle player facing
			~C_Boomerang();


			void Update();
			void Move();
			void Render();
};
#endif