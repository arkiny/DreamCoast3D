#pragma once

enum ECANCELTYPE
	{
		E_CANCEL_CANTCANCEL,
		E_CANCEL_REAR,
		E_CANCEL_FORCE,
		E_CANCEL_MAX
	};

class cGamePlayableObject;

__interface iPlayerState{
	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType();
	virtual std::map < int, ECANCELTYPE > GetCancelInfo();
	bool GetIsDoing();
};
//안심 백업 : 민우
//#pragma once
//
//class cGamePlayableObject;
//
//__interface iPlayerState{
//	void Start(cGamePlayableObject* pPlayer);
//	void Execute(cGamePlayableObject* pPlayer, float fDelta);
//	void Exit(cGamePlayableObject* pPlayer);
//	int GetCurrentStateType();
//};