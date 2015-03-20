#pragma once
#include "cPlayerCommon.h"

class cPlayerCombo1R : public cPlayerCommon
{
public:
	cPlayerCombo1R();
	~cPlayerCombo1R();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;

};

