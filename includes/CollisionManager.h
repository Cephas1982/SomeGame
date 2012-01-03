#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include <SDL.h>
#include <vector>
#include <math.h>
#include "Config.h"
#include "Collision.h"
#include "Enemy1.h"
#include "BaseEntity.h"
#include "BitManipulation.h"
#include "GUI.h"

class C_CollisionManager{
		private:
				C_Collision CollisionChecker;
				C_BitManip BitManip;
				//containers for data that has to be checked
				bool m_waitingForTransition;//used to  control transitions
				enum e_compareStates{DO_NOTHING, DESTROY_WEAPON1, DESTROY_WEAPON2, DESTROY_BOTH, DAMAGE_WEAPON, CM_DEFLECT_NORMAL_W1, CM_DEFLECT_NORMAL_W2
									 };
			
		public:
				C_CollisionManager();
				~C_CollisionManager();

				void Check_EntityVsTile(std::vector<C_BaseEntity*>*);
				void Check_EntityVsObjects(std::vector<C_BaseEntity*>*);
				void Check_PlayerVsEnemy(std::vector<C_BaseEntity*>*, C_GUI* );
				void Check_WeaponVsEntity(std::vector<C_BaseEntity*>*);
				void Check_WeaponVsWeapon(std::vector<C_BaseEntity*>*);

				int CompareWeaponProperties(int, int);
				int Compare_WeaponVsEntity(int, int);

};
#endif