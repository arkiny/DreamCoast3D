#include "stdafx.h"
#include "cPlayerCommon.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"
#include "cPlayerDead.h"

//���� �� ������Ʈ�� ���, �̵�, ����, �ǰ�, ���

cPlayerCommon::cPlayerCommon()
{
	m_nCurrentStateType = 0;
	m_IsRestart = false;
	m_IsDoing = false;
	m_eStateGroup = ESTATEGROUP::E_STATEGROUP_IDLE;
	m_fDeadAccumTime = 0.f;
}


cPlayerCommon::~cPlayerCommon()
{
}
void cPlayerCommon::Start(cGamePlayableObject* pPlayer)
{
	SetIsDoing(true);
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(m_nCurrentStateType);
	pPlayer->SetStatePassedTime(0.0f);
}
void cPlayerCommon::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	//if (pPlayer->GetStatInfo()->fCurrentHp <= 0)
	//{
	//	pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_DEAD);

	//	// ���� �̺κ� ��������
	//	m_fDeadAccumTime += fDelta;
	//	if (m_fDeadAccumTime >= 2.0f)
	//	{
	//		m_fDeadAccumTime = 0.f;

	//		pPlayer->GetEventDelegate()->DeadScene(true);
	//	}
	//	///////////////////////////////////////////////////////
	//	return;
	//}
	//��ų���
	if (g_pControlManager->GetInputInfo('C'))
	{
		if (pPlayer->GetSkill1CoolTime() >= pPlayer->GetSkill1DelayTime()
			&& pPlayer->GetStatInfo()->fCurrentMana > 10.0f){
			pPlayer->SetSkill1CoolTime(0.0f);
			pPlayer->GetStatInfo()->fCurrentMana -= 10.0f;
			pPlayer->GetGameObjDeligate()->RangeSkill(pPlayer);
		}
	}

	//� �����̵� ����Ű�� �ԷµǸ� RUN������ �õ��Ѵ�
	if (g_pControlManager->GetInputInfo('W') || 
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('D'))
	{
		//RUN���� �ƴҶ��� RUN���� ��ȯ�� �õ��Ѵ�
		if (pPlayer->GetPlayableState() != pPlayer->EPLAYABLESTATE_RUN)
		{
			pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_RUN);
			//return;
		}
	}
	//��Ŭ���� �������� ��Ÿ�� ��ȯ�� �õ��Ѵ�
	if (g_pControlManager->GetInputInfo(VK_LBUTTON))
	{
		//�÷��̾ ���� ���° ��Ÿ�� ����ϴ��� �޾ƿ�
		size_t nComboCount = pPlayer->GetComboCount();
		if (nComboCount == 0)	{ pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO1); }
		else if (nComboCount == 1)	{ pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO2); }
		//return;
	}
	
	pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);
	//�귯�� �ð��� �ִϸ��̼� �������̸� �ʰ��ϸ� �ִϸ��̼� ����. �������� fDelta�� �����ִ°� ���� �ִϸ��̼��� �߸������� ��Ƣ�� �ϱ� ���� �ӽù���
	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() <= pPlayer->GetStatePassedTime() + DBL_EPSILON + fDelta*2)
	{
		//SetIsDoing(false);
		Exit(pPlayer);		//�ִϸ��̼� �����Ű��
	}
}
void cPlayerCommon::Exit(cGamePlayableObject* pPlayer)
{
	SetIsDoing(false);
	//����۵Ǵ� �����̸� ������ϰ�(�ݺ�)
	if (GetIsRestart() == true)
	{
		Start(pPlayer);
	}
	//�׷��� ������ �����·� ���ư���
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}