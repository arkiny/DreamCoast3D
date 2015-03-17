#include "stdafx.h"
#include "cAIFSM.h"
#include "cGameAIObject.h"
#include "cSkinnedMesh.h"
#include "cAction.h"
#include "cActionMove.h"
#include "cTransform.h"

// 대기상태
void cAIIdle::Start(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
	//pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_IDLE);

	if (pAIObject->GetAItype() == cGameAIObject::E_AI_TYPE::E_AI_BOSS)
	{
		pAIObject->GetSkinnedMesh()->SetAnimationIndex(6);
	}
	else
	{
		pAIObject->GetSkinnedMesh()->SetAnimationIndex(0);
	}

}

void cAIIdle::Execute(cGameAIObject* pAIObject, float fDelta){
	//pAIObject->SetPassedTime(pAIObject->GetPassedTime() + fDelta);

	// Idle 1초마다 나는 생각한다.
	if (pAIObject->GetPassedTime() >= 1.0f){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
	}

	return;
}

void cAIIdle::Exit(cGameAIObject* pAIObject){
 	//pAIObject->SetPassedTime(0);
}

int  cAIIdle::GetCurrentStateType(){
	return cGameAIObject::eAISTATE_IDLE;
}

// 일반 움직임
void cAIMove::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_MOVE);
	pAIObject->SetPassedTime(0);
}
void cAIMove::Execute(cGameAIObject* pAIObject, float fDelta){
	//pAIObject->GetGameObjDeligate()->isGameObjectCollided(pAIObject);
	if (pAIObject->GetPassedTime() >= 1.0f){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
	}
}
void cAIMove::Exit(cGameAIObject* pAIObject){
	//pAIObject->SetPassedTime(0);
}
int  cAIMove::GetCurrentStateType(){
	return cGameAIObject::eAISTATE_MOVE;
}

// 랜덤 움직임
void cAIRandomMove::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_MOVE);
	pAIObject->SetPassedTime(0);
	
	cActionMove* pAction = new cActionMove;
	D3DXVECTOR3 curPos = pAIObject->GetPosition();
	pAction->SetFrom(curPos);
	pAction->SetDelegate(pAIObject);


	float x = -1;
	do {
		x = (rand() % 50) / 10.0f;
	} while (x > 256 && x < 0);

	float z = -1;
	do {
		z = (rand() % 50) / 10.0f;
	} while (z > 256 && z < 0);

	int xminus = rand() % 2;
	int zminus = rand() % 2;
	if (xminus){
		x *= -1.0f;
	}
	if (zminus){
		z *= -1.0f;
	}

	x += curPos.x;
	z += curPos.z;
	D3DXVECTOR3 to = D3DXVECTOR3(x, curPos.y, z);

	float t = D3DXVec3Length(&(curPos - to));
	pAction->SetActionTime(t/10.0f);
	pAction->SetTo(to);
	pAIObject->SetAction(pAction);
	SAFE_RELEASE(pAction);
}


void cAIRandomMove::Execute(cGameAIObject* pAIObject, float fDelta){
	//pAIObject->GetGameObjDeligate()->isGameObjectCollided(pAIObject);
	if (pAIObject->GetPassedTime() >= 1.0f){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
	}
}

void cAIRandomMove::Exit(cGameAIObject* pAIObject){
}

int	 cAIRandomMove::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_RANDOMMOVE;
}

void cAIMoveToTarget::Start(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_MOVE);
}

// 타겟으로 이동
void cAIMoveToTarget::Execute(cGameAIObject* pAIObject, float fDelta){
	
	if (pAIObject->GetPassedTime() >= .2f){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
	}

	D3DXVECTOR3 curPos = pAIObject->GetPosition();
	D3DXVECTOR3 addVec;

	if (pAIObject->GetTargetObject()){
		D3DXVECTOR3 targetPos = pAIObject->GetTargetObject()->GetPosition();
		//addVec = targetPos - curPos;
		D3DXVECTOR3 vDeltaPos = targetPos - curPos;

		D3DXVec3Normalize(&vDeltaPos, &vDeltaPos);

		pAIObject->SetFront(vDeltaPos);

		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);
		addVec = vDeltaPos;

		D3DXVec3Normalize(&addVec, &addVec);
		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
		vForce = pAIObject->GetGameObjDeligate()->isCollidedStaticObject(pAIObject) + addVec;
		D3DXVec3Normalize(&vForce, &vForce);

		newPos = curPos + (vForce*fDelta* pAIObject->GetMoveSpeed());
		pAIObject->SetPosition(newPos);

		if (fabs(vDeltaPos.x) > 0.0001f)
		{
			float fAngle;
			fAngle = -atan2(vDeltaPos.z, vDeltaPos.x) - D3DXToRadian(-270.0f);
			pAIObject->SetAIAngle(fAngle);
			pAIObject->GetTransform()->SetYAxisAngle(fAngle);
		}
		else{
			if (vDeltaPos.z > 0.0f){
				pAIObject->SetAIAngle(D3DXToRadian(-180.0f));
				pAIObject->GetTransform()->SetYAxisAngle(D3DXToRadian(-180.0f));
			}
			else {
				pAIObject->SetAIAngle(D3DXToRadian(0.0f));
				pAIObject->GetTransform()->SetYAxisAngle(D3DXToRadian(0.0f));
			}
		}

	}
}

