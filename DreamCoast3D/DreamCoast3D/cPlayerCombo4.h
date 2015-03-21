#pragma once
//#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerCombo4 : public cPlayerCommon
{
public:
	cPlayerCombo4();
	~cPlayerCombo4();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;
};

