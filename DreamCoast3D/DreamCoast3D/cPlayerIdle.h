#pragma once
//#include "iPlayerState.h"
#include "cPlayerCommon.h"

class cPlayerIdle : public cPlayerCommon
{
public:
	cPlayerIdle();
	~cPlayerIdle();

	void Start(cGamePlayableObject* pPlayer) override;
	void Execute(cGamePlayableObject* pPlayer, float fDelta) override;
	void Exit(cGamePlayableObject* pPlayer) override;
	
	//virtual int GetCurrentStateType() { return 1; }
	//int GetCurrentStateType() { return cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE; }
};


//안심 백업 : 민우
//#pragma once
//#include "iPlayerState.h"
//
//class cPlayerIdle : public iPlayerState
//{
//public:
//	cPlayerIdle();
//	~cPlayerIdle();
//
//	void Start(cGamePlayableObject* pPlayer);
//	void Execute(cGamePlayableObject* pPlayer, float fDelta);
//	void Exit(cGamePlayableObject* pPlayer);
//	int GetCurrentStateType() { return 0; }
//};
//
