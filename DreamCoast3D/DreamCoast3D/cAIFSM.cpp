#include "stdafx.h"
#include "cAIFSM.h"
#include "cGameAIObject.h"
#include "cSkinnedMesh.h"
#include "cAction.h"
#include "cActionMove.h"

void cAIIdle::Start(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_IDLE);
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



void cAIMove::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_MOVE);
	pAIObject->SetPassedTime(0);
}
void cAIMove::Execute(cGameAIObject* pAIObject, float fDelta){
	pAIObject->GetGameObjDeligate()->isGameObjectCollided(pAIObject);
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
	pAIObject->GetGameObjDeligate()->isGameObjectCollided(pAIObject);
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
	cActionMove* pAction = new cActionMove;

	D3DXVECTOR3 curPos = pAIObject->GetPosition();
	
	pAction->SetDelegate(pAIObject);

	D3DXVECTOR3 to = pAIObject->GetTargetObject()->GetPosition();
	float t = D3DXVec3Length(&(curPos - to));
	
	pAction->SetActionTime(t / 10.0f);

	pAction->SetFrom(curPos);
	pAction->SetTo(to);
	pAIObject->SetAction(pAction);
	SAFE_RELEASE(pAction);
}

void cAIMoveToTarget::Execute(cGameAIObject* pAIObject, float fDelta){
	if (pAIObject->GetPassedTime() >= .2f){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
	}
}

void cAIMoveToTarget::Exit(cGameAIObject* pAIObject){

}

int cAIMoveToTarget::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_MOVETOTARGET;
}


void cAIAttack::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_ATTACK);
	pAIObject->SetPassedTime(0);
}

void cAIAttack::Execute(cGameAIObject* pAIObject, float fDelta){

}

void cAIAttack::Exit(cGameAIObject* pAIObject){
	//pAIObject->SetPassedTime(0);
}

int  cAIAttack::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_ATTACK;
}

void cAIOnHit::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(3);
	pAIObject->SetPassedTime(0);
}

void cAIOnHit::Execute(cGameAIObject* pAIObject, float fDelta){
	if (pAIObject->GetPassedTime() > 1.0f){
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
	// 판단 시에는 예전 애니메이션을 그대로 사용한다.
}

void cAIThink::Execute(cGameAIObject* pAIObject, float fDelta){
	std::vector<cGameObject*> vecInsight;

	if (pAIObject->GetAItype() == pAIObject->E_AI_AGGRESSIVE){
		D3DXVECTOR3 vCenter = pAIObject->GetPosition();
		vecInsight = pAIObject->GetGameObjDeligate()->GetInSightObject(ST_BOUNDING_SPHERE(vCenter, 10.0f));
		if (vecInsight.size() > 0){
			int i = 0;
		}
	}

	// 생각...
	if (pAIObject->GetTargetObject()){ // 타겟이 있을경우 타겟 갱신
		pAIObject->ChangeState(pAIObject->eAISTATE_MOVETOTARGET);
		return;
	}
	else if (vecInsight.empty() == false){
		for (auto p : vecInsight){
			pAIObject->AddGameObjToAggroMap(p);
		}
		pAIObject->CheckAggroMapAndSetTarget();
		return;
	}
	else if (pAIObject->GetPrevState()->GetCurrentStateType() == pAIObject->eAISTATE_IDLE 
		&& pAIObject->GetTargetObject()!=NULL){ // 타겟이 없으면 그냥 아이들...
		pAIObject->ChangeState(pAIObject->eAISTATE_IDLE);
		return;
	}
	else { // 그것도 아니면 예전 활동을 계속
		pAIObject->ChangeToPrevState();
		return;
	}
}

void cAIThink::Exit(cGameAIObject* pAIObject){
	//
}

int  cAIThink::GetCurrentStateType(){
	return cGameAIObject::EAIOBJECTSTATE::eAISTATE_THINK;
}