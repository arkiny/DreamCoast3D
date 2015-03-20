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

	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1R] = E_CANCEL_CANTCANCEL;
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