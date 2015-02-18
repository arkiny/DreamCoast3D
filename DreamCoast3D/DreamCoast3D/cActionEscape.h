#pragma once
#include "cAction.h"

class cActionEscape : public cAction
{
public:
	cActionEscape();
	virtual ~cActionEscape();

	virtual void Update(float fDelta) {}
};

