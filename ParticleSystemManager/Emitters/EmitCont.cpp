#include "EmitCont.h"

EmitCont::EmitCont(){}

EmitCont::EmitCont(DWORD * globalTime)
{
	this->setCTime(globalTime);
	this->b_constant = true;
}

