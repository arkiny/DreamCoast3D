#include "stdafx.h"
#include "cPlayerUnarmedWait.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerUnarmedWait::cPlayerUnarmedWait()
{
	m_nCurrentStateType = 1; //1번 Idle
	SetIsRestart(true);	//상태 재시작 여부
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_IDLE);

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;		//대기가 끝나면 다시 대기
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;

}


cPlayerUnarmedWait::~cPlayerUnarmedWait()
{
}
void cPlayerUnarmedWait::Start(cGamePlayableObject* pPlayer)
{
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_UNARMEDWAIT);
}
void cPlayerUnarmedWait::Execute(cGamePlayableObject* pPlayer, float fDelta)
{

}
void cPlayerUnarmedWait::Exit(cGamePlayableObject* pPlayer)
{

}