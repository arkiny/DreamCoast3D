#pragma once

class cUIObject;

__interface iUIManagerDeligate
{
	void ChangeScene(int nScene, cUIObject* pSender);
};
