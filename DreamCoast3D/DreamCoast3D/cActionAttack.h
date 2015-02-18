#pragma once
#include "cAction.h"

class cActionAttack : public cAction
{
public:
	cActionAttack();
	virtual ~cActionAttack();

	virtual void Update(float fDelta) {}
};

