#pragma once

#include "cGameMapHeight.h"
class cGridSystem;

class cGameMapHeightGridSystem : public cGameMapHeight
{
public:
	cGameMapHeightGridSystem();
	virtual 	~cGameMapHeightGridSystem();

	virtual void Setup();
};

