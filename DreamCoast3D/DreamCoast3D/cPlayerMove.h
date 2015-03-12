#pragma once
#include "iPlayerState.h"

class cPlayerMove : public iPlayerState
{
public:
	cPlayerMove();
	~cPlayerMove();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType() { return 1; }

	// MS
	void MoveFront(cGamePlayableObject* pPlayer, float fDelta);
	void MoveBack(cGamePlayableObject* pPlayer, float fDelta);
	void MoveRight(cGamePlayableObject* pPlayer, float fDelta);
	void MoveLeft(cGamePlayableObject* pPlayer, float fDelta);

};