void cAIMoveToTarget::Exit(cGameAIObject* pAIObject){

}

int cAIMoveToTarget::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_MOVETOTARGET;
}

//////공격
void cAIAttack::Start(cGameAIObject* pAIObject){
	if (pAIObject->GetAItype() == cGameAIObject::E_AI_TYPE::E_AI_BOSS)
	{
		if (pAIObject->GetHP() > 600)
		{
			pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPHASEFIRST);
		}
		else if (pAIObject->GetHP() > 300)
		{
			pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPHASESECOND);
		}
		else if (pAIObject->GetHP() > 100)
		{
			pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPHASETHIRD);
		}
		else if (pAIObject->GetHP() >= 0.f)
		{
			pAIObject->ChangeState(pAIObject->eAISTATE_BOSSDEAD);
		}

		return;
	}
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(3);
	pAIObject->SetPassedTime(0);
	pAIObject->SetAttackCoolTime(0);
}

void cAIAttack::Execute(cGameAIObject* pAIObject, float fDelta){

	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime()){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
	}
	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIAttack::Exit(cGameAIObject* pAIObject){
}

int  cAIAttack::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_ATTACK;
}

void cAIOnHit::Start(cGameAIObject* pAIObject){
	if (pAIObject->GetAItype() == cGameAIObject::E_AI_TYPE::E_AI_BOSS)
	{
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
	}

	pAIObject->GetSkinnedMesh()->SetAnimationIndex(3);
	pAIObject->SetPassedTime(0);
}

void cAIOnHit::Execute(cGameAIObject* pAIObject, float fDelta){
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime()){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
	}
}
void cAIOnHit::Exit(cGameAIObject* pAIObject){

}

int  cAIOnHit::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_ONHIT;
}

void cAIThink::Start(cGameAIObject* pAIObject){
	// 판단 시에는 예전 애니메이션을 그대로 사용한다.
}

void cAIThink::Execute(cGameAIObject* pAIObject, float fDelta){
	std::vector<cGameObject*> vecInsight;

	if (pAIObject->GetAItype() == pAIObject->E_AI_AGGRESSIVE){
		D3DXVECTOR3 vCenter = pAIObject->GetPosition();
		vecInsight = pAIObject->GetGameObjDeligate()
			->GetInSightObject(ST_BOUNDING_SPHERE(vCenter, 10.0f));
		if (pAIObject->GetAItype() == 2)
		{
			vecInsight = pAIObject->GetGameObjDeligate()
				->GetInSightObject(ST_BOUNDING_SPHERE(vCenter, 50.0f));
		}
		if (vecInsight.size() <= 1) // 보통은 자기 자신을 포함해서 리턴한다.
		{
			pAIObject->SetTargetObject(NULL);
		}
	}

	// 생각...
	// 타겟이 있을경우 타겟 갱신
	if (pAIObject->GetTargetObject()){ 
		D3DXVECTOR3 vCurPos = pAIObject->GetPosition();
		D3DXVECTOR3 vTargetPos = pAIObject->GetTargetObject()->GetPosition();
		float fAttackRange = pAIObject->GetAttackRange();
		if (pAIObject->GetAItype() == 2)
		{
			fAttackRange = 3.f;
		}
		// pAIObject->SetAttackCoolTime(pAIObject->GetAttackCoolTime() + fDelta);
		// pAIObject->GetAttackSphere() // 실질적인 타격 스피어
		// 적을 향해 이동중에 적이 사거리 내에 있을 경우
		float fDist = D3DXVec3Length(&(vTargetPos - vCurPos));
		if (fAttackRange > fDist){

			if (pAIObject->GetAttackCoolTime() > pAIObject->GetAttackSpeed()){
				pAIObject->ChangeState(pAIObject->eAISTATE_ATTACK);
				return;
			}
			else{
				//pAIObject->ChangeState(pAIObject->eAISTATE_MOVETOTARGET);
				return;
			}
		}
		else {
			pAIObject->ChangeState(pAIObject->eAISTATE_MOVETOTARGET);
			return;
		}		
	}

	// 시야 내에 적이 있다면
	if (vecInsight.size() > 1){
		for (auto p : vecInsight){
			pAIObject->AddGameObjToAggroMap(p);
		}
		pAIObject->CheckAggroMapAndSetTarget();
		return;
	}

	//if (pAIObject->GetPrevState()->GetCurrentStateType() == pAIObject->eAISTATE_IDLE 
	//	|| pAIObject->GetTargetObject()==NULL){ // 타겟이 없으면 그냥 아이들...
	//	pAIObject->ChangeState(pAIObject->eAISTATE_IDLE);
	//	return;
	//}

	if (pAIObject->GetTargetObject() == NULL){
		pAIObject->ChangeState(pAIObject->eAISTATE_IDLE);
	}

	// 그것도 아니면 예전 활동을 계속
	pAIObject->ChangeToPrevState();
	return;
}

