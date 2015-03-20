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
	//스킬사용
	if (g_pControlManager->GetInputInfo('C'))
	{
		if (pPlayer->GetSkill1CoolTime() >= pPlayer->GetSkill1DelayTime()
			&& pPlayer->GetStatInfo()->fCurrentMana > 10.0f){
			pPlayer->SetSkill1CoolTime(0.0f);
			pPlayer->GetStatInfo()->fCurrentMana -= 10.0f;
			pPlayer->GetGameObjDeligate()->RangeSkill(pPlayer);
		}
	}

	//어떤 동작이든 방향키가 입력되면 RUN동작을 시도한다
	if (g_pControlManager->GetInputInfo('W') || 
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('D'))
	{
		//RUN중이 아닐때만 RUN으로 전환을 시도한다
		if (pPlayer->GetPlayableState() != pPlayer->EPLAYABLESTATE_RUN)
		{
			pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_RUN);
			//return;
		}
	}
	//왼클릭이 눌러지면 평타로 전환을 시도한다
	if (g_pControlManager->GetInputInfo(VK_LBUTTON))
	{
		if (pPlayer->GetPlayableState() != pPlayer->EPLAYABLESTATE_COMBO1)//연타공격 전 임시
		{
			pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO1);
		}
		//return;
	}
	
	pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);
	//흘러간 시간이 애니메이션 구간길이를 초과하면 애니메이션 종료. 마지막에 fDelta를 더해주는건 조금 애니메이션이 잘리겠지만 안튀게 하기 위한 임시방편
	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime() + DBL_EPSILON + fDelta)
	{
		SetIsDoing(false);	//애니메이션 종료 선언
		Exit(pPlayer);		//애니메이션 종료시키기
	}
}
void cPlayerCommon::Exit(cGamePlayableObject* pPlayer)
{
	
	//재시작되는 동작이면 재시작하고(반복)
	if (m_IsRestart)
	{
		Start(pPlayer);
	}
	//그렇지 않으면 대기상태로 돌아간다
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}