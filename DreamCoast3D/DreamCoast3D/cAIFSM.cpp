#include "stdafx.h"
#include "cAIFSM.h"
#include "cGameAIObject.h"
#include "cSkinnedMesh.h"
#include "cAction.h"
#include "cActionMove.h"

void cAIIdle::Start(cGameAIObject* pAIObject){
}

void cAIIdle::Execute(cGameAIObject* pAIObject, float fDelta){
	pAIObject->SetPassedTime(pAIObject->GetPassedTime() + fDelta);
	// Idle 1초마다 나는 생각한다.
	if (pAIObject->GetPassedTime() >= 1.0f){
		pAIObject->ChangeState(pAIObject->eAISTATE_THINK);
	}
}

void cAIIdle::Exit(cGameAIObject* pAIObject){
 	pAIObject->SetPassedTime(0);
}

int  cAIIdle::GetCurrentStateType(){
	return 0;
}

void cAIMove::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_MOVE);
}
void cAIMove::Execute(cGameAIObject* pAIObject, float fDelta){
	
}

void cAIMove::Exit(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
}

int  cAIMove::GetCurrentStateType(){
	return 1;
}

void cAIRandomMove::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_MOVE);
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
	
}
void cAIRandomMove::Exit(cGameAIObject* pAIObject){

}
int	 cAIRandomMove::GetCurrentStateType(){
	return 2;
}

void cAIAttack::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_ATTACK);
}
void cAIAttack::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIAttack::Exit(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
}
int  cAIAttack::GetCurrentStateType(){
	return 3;
}

void cAIOnHit::Start(cGameAIObject* pAIObject){
	pAIObject->GetSkinnedMesh()->SetAnimationIndex(pAIObject->eAISTATE_ONHIT);
}
void cAIOnHit::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIOnHit::Exit(cGameAIObject* pAIObject){
	pAIObject->SetPassedTime(0);
}
int  cAIOnHit::GetCurrentStateType(){
	return 4;
}

void cAIThink::Start(cGameAIObject* pAIObject){
	// 판단 시에는 예전 애니메이션을 그대로 사용한다.
}

void cAIThink::Execute(cGameAIObject* pAIObject, float fDelta){
	// 일단은 생각은 하는데, 랜덤 이동을 무조건 실시한다.
	pAIObject->ChangeState(pAIObject->eAISTATE_RANDOMMOVE);
}

void cAIThink::Exit(cGameAIObject* pAIObject){
	
}
int  cAIThink::GetCurrentStateType(){
	return 5;
}