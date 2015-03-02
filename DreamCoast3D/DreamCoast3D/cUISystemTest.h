#pragma once

#include "cUIWindow.h"

class cUISystemTest : public cUIWindow
{
public:
	cUISystemTest();
	virtual ~cUISystemTest();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
};

