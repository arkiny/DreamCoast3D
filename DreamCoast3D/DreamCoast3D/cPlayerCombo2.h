#pragma once
//#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerCombo2 : public cPlayerCommon
{
public:
	cPlayerCombo2();
	~cPlayerCombo2();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;
};

