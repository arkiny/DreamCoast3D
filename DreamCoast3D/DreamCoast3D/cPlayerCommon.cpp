#include "stdafx.h"
#include "cPlayerCommon.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"
#include "cPlayerDead.h"

//지금 핫 스테이트는 대기, 이동, 공격, 피격, 사망

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

	//	// 들어가면 이부분 지워도됨
	//	m_fDeadAccumTime += fDelta;
	//	if (m_fDeadAccumTime >= 2.0f)
	//	{
	//		m_fDeadAccumTime = 0.f;

	//		pPlayer->GetEventDelegate()->DeadScene(true);
	//	}
	//	///////////////////////////////////////////////////////
	//	return;
	//}
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
		//플레이어가 지금 몇번째 평타를 써야하는지 받아옴
		size_t nComboCount = pPlayer->GetComboCount();
		if (nComboCount == 0)	{ pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO1); }
		else if (nComboCount == 1)	{ pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO2); }
		//return;
	}
	
	pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);
	//흘러간 시간이 애니메이션 구간길이를 초과하면 애니메이션 종료. 마지막에 fDelta를 더해주는건 조금 애니메이션이 잘리겠지만 안튀게 하기 위한 임시방편
	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() <= pPlayer->GetStatePassedTime() + DBL_EPSILON + fDelta*2)
	{
		//SetIsDoing(false);
		Exit(pPlayer);		//애니메이션 종료시키기
	}
}
void cPlayerCommon::Exit(cGamePlayableObject* pPlayer)
{
	SetIsDoing(false);
	//재시작되는 동작이면 재시작하고(반복)
	if (GetIsRestart() == true)
	{
		Start(pPlayer);
	}
	//그렇지 않으면 대기상태로 돌아간다
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}