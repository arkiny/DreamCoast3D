#pragma once
#include "cAction.h"

// AI의 모든 액션의 중심
// 모든 AI는 계속 생각하고 있으면서
// 주변에 반응한다.

class cAcitionThink : public cAction
{
public:
	cAcitionThink();
	virtual ~cAcitionThink();

	virtual void Start(){}
	virtual void Update(float fDelta) {}
	virtual void Exit(){}
};

