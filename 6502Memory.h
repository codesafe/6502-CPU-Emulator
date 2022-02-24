#ifndef MEMORY_H
#define MEMORY_H

#include "Predef.h"

class Memory
{
	private:
		BYTE* memory;


	public:
		Memory();
		~Memory();

		void Create();
		void Destroy();

		BYTE Fetch();
};


#endif