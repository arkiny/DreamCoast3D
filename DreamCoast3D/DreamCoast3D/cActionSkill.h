#pragma once
#include "cAction.h"

class cActionSkill : public cAction
{
public:
	cActionSkill();
	virtual ~cActionSkill();

	virtual void Start(){}
	virtual void Update(float fDelta) {}
	virtual void Exit(){}
};

