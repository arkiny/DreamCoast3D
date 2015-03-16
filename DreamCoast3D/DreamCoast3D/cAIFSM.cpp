#include "stdafx.h"
#include "cAIFSM.h"
#include "cGameAIObject.h"
#include "cSkinnedMesh.h"
#include "cAction.h"
#include "cActionMove.h"
#include "cTransform.h"

// ������
void cAIIdle::Start(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
	//pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_IDLE);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(6);
}

void cAIIdle::Execute(cGameAIObject* pAIObject, float fDelta){
	//pAIObject->SetPassedTime(pAIObject->GetPassedTime() + fDelta);

	// Idle 1�ʸ��� ���� �����Ѵ�.
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

// �Ϲ� ������
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

// ���� ������
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

// Ÿ������ �̵�
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

//////����
void cAIAttack::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(3);
	pAIObject->SetPassedTime(0);
	pAIObject->SetAttackCoolTime(0);

	if (pAIObject->GetAItype() == cGameAIObject::E_AI_TYPE::E_AI_BOSS)
	{
		pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPAGE);
	}
	
}

void cAIAttack::Execute(cGameAIObject* pAIObject, float fDelta){
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime()){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
	}
	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIAttack::Exit(cGameAIObject* pAIObject){
	//pAIObject->SetPassedTime(0);
	//pAIObject->SetAttackCoolTime(0);
}

int  cAIAttack::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_ATTACK;
}

void cAIOnHit::Start(cGameAIObject* pAIObject){
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
	//pAIObject->SetPassedTime(0);
}

int  cAIOnHit::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_ONHIT;
}

void cAIThink::Start(cGameAIObject* pAIObject){
	// �Ǵ� �ÿ��� ���� �ִϸ��̼��� �״�� ����Ѵ�.
}

void cAIThink::Execute(cGameAIObject* pAIObject, float fDelta){
	std::vector<cGameObject*> vecInsight;

	if (pAIObject->GetAItype() == pAIObject->E_AI_AGGRESSIVE){
		D3DXVECTOR3 vCenter = pAIObject->GetPosition();
		vecInsight = pAIObject->GetGameObjDeligate()
			->GetInSightObject(ST_BOUNDING_SPHERE(vCenter, 10.0f));
		if (vecInsight.size() <= 1) // ������ �ڱ� �ڽ��� �����ؼ� �����Ѵ�.
		{
			pAIObject->SetTargetObject(NULL);
		}
	}

	// ����...
	// Ÿ���� ������� Ÿ�� ����
	if (pAIObject->GetTargetObject()){ 
		D3DXVECTOR3 vCurPos = pAIObject->GetPosition();
		D3DXVECTOR3 vTargetPos = pAIObject->GetTargetObject()->GetPosition();
		float fAttackRange = pAIObject->GetAttackRange();
		// pAIObject->SetAttackCoolTime(pAIObject->GetAttackCoolTime() + fDelta);
		// pAIObject->GetAttackSphere() // �������� Ÿ�� ���Ǿ�
		// ���� ���� �̵��߿� ���� ��Ÿ� ���� ���� ���
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

	// �þ� ���� ���� �ִٸ�
	if (vecInsight.size() > 1){
		for (auto p : vecInsight){
			pAIObject->AddGameObjToAggroMap(p);
		}
		pAIObject->CheckAggroMapAndSetTarget();
		return;
	}

	//if (pAIObject->GetPrevState()->GetCurrentStateType() == pAIObject->eAISTATE_IDLE 
	//	|| pAIObject->GetTargetObject()==NULL){ // Ÿ���� ������ �׳� ���̵�...
	//	pAIObject->ChangeState(pAIObject->eAISTATE_IDLE);
	//	return;
	//}

	if (pAIObject->GetTargetObject() == NULL){
		pAIObject->ChangeState(pAIObject->eAISTATE_IDLE);
	}

	// �װ͵� �ƴϸ� ���� Ȱ���� ���
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
}

void cAIDead::Exit(cGameAIObject* pAIObject){

}


////
int cAIDead::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_DEAD;
}

void cAIBossPage::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(m_nIndex);
}

void cAIBossPage::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPAGE);
		m_nIndex++;
		if (m_nIndex > 20)
		{
			m_nIndex = 20;
		}
		return;
	}

	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIBossPage::Exit(cGameAIObject* pAIObject)
{

}

int cAIBossPage::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSPAGE;
}


////
void cAIBossPageFirst::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(20);
}

void cAIBossPageFirst::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		pAIObject->ChangeState(pAIObject->eAISTATE_BOSSPAGESECOND);
		return;
	}
	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIBossPageFirst::Exit(cGameAIObject* pAIObject)
{
}

int cAIBossPageFirst::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSPAGEFIRST;
}


////
void cAIBossPageSecond::Start(cGameAIObject* pAIObject)
{
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(24);
}

void cAIBossPageSecond::Execute(cGameAIObject* pAIObject, float fDelta)
{
	if (pAIObject->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pAIObject->GetPassedTime())
	{
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
		return;
	}
	pAIObject->GetGameObjDeligate()->AttackMobToPlayer(pAIObject);
}

void cAIBossPageSecond::Exit(cGameAIObject* pAIObject)
{
}

int cAIBossPageSecond::GetCurrentStateType()
{
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_BOSSPAGESECOND;
}