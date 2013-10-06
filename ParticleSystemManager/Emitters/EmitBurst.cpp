#include "EmitBurst.h"

EmitBurst::EmitBurst(){}

EmitBurst::EmitBurst(DWORD * globalTime)
{
	this->setCTime(globalTime);
	b_constant = false;
}