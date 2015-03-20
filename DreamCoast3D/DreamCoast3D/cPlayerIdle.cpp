#include "stdafx.h"
#include "cPlayerIdle.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerIdle::cPlayerIdle()
	//: m_nCurrentStateType(1)
{
	m_nCurrentStateType = 1; //1번 Idle
	SetIsRestart(true);	//상태 재시작 여부
	SetIsDoing(false);

	//m_IsRestart = false;
	//m_IsDoing = false;
	
	/*m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1R] = E_CANCEL_FORCE;*/
	
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;
}


cPlayerIdle::~cPlayerIdle()
{
}

void cPlayerIdle::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	//pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_IDLE);
}

void cPlayerIdle::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer);
	
	cPlayerCommon::Execute(pPlayer, fDelta);
	/*if (g_pControlManager->GetInputInfo('W') || 
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('D'))
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_RUN);
		return;
	}

	if (g_pControlManager->GetInputInfo(VK_LBUTTON)){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO1);
		return;
	}*/

	//if (g_pControlManager->GetInputInfo('Q')){
	//	D3DXMATRIXA16 matR;
	//	float angle = pPlayer->GetPlayerAngle();
	//	angle -= fDelta* 3.0f;
	//	pPlayer->SetPlayerAngle(angle);
	//	D3DXMatrixRotationY(&matR, angle);
	//	D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
	//	D3DXVec3TransformNormal(&vDir, &vDir, &matR);
	//	pPlayer->SetFront(vDir);
	//	pPlayer->SetYangle(angle);
	//}

	//if (g_pControlManager->GetInputInfo('E')){
	//	D3DXMATRIXA16 matR;
	//	float angle = pPlayer->GetPlayerAngle();
	//	angle += fDelta* 3.0f;
	//	pPlayer->SetPlayerAngle(angle);
	//	D3DXMatrixRotationY(&matR, angle);
	//	D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
	//	D3DXVec3TransformNormal(&vDir, &vDir, &matR);
	//	pPlayer->SetFront(vDir);
	//	pPlayer->SetYangle(angle);
	//}

	// do nothing

	// Gravity Force

	D3DXVECTOR3 vFront(0.f, 0.f, 0.f);
	D3DXVECTOR3 vGravity(0.f, 0.f, 0.f);
	D3DXVECTOR3 vCurrentPos(0.f, 0.f, 0.f);

	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	D3DXVECTOR3 addVec = (pPlayer->GetFront());

	D3DXVECTOR3 newPos;
	D3DXVec3Normalize(&addVec, &addVec);
	newPos = curPos + addVec*fDelta*pPlayer->GetMoveSpeed();

	pPlayer->GetGameObjDeligate()->SetCurrentPosition(curPos);
	pPlayer->GetGameObjDeligate()->SetNextPosition(newPos);

	if (pPlayer->GetGameObjDeligate()->CalGradient(pPlayer))
	{
		vGravity.y = -9.8f;
		vFront = -pPlayer->GetFront();
		vGravity = (vGravity + vFront)*g_pTimer->DeltaTime();
	}

	if (pPlayer->GetGameObjDeligate()->CalGradientMinus(pPlayer))
	{
		vGravity.y = -9.8f;
		vFront = pPlayer->GetFront();
		vGravity = (vGravity + vFront)*g_pTimer->DeltaTime();
	}

	vCurrentPos = pPlayer->GetPosition();
	pPlayer->SetPosition(vCurrentPos + vGravity);
}

void cPlayerIdle::Exit(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Exit(pPlayer);
	// currently do nothing
}

//안심 백업 : 민우
//#include "stdafx.h"
//#include "cPlayerIdle.h"
//#include "cGamePlayableObject.h"
//#include "cSkinnedMesh.h"
//
//cPlayerIdle::cPlayerIdle()
//{
//}
//
//
//cPlayerIdle::~cPlayerIdle()
//{
//}
//
//void cPlayerIdle::Start(cGamePlayableObject* pPlayer){
//	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_IDLE);
//	//static int a = 0;
//	//++a;
//}
//
//void cPlayerIdle::Execute(cGamePlayableObject* pPlayer, float fDelta){
//	pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer);
//	if (g_pControlManager->GetInputInfo('W') || 
//		g_pControlManager->GetInputInfo('S') ||
//		g_pControlManager->GetInputInfo('A') ||
//		g_pControlManager->GetInputInfo('D')){
//		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_MOVE);
//		return;
//	}
//
//	if (g_pControlManager->GetInputInfo(VK_LBUTTON)){
//		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_ATTACK);
//		return;
//	}
//
//	if (g_pControlManager->GetInputInfo('Q')){
//		D3DXMATRIXA16 matR;
//		float angle = pPlayer->GetPlayerAngle();
//		angle -= fDelta* 4.0f;
//		pPlayer->SetPlayerAngle(angle);
//		D3DXMatrixRotationY(&matR, angle);
//		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
//		pPlayer->SetFront(vDir);
//		pPlayer->SetYangle(angle);
//	}
//
//	if (g_pControlManager->GetInputInfo('E')){
//		D3DXMATRIXA16 matR;
//		float angle = pPlayer->GetPlayerAngle();
//		angle += fDelta* 4.0f;
//		pPlayer->SetPlayerAngle(angle);
//		D3DXMatrixRotationY(&matR, angle);
//		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
//		pPlayer->SetFront(vDir);
//		pPlayer->SetYangle(angle);
//	}
//
//	// do nothing
//
//	// Gravity Force
//
//	D3DXVECTOR3 vFront(0.f, 0.f, 0.f);
//	D3DXVECTOR3 vGravity(0.f, 0.f, 0.f);
//	D3DXVECTOR3 vCurrentPos(0.f, 0.f, 0.f);
//
//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
//	D3DXVECTOR3 addVec = (pPlayer->GetFront());
//
//	D3DXVECTOR3 newPos;
//	D3DXVec3Normalize(&addVec, &addVec);
//	newPos = curPos + addVec*fDelta*pPlayer->GetMoveSpeed();
//
//	pPlayer->GetGameObjDeligate()->SetCurrentPosition(curPos);
//	pPlayer->GetGameObjDeligate()->SetNextPosition(newPos);
//
//	if (pPlayer->GetGameObjDeligate()->CalGradient(pPlayer))
//	{
//		vGravity.y = -9.8f;
//		vFront = -pPlayer->GetFront();
//		vGravity = (vGravity + vFront)*g_pTimer->DeltaTime();
//	}
//
//	if (pPlayer->GetGameObjDeligate()->CalGradientMinus(pPlayer))
//	{
//		vGravity.y = -9.8f;
//		vFront = pPlayer->GetFront();
//		vGravity = (vGravity + vFront)*g_pTimer->DeltaTime();
//	}
//
//	vCurrentPos = pPlayer->GetPosition();
//	pPlayer->SetPosition(vCurrentPos + vGravity);
//}
//
//void cPlayerIdle::Exit(cGamePlayableObject* pPlayer){
//	// currently do nothing
//}