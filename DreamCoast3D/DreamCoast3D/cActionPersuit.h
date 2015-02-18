#pragma once
#include "cAction.h"

class cActionPersuit : public cAction
{
public:
	cActionPersuit();
	virtual ~cActionPersuit();

	virtual void Update(float fDelta) {}
};

