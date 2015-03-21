#include "stdafx.h"
#include "cPlayerCombo3R.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame이 이동하는 거리에 따라서 캐릭터의 실제 좌표도 영향을 받아야 되고 카메라도 영향을 받아야 한다.
cPlayerCombo3R::cPlayerCombo3R()
{
	m_nCurrentStateType = cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO3R;	//8번 (장)Combo3R
	m_IsRestart = false;
	m_IsDoing = false;
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_IDLE);

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;		//평타후딜레이를 명시적으로 취하는 자세니까 대기만 빼고 모두 캔슬가능.
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;
}

cPlayerCombo3R::~cPlayerCombo3R()
{
}
void cPlayerCombo3R::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
}
void cPlayerCombo3R::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	cPlayerCommon::Execute(pPlayer, fDelta);
}
void cPlayerCombo3R::Exit(cGamePlayableObject* pPlayer)
{
	pPlayer->SetComboCount(0);	//평타 번호 초기화
	cPlayerCommon::Exit(pPlayer);
	
}