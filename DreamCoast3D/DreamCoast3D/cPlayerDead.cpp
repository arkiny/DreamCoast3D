#include "stdafx.h"
#include "cPlayerDead.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerDead::cPlayerDead()
	: m_fDeadAccumTime(0.f)
{
	m_nCurrentStateType = 12;	//12번 (장)사망
	m_IsRestart = false;
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_END);//사망하면 끝

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;			//사망은 최고 우선순위. 아무것도 사망을 캔슬하지 못한다
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_CANTCANCEL;

}


cPlayerDead::~cPlayerDead()
{
}

void cPlayerDead::Start(cGamePlayableObject* pPlayer)
{
}

void cPlayerDead::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	//cPlayerCommon::Execute(pPlayer, fDelta);

	m_fDeadAccumTime += fDelta;
	if (m_fDeadAccumTime >= 2.0f)
	{
		m_fDeadAccumTime = 0.f;

		pPlayer->GetEventDelegate()->DeadScene(true);
	}
}

void cPlayerDead::Exit(cGamePlayableObject* pPlayer)
{
	//pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_DEAD);
}
