#pragma once
#include "cAction.h"

class cActionAttack : public cAction
{
public:
	cActionAttack();
	virtual ~cActionAttack();

	virtual void Start(){}
	virtual void Update(float fDelta) {}
	virtual void Exit(){}
};

