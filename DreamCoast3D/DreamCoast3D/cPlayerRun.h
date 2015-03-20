#pragma once
#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerRun : public cPlayerCommon
{
public:
	cPlayerRun();
	~cPlayerRun();
	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;
	
	void MoveFront(cGamePlayableObject* pPlayer, float fDelta);
	void MoveBack(cGamePlayableObject* pPlayer, float fDelta);
	void MoveRight(cGamePlayableObject* pPlayer, float fDelta);
	void MoveLeft(cGamePlayableObject* pPlayer, float fDelta);

	//int GetCurrentStateType() { return 2; }
};

