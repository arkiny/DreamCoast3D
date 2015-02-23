#pragma once
#include "iPlayerState.h"

class cPlayerIdle : public iPlayerState
{
public:
	cPlayerIdle();
	~cPlayerIdle();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
};

