#include "Clock.hpp"

//===================================================================================================
//               class Clock		                                                               ==
//===================================================================================================
Clock::Clock(Clock* parentClock)
	: m_parentClock(parentClock)
{
}
Clock::~Clock()
{

}

