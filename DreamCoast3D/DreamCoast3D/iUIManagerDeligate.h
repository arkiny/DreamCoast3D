#pragma once

#include "iInventoryControl.h"

class cUIObject;

__interface iUIManagerDeligate
{
	void ChangeScene(int nScene, cUIObject* pSender);
	void SetShowCursor(bool isShow);
	iInventoryControl* GetInventoryControl();
};

