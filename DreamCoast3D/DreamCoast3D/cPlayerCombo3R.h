#pragma once
#include "cPlayerCommon.h"

class cPlayerCombo3R : public cPlayerCommon
{
public:
	cPlayerCombo3R();
	~cPlayerCombo3R();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;

};

