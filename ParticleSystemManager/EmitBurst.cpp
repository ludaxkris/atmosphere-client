#include "./Emitters/EmitBurst.h"

EmitBurst::EmitBurst(CTime * p_ctime)
{
	this->setCTime(p_ctime);
	b_constant = false;
}