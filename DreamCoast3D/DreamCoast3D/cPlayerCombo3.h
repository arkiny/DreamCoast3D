#pragma once
//#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerCombo3 : public cPlayerCommon
{
public:
	cPlayerCombo3();
	~cPlayerCombo3();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;
	
	//int GetCurrentStateType() { return 3; }
};

