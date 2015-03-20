#include "stdafx.h"
#include "cPlayerCombo1R.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame�� �̵��ϴ� �Ÿ��� ���� ĳ������ ���� ��ǥ�� ������ �޾ƾ� �ǰ� ī�޶� ������ �޾ƾ� �Ѵ�.
cPlayerCombo1R::cPlayerCombo1R()
{
	m_nCurrentStateType = 4;	//4�� (��)Combo1R
	m_IsRestart = false;
	m_IsDoing = false;
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_IDLE);

	/*
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1R] = E_CANCEL_CANTCANCEL;
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

cPlayerCombo1R::~cPlayerCombo1R()
{
}
void cPlayerCombo1R::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
}
void cPlayerCombo1R::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	cPlayerCommon::Execute(pPlayer, fDelta);
}
void cPlayerCombo1R::Exit(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Exit(pPlayer);
}