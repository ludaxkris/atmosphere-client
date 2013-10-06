#pragma once
#include "./../Emitter.h"

class EmitBurst : public Emitter
{
	public:
		EmitBurst();
		EmitBurst(DWORD * globalTime);
};