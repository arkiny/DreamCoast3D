#pragma once
#include "iPlayerState.h"

class cPlayerOnHit : public iPlayerState
{
public:
	cPlayerOnHit();
	~cPlayerOnHit();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType() { return 3; }
};

