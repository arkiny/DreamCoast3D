#pragma once
#include "cPlayerCommon.h"

class cPlayerCombo2R : public cPlayerCommon
{
public:
	cPlayerCombo2R();
	~cPlayerCombo2R();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;

};

