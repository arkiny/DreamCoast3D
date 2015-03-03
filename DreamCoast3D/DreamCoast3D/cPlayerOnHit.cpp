#include "stdafx.h"
#include "cPlayerOnHit.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerOnHit::cPlayerOnHit()
{
}


cPlayerOnHit::~cPlayerOnHit()
{
}

void cPlayerOnHit::Start(cGamePlayableObject* pPlayer){
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_ONHIT);
}

void cPlayerOnHit::Execute(cGamePlayableObject* pPlayer, float fDelta){
	float fWeight = 0.5f; // 애니메이션 빨리 끝나는 속도
	//float fAniSpeed = 2.0f;// 애니메이션의 속도
	pPlayer->SetStatePassedTime(fDelta + pPlayer->GetStatePassedTime());
	
	D3DXVECTOR3 newPos;
	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	// TODO : 차후 넉백거리 조절
	D3DXVECTOR3 addVec = -(pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed()*0.05f);
	newPos = curPos + addVec;
	//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
	//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
	pPlayer->SetPosition(newPos);

	if (pPlayer->GetStatePassedTime() > pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() * fWeight){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
		return;
	}
}

void cPlayerOnHit::Exit(cGamePlayableObject* pPlayer){

}
