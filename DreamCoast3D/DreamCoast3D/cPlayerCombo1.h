#pragma once
//#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerCombo1 : public cPlayerCommon
{
public:
	cPlayerCombo1();
	~cPlayerCombo1();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;
	
	//int GetCurrentStateType() { return 3; }
};

