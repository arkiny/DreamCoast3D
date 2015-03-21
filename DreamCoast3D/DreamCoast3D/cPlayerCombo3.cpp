#include "stdafx.h"
#include "cPlayerCombo3.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame이 이동하는 거리에 따라서 캐릭터의 실제 좌표도 영향을 받아야 되고 카메라도 영향을 받아야 한다.
cPlayerCombo3::cPlayerCombo3()
{
	m_nCurrentStateType = cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO3;	//7번 (장)Combo3
	SetIsRestart(false);
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_ATTACK);
	
	/*m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_REAR;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo3] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo3R] = E_CANCEL_CANTCANCEL;*/

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;	//공격중 대기로 즉시 전환 불가
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_REAR;		//공격중 이동은 후딜캔슬
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_REAR;		//공격에 공격은 후딜캔슬
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_REAR;		//공격중 방어는 후딜캔슬
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;

}

cPlayerCombo3::~cPlayerCombo3()
{
}
void cPlayerCombo3::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	pPlayer->SetComboCount(pPlayer->GetComboCount()+1);	//평타 번호 증가
	g_pSoundManager->executeAttack();

	//pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_Combo3);
}
void cPlayerCombo3::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	for (auto p : *pPlayer->GetAttackSpheres())
	{
		pPlayer->GetGameObjDeligate()->isGameAttackSphereCollided(pPlayer, p.second);
	}

	cPlayerCommon::Execute(pPlayer, fDelta);
}
void cPlayerCombo3::Exit(cGamePlayableObject* pPlayer)
{
	//cPlayerCommon::Exit(pPlayer);
	//모션이 위치를 이동하는 경우 좌표 동기화
	SyncAfterAnimPos(pPlayer);

	SetIsDoing(false);
	//재시작되는 동작이면 재시작하고(반복)
	if (m_IsRestart)
	{
		Start(pPlayer);
	}
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO3R);
	}
}