#include "stdafx.h"
#include "cPlayerCombo4.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame�� �̵��ϴ� �Ÿ��� ���� ĳ������ ���� ��ǥ�� ������ �޾ƾ� �ǰ� ī�޶� ������ �޾ƾ� �Ѵ�.
cPlayerCombo4::cPlayerCombo4()
{
	//cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO2;
	m_nCurrentStateType = cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO4;	//9�� (��)Combo4
	SetIsRestart(false);
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_ATTACK);
	
	/*m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_REAR;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo4] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo4R] = E_CANCEL_CANTCANCEL;*/

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;	//������ ���� ��� ��ȯ �Ұ�
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_REAR;		//������ �̵��� �ĵ�ĵ��
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_REAR;		//���ݿ� ������ �ĵ�ĵ��
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_REAR;		//������ ���� �ĵ�ĵ��
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;
}

cPlayerCombo4::~cPlayerCombo4()
{
}
void cPlayerCombo4::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	pPlayer->SetComboCount(0);	//��Ÿ ��ȣ �ʱ�ȭ
	g_pSoundManager->executeAttack();
}
void cPlayerCombo4::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	for (auto p : *pPlayer->GetAttackSpheres())
	{
		pPlayer->GetGameObjDeligate()->isGameAttackSphereCollided(pPlayer, p.second);
	}

	cPlayerCommon::Execute(pPlayer, fDelta);
}
void cPlayerCombo4::Exit(cGamePlayableObject* pPlayer)
{
	pPlayer->SetComboCount(0);	//��Ÿ ��ȣ �ʱ�ȭ

	//����� ��ġ�� �̵��ϴ� ��� ��ǥ ����ȭ
	SyncAfterAnimPos(pPlayer);

	cPlayerCommon::Exit(pPlayer);
}