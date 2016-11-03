#pragma once
#ifndef _INCLUDE_Clock__
#define _INCLUDE_Clock__
#include "MasterClock.hpp"


//===================================================================================================
//               class Clock		                                                               ==
//===================================================================================================
class Clock : public MasterClock
{
public:
	Clock(Clock* parentClock);
	~Clock();

	Clock* GetParentClock() { return m_parentClock; }

private:
	Clock*				m_parentClock;
};




#endif