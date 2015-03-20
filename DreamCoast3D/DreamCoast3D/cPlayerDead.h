#pragma once

#include "cPlayerCommon.h"

class cPlayerDead : public cPlayerCommon
{
public:
	cPlayerDead();
	~cPlayerDead();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	//int GetCurrentStateType() { return 0; }

	float m_fDeadAccumTime;
};

