#include "stdafx.h"
#include "cPlayerCombo3.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame�� �̵��ϴ� �Ÿ��� ���� ĳ������ ���� ��ǥ�� ������ �޾ƾ� �ǰ� ī�޶� ������ �޾ƾ� �Ѵ�.
cPlayerCombo3::cPlayerCombo3()
{
	m_nCurrentStateType = cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO3;	//7�� (��)Combo3
	SetIsRestart(false);
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_ATTACK);
	
	/*m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_REAR;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo3] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo3R] = E_CANCEL_CANTCANCEL;*/

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

cPlayerCombo3::~cPlayerCombo3()
{
}
void cPlayerCombo3::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	pPlayer->SetComboCount(pPlayer->GetComboCount()+1);	//��Ÿ ��ȣ ����
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
	//����� ��ġ�� �̵��ϴ� ��� ��ǥ ����ȭ
	SyncAfterAnimPos(pPlayer);

	SetIsDoing(false);
	//����۵Ǵ� �����̸� ������ϰ�(�ݺ�)
	if (m_IsRestart)
	{
		Start(pPlayer);
	}
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO3R);
	}
}