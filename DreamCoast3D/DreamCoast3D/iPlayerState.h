#pragma once

enum ECANCELTYPE
{
	E_CANCEL_CANTCANCEL,
	E_CANCEL_REAR,
	E_CANCEL_FORCE,
	E_CANCEL_MAX
};
enum ESTATEGROUP
{
	E_STATEGROUP_IDLE,			//대기상태 종류
	E_STATEGROUP_MOVE,			//이동상태 종류
	E_STATEGROUP_ATTACK,		//평타 종류
	E_STATEGROUP_SKILL,			//공격스킬 종류
	E_STATEGROUP_DODGE,			//회피상태 종류
	E_STATEGROUP_GUARD,			//방어상태 종류
	E_STATEGROUP_PASSIVE,		//피격, 사망, 에어본
	E_STATEGROUP_INVINCIBLE,	//무적상태
	E_STATEGROUP_END,			//사망과 같은 종료 상태
	E_STATEGROUP_MAX
};

class cGamePlayableObject;

__interface iPlayerState{
	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType();
	virtual std::map < int, ECANCELTYPE > GetCancelInfo();
	bool GetIsDoing();
	ESTATEGROUP GetCurrentStateGroup();
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