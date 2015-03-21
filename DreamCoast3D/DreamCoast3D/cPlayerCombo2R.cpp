#include "stdafx.h"
#include "cPlayerCombo2R.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame�� �̵��ϴ� �Ÿ��� ���� ĳ������ ���� ��ǥ�� ������ �޾ƾ� �ǰ� ī�޶� ������ �޾ƾ� �Ѵ�.
cPlayerCombo2R::cPlayerCombo2R()
{
	m_nCurrentStateType = cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO2R;	//6�� (��)Combo2R
	m_IsRestart = false;
	m_IsDoing = false;
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_IDLE);

	/*
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo2] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo2R] = E_CANCEL_CANTCANCEL;
	*/

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;		//��Ÿ�ĵ����̸� ��������� ���ϴ� �ڼ��ϱ� ��⸸ ���� ��� ĵ������.
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;

}

cPlayerCombo2R::~cPlayerCombo2R()
{
}
void cPlayerCombo2R::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
}
void cPlayerCombo2R::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	cPlayerCommon::Execute(pPlayer, fDelta);
}
void cPlayerCombo2R::Exit(cGamePlayableObject* pPlayer)
{
	pPlayer->SetComboCount(0);	//��Ÿ ��ȣ �ʱ�ȭ
	//����� ��ġ�� �̵��ϴ� ��� ��ǥ ����ȭ
	SyncAfterAnimPos(pPlayer);
	cPlayerCommon::Exit(pPlayer);
}