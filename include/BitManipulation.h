#ifndef BITMANIP_H
#define BITMANIP_H
#include <math.h>
class C_BitManip
{
	private:

			bool m_BitChunk[32]; //this represents bits 0-15. TRUE = 1, FALSE = 0.
			typedef unsigned int INT;
	public:
			C_BitManip();
			~C_BitManip();

			void Load_BitChunk(INT);// this function takes a byte and parses each bit into the BitChunk array

			//set functions			
			bool Set_bit(INT &, int, bool);//1st parameter is an unsigned int, 2nd parameter is the bit (0-7) you want toggled. Returns the new value
			
			//get functions
			INT Get_bitChunk();// parses m_BitChunk array and returns the vaule of m_updatedByte;
			bool  Get_bit(INT, int);// Parameter 1 is the integer to parse, parameter 2 is the bit flag you want returned. 
								   // (0-31 are valid parameters) true = on, false = off
};
#endif