#pragma once
#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerOnHit : public cPlayerCommon
{
public:
	cPlayerOnHit();
	~cPlayerOnHit();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	//int GetCurrentStateType() { return 3; }
};
//�Ƚ� ��� : �ο�
// #pragma once
//#include "iPlayerState.h"
//
//class cPlayerOnHit : public iPlayerState
//{
//public:
//	cPlayerOnHit();
//	~cPlayerOnHit();
//
//	void Start(cGamePlayableObject* pPlayer);
//	void Execute(cGamePlayableObject* pPlayer, float fDelta);
//	void Exit(cGamePlayableObject* pPlayer);
//	int GetCurrentStateType() { return 3; }
//};
//
