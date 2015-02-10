#pragma once
#include "cGameActionSkinnedMeshObj.h"
class cZealot :
	public cGameActionSkinnedMeshObj
{
public:
	cZealot();
	~cZealot();

	virtual void SetAnimation(DWORD dwindex);
	virtual void SetAnimationLoop(DWORD dwIndex, bool bEnalbe);
};