void cAIThink::Exit(cGameAIObject* pAIObject){
	//
}

int  cAIThink::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_THINK;
}

void cAIDead::Start(cGameAIObject* pAIObject){

}

void cAIDead::Execute(cGameAIObject* pAIObject, float fDelta){
    pAIObject->GetGameObjDeligate()->EraseFromGameObjectSet(pAIObject);
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		//pAIObject->ChangeState(pAIObject->eAISTATE_DEAD);
		return;
	}

}

void cAIDead::Exit(cGameAIObject* pAIObject){

}


////
int cAIDead::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_DEAD;
}

void cAIBossPhaseFirst::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(m_nIndex);

}

void cAIBossPhaseFirst::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() - 0.05f
		< pAIObject->GetPassedTime())
	{
		m_nEpsilon = 1;
	}

	if (m_nEpsilon == 1)
	{
		D3DXVECTOR3 vCenter(0.f, 0.f, 0.f);
		vCenter = pAIObject->GetUpdatedDetailedSphere()->at("FxBottom").m_vCenter;
		pAIObject->SetPosition(vCenter);
		m_nEpsilon = 2;
	}
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		m_nEpsilon = 0;
		if (m_nIndex == 18)
		{
			m_nIndex = 3;
		}
		else if (m_nIndex == 3)
		{
			m_nIndex = 18;
		}
		pAIObject->SetHP(pAIObject->GetHP() - 100);
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
		
	}
	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIBossPhaseFirst::Exit(cGameAIObject* pAIObject)
{

}

int cAIBossPhaseFirst::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSPHASEFIRST;
}


////
void cAIBossPhaseSecond::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(m_nIndex);
}

void cAIBossPhaseSecond::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() - 0.05f
		< pAIObject->GetPassedTime() && m_nEpsilon == 0)
	{
		m_nEpsilon = 1;
	}

	if (m_nEpsilon == 1)
	{
		D3DXVECTOR3 vCenter(0.f, 0.f, 0.f);
		vCenter = pAIObject->GetUpdatedDetailedSphere()->at("FxBottom").m_vCenter;
		pAIObject->SetPosition(vCenter);
		m_nEpsilon = 2;
	}

	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		pAIObject->SetHP(pAIObject->GetHP() - 100);
		m_nEpsilon = 0;
		if (m_nIndex == 3)
		{
			m_nIndex = 23;
		}
		else if (m_nIndex == 23)
		{
			m_nIndex = 21;
		}
		else if (m_nIndex == 21)
		{
			m_nIndex = 3;
		}
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);

		return;
	}
	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIBossPhaseSecond::Exit(cGameAIObject* pAIObject)
{
}

int cAIBossPhaseSecond::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSPHASESECOND;
}


////
void cAIBossPhaseThird::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(m_nIndex);
}

void cAIBossPhaseThird::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() - 0.05f
		< pAIObject->GetPassedTime() && m_nEpsilon == 0)
	{
		m_nEpsilon = 1;
	}

	if (m_nEpsilon == 1)
	{
		//// FxBottom FxCenter FxTop
		D3DXVECTOR3 vCenter(0.f, 0.f, 0.f);
		vCenter = pAIObject->GetUpdatedDetailedSphere()->at("FxBottom").m_vCenter;
		pAIObject->SetPosition(vCenter);
		m_nEpsilon = 2;
	}

	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		m_nEpsilon = 0;
		if (pAIObject->GetHP() > 100)
		{
			pAIObject->SetHP(pAIObject->GetHP() - 30);
		}
		if (m_nIndex == 3)
		{
			m_nIndex = 18;
			pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		}
		else if (m_nIndex == 18)
		{
			m_nIndex = 22;
			pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		}
		else if (m_nIndex == 22)
		{
			m_nIndex = 16;
			pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		}
		else if (m_nIndex == 16)
		{
			m_nIndex = 24;
			pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPHASETHIRD);
		}
		else if (m_nIndex == 24)
		{
			m_nIndex = 3;
			pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		}

		return;
	}

	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIBossPhaseThird::Exit(cGameAIObject* pAIObject)
{
}

int cAIBossPhaseThird::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSPHASETHIRD;
}

void cAIBossDead::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(m_nIndex);
}

void cAIBossDead::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() -0.1f<
		pAIObject->GetPassedTime())
	{
		pAIObject->ChangeState(pAIObject->eAISTATE_DEAD);
		return;
	}
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		if (m_nIndex == 5)
		{
			pAIObject->SetHP(0);
			m_nIndex = 6;
			pAIObject->ChangeState(pAIObject->eAISTATE_BOSSDEAD);
		}
		return;
	}


}

void cAIBossDead::Exit(cGameAIObject* pAIObject)
{
}

int cAIBossDead::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSDEAD;
}