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

//�Ƚ� ��� : �ο�
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

