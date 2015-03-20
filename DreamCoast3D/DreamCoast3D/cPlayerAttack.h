#pragma once
#include "iPlayerState.h"

class cPlayerAttack : public iPlayerState
{
public:
	cPlayerAttack();
	~cPlayerAttack();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType() { return 2; }
};

//안심 백업 : 민우
//#pragma once
//#include "iPlayerState.h"
//
//class cPlayerAttack : public iPlayerState
//{
//public:
//	cPlayerAttack();
//	~cPlayerAttack();
//
//	void Start(cGamePlayableObject* pPlayer);
//	void Execute(cGamePlayableObject* pPlayer, float fDelta);
//	void Exit(cGamePlayableObject* pPlayer);
//	int GetCurrentStateType() { return 2; }
//};

