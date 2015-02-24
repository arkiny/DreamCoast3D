#pragma once
#include "iPlayerState.h"

class cPlayerMove : public iPlayerState
{
public:
	cPlayerMove();
	~cPlayerMove();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType() { return 1; }
};

