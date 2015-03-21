#include "stdafx.h"
#include "cPlayerOnHit.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerOnHit::cPlayerOnHit()
{
	m_nCurrentStateType = 11;	//11번 (장)피격
	SetIsRestart(false);
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_PASSIVE);

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;		//피격중 무적상태로 전환할 기능이 있을지도 모르니 강제캔슬
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;			//피격중 사망으로 즉시 전환가능

}

cPlayerOnHit::~cPlayerOnHit()
{
}

void cPlayerOnHit::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	//pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_ONHIT);
}

void cPlayerOnHit::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	//Common을 안쓰고 이 동작의 독자적인 흐름을 설계하려면 시간흐름과 동작종료를 처리해줘야한다.
	//cPlayerCommon::Execute(pPlayer, fDelta);

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
		Exit(pPlayer);
		return;
	}
}

void cPlayerOnHit::Exit(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Exit(pPlayer);

}


//안심 백업 : 민우
//#include "stdafx.h"
//#include "cPlayerOnHit.h"
//#include "cGamePlayableObject.h"
//#include "cSkinnedMesh.h"
//
//cPlayerOnHit::cPlayerOnHit()
//{
//}
//
//
//cPlayerOnHit::~cPlayerOnHit()
//{
//}
//
//void cPlayerOnHit::Start(cGamePlayableObject* pPlayer){
//	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_ONHIT);
//}
//
//void cPlayerOnHit::Execute(cGamePlayableObject* pPlayer, float fDelta){
//	float fWeight = 0.5f; // 애니메이션 빨리 끝나는 속도
//	//float fAniSpeed = 2.0f;// 애니메이션의 속도
//	pPlayer->SetStatePassedTime(fDelta + pPlayer->GetStatePassedTime());
//	
//	D3DXVECTOR3 newPos;
//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
//	// TODO : 차후 넉백거리 조절
//	D3DXVECTOR3 addVec = -(pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed()*0.05f);
//	newPos = curPos + addVec;
//	//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
//	//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
//	pPlayer->SetPosition(newPos);
//
//	if (pPlayer->GetStatePassedTime() > pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() * fWeight){
//		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
//		return;
//	}
//}
//
//void cPlayerOnHit::Exit(cGamePlayableObject* pPlayer){
//
//}
