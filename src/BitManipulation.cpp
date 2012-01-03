#ifndef BITMANIP_CPP
#define BITMANIP_CPP
#include "BitManipulation.h"

C_BitManip::C_BitManip()
{
}
C_BitManip::~C_BitManip()
{
}

void C_BitManip::Load_BitChunk(INT newInt)
{
	double base = 2;
	for(int i = 0; i < 32; i++){
		int testByte = pow(base,i);
		testByte = testByte & newInt;
		if(testByte == pow(base,i))//if bit 'i' is on/off set the bitchunk accordingly
			m_BitChunk[i] = true;
		else
			m_BitChunk[i] = false;
	}
}

bool C_BitManip::Set_bit(INT &r_newInt, int bit_to_toggle, bool onOff)
{
	bool valueOK = true;
	if(bit_to_toggle >= 0 && bit_to_toggle <= 31){//bit_to_toggle must be between 0-7
		Load_BitChunk(r_newInt);//load up the bitchunk flags

		m_BitChunk[bit_to_toggle] = onOff;	//turn on or off based on onOff

		INT asdf = Get_bitChunk();

		r_newInt = Get_bitChunk();//reparse
	}
	return valueOK;
}

unsigned int C_BitManip::Get_bitChunk()
{
	INT newChunk = 0;
	for(int i = 0; i < 32; i++){
		if(m_BitChunk[i])//if this bit is on
			newChunk += pow(double(2), i);
	}

	return newChunk;
}

bool C_BitManip::Get_bit(INT sampleInt, int bit_to_get)
{	
	bool bitFlag = false;
	if(bit_to_get >= 0 && bit_to_get <= 31){//bit_to_get must be between 0-7
		
		Load_BitChunk(sampleInt);//set the chunks for this byte
	
		if(m_BitChunk[bit_to_get])//if the bit flag is on
			bitFlag = true;		  //update bitFlag
	}
	//TODO: Error checking necessary?? add a pause or exit program line
	return bitFlag;
}
#endif