#pragma once
#include "Emitter.h"

class EmitBurst : public Emitter
{
	public:
		EmitBurst(CTime * p_ctime);
		~EmitBurst(void);
};