#include "stdafx.h"
#include "cPlayerCombo1.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame이 이동하는 거리에 따라서 캐릭터의 실제 좌표도 영향을 받아야 되고 카메라도 영향을 받아야 한다.
cPlayerCombo1::cPlayerCombo1()
{
	m_nCurrentStateType = 3;	//3번 (장)Combo1
	m_IsRestart = false;
	//m_IsDoing = false;
	SetIsDoing(false);
	
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_REAR;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1R] = E_CANCEL_CANTCANCEL;
}

cPlayerCombo1::~cPlayerCombo1()
{
}
void cPlayerCombo1::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	//pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_COMBO1);
}
void cPlayerCombo1::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	cPlayerCommon::Execute(pPlayer, fDelta);
	//pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);

	//std::map<std::string, ST_BOUNDING_SPHERE>* pSp = pPlayer->GetAttackSpheres();
	// 파티클을 이용한 트레일 이펙트는 폐기
	/*for (auto p : *pSp){
	pPlayer->GetEffectDelegate()->AddEffect(6, p.second.m_vCenter);
	}*/

	for (auto p : *pPlayer->GetAttackSpheres()){
		pPlayer->GetGameObjDeligate()->isGameAttackSphereCollided(pPlayer, p.second);
	}

	if (g_pControlManager->GetInputInfo('A')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle -= D3DX_PI / 2;
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);


		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed() / 2.f;


		pPlayer->SetPosition(newPos);
	}

	if (g_pControlManager->GetInputInfo('D')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle += D3DX_PI / 2;
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);


		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed() / 2.f;


		pPlayer->SetPosition(newPos);
	}

	D3DXVECTOR3 newPos;
	if (g_pControlManager->GetInputInfo('W')){
		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed());
		newPos = curPos + addVec;
		//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
		//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
		pPlayer->SetPosition(newPos);
		//}
	}

	if (g_pControlManager->GetInputInfo('S')){
		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed() / 2.f);
		newPos = curPos - addVec;
		//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
		//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
		pPlayer->SetPosition(newPos);
		//}
	}

	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime()){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}
void cPlayerCombo1::Exit(cGamePlayableObject* pPlayer)
{
	//cPlayerCommon::Exit(pPlayer);
	//재시작되는 동작이면 재시작하고(반복)
	if (m_IsRestart)
	{
		Start(pPlayer);
	}
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO1R);
	}
}