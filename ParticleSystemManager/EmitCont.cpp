#include "./Emitters/EmitCont.h"

EmitCont::EmitCont(CTime * p_ctime)
{
	this->setCTime(p_ctime);
	b_constant = true;
}

