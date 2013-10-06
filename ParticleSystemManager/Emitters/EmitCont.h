#pragma once
#include "./../Emitter.h"

class EmitCont : public Emitter
{
	public:
		EmitCont();
		EmitCont(DWORD * globalTime);
};