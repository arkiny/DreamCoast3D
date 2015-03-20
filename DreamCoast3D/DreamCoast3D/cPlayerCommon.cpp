#include "stdafx.h"
#include "cPlayerCommon.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerCommon::cPlayerCommon()
{
	//SetIsRestart(false);
}


cPlayerCommon::~cPlayerCommon()
{
}
void cPlayerCommon::Start(cGamePlayableObject* pPlayer)
{
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(m_nCurrentStateType);
	pPlayer->SetStatePassedTime(0.0f);
	//m_IsDoing = true;
	SetIsDoing(true);
}
void cPlayerCommon::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
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
		if (pPlayer->GetPlayableState() != pPlayer->EPLAYABLESTATE_COMBO1)//��Ÿ���� �� �ӽ�
		{
			pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO1);
		}
		//return;
	}
	
	pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);
	//�귯�� �ð��� �ִϸ��̼� �������̸� �ʰ��ϸ� �ִϸ��̼� ����. �������� fDelta�� �����ִ°� ���� �ִϸ��̼��� �߸������� ��Ƣ�� �ϱ� ���� �ӽù���
	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime() + DBL_EPSILON + fDelta)
	{
		SetIsDoing(false);	//�ִϸ��̼� ���� ����
		Exit(pPlayer);		//�ִϸ��̼� �����Ű��
	}
}
void cPlayerCommon::Exit(cGamePlayableObject* pPlayer)
{
	
	//����۵Ǵ� �����̸� ������ϰ�(�ݺ�)
	if (m_IsRestart)
	{
		Start(pPlayer);
	}
	//�׷��� ������ �����·� ���ư���
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}