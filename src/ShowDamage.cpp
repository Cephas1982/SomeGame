#ifndef SHOWDAMAGE_CPP
#define SHOWDAMAGE_CPP
#include "ShowDamage.h"
C_ShowDamage::C_ShowDamage()
{
	//init stuff
	is_finished = true;
	m_counter = 0;
	m_damage = 0;
	m_fontSize = 12;
}
C_ShowDamage::~C_ShowDamage()
{
}
/*
bool C_ShowDamage::DisplayDamage()
{
	int fontType = 12;
	//determine font & color based on damage (12 is smallest font type)
	if(damage >= 7)
		fontType = 14;
	if(damage >= 10)
		fontType = 16;
	if(damage >= 15)
		fontType = 20;
	if(damage > 20)
		fontType = 22;

	//clear stream
	text.clear();
	text.str("");
	text << damage; //output damage
	Print(&text, screen, txtPos.x, txtPos.y, fontType);
	

	counter++;
	txtPos.y -= 2;
	if(counter == 45 ){
		is_finished = true;
		counter = 0;
		//clear stream
		text.clear();
		text.str("");
	}
	return is_finished;
}

//get functions
bool C_ShowDamage::getIsFinished()
{
	return is_finished;
}
*/

//set functions
void C_ShowDamage::Start(SDL_Rect hitbox, int dmg)
{
	is_finished = false;
	m_damage = dmg;
	m_counter = 30;
	m_txtPos = hitbox;
	m_txtPos.x = m_txtPos.x + (m_txtPos.w/2);// - Camera->getX(); //position on top of enemy and in the middle
	m_txtPos.y = m_txtPos.y;// - Camera->getY();
}

#endif