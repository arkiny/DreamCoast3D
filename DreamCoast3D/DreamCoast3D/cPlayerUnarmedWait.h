#pragma once
//#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerUnarmedWait : public cPlayerCommon
{
public:
	cPlayerUnarmedWait();
	~cPlayerUnarmedWait();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	//int GetCurrentStateType() { return 0; }

};

