#pragma once
#include "cAction.h"

class cActionEscape : public cAction
{
public:
	cActionEscape();
	virtual ~cActionEscape();

	virtual void Start(){}
	virtual void Update(float fDelta) {}
	virtual void Exit(){}
};

