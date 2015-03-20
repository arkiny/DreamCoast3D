#include "stdafx.h"
//#include "cPlayerMove.h"
//#include "cGamePlayableObject.h"
//#include "cSkinnedMesh.h"
//
//cPlayerMove::cPlayerMove()
//{
//
//}
//
//
//cPlayerMove::~cPlayerMove()
//{
//}
//
//void cPlayerMove::Start(cGamePlayableObject* pPlayer){
//	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_MOVE);
//}
//
//void cPlayerMove::Execute(cGamePlayableObject* pPlayer, float fDelta){
//
//	if (g_pControlManager->GetInputInfo('C'))
//	{
//		if (pPlayer->GetSkill1CoolTime() >= pPlayer->GetSkill1DelayTime()
//			&& pPlayer->GetStatInfo()->fCurrentMana > 10.0f){
//			pPlayer->SetSkill1CoolTime(0.0f);
//			pPlayer->GetStatInfo()->fCurrentMana -= 10.0f;
//			pPlayer->GetGameObjDeligate()->RangeSkill(pPlayer);
//		}
//	}
//
//	if (g_pControlManager->GetInputInfo('Q')){
//		D3DXMATRIXA16 matR;
//		float angle = pPlayer->GetPlayerAngle();
//		angle -= fDelta* 2.0f;
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
//		angle += fDelta* 2.0f;
//		pPlayer->SetPlayerAngle(angle);
//		D3DXMatrixRotationY(&matR, angle);
//		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
//		pPlayer->SetFront(vDir);
//		pPlayer->SetYangle(angle);
//	}
//
//	if (g_pControlManager->GetInputInfo('W') ||
//		g_pControlManager->GetInputInfo('A') ||
//		g_pControlManager->GetInputInfo('S') ||
//		g_pControlManager->GetInputInfo('D')){
//
//		MoveFront(pPlayer, fDelta);
//		MoveBack(pPlayer, fDelta);
//		MoveRight(pPlayer, fDelta);
//		MoveLeft(pPlayer, fDelta);
//
//		if (g_pControlManager->GetInputInfo(VK_LBUTTON)){
//			pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_ATTACK);
//		}
//
//		return;
//	}
//
//	pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
//}
//
//void cPlayerMove::Exit(cGamePlayableObject* pPlayer){
//
//}
//
//void cPlayerMove::MoveFront(cGamePlayableObject* pPlayer, float fDelta)
//{
//	D3DXVECTOR3 newPos;
//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
//	D3DXVECTOR3 addVec = (pPlayer->GetFront());
//
//	if (g_pControlManager->GetInputInfo('W')){
//		D3DXVec3Normalize(&addVec, &addVec);
//
//		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
//
//		vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
//		D3DXVec3Normalize(&vForce, &vForce);
//
//		if (g_pControlManager->GetInputInfo('A') || g_pControlManager->GetInputInfo('D'))
//		{
//			pPlayer->SetMoveSpeed(0.f);
//		}
//		else
//		{
//			pPlayer->SetMoveSpeed(10.f);
//		}
//
//		newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed();
//
//		pPlayer->GetGameObjDeligate()->SetNextPosition(newPos);
//		pPlayer->GetGameObjDeligate()->SetCurrentPosition(curPos);
//
//		if (pPlayer->GetGameObjDeligate()->CalGradient(pPlayer))
//		{
//			newPos = curPos;
//		}
//
//		D3DXVECTOR3 vGravity(0.f, 0.f, 0.f);
//		vGravity = pPlayer->GetGameObjDeligate()->GravityForce() + addVec;
//
//		newPos += vGravity*fDelta;
//
//		pPlayer->SetPosition(newPos);
//		pPlayer->GetEventDelegate()->CheckEventFromRange(pPlayer, 1);
//	}
//
//}
//
//void cPlayerMove::MoveBack(cGamePlayableObject* pPlayer, float fDelta)
//{
//
//	D3DXVECTOR3 newPos;
//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
//	D3DXVECTOR3 addVec = (pPlayer->GetFront());
//
//	if (g_pControlManager->GetInputInfo('S')){
//
//		pPlayer->SetMoveSpeed(5.f);
//
//		D3DXVECTOR3 curPos = pPlayer->GetPosition();
//		D3DXVECTOR3 addVec = -(pPlayer->GetFront());
//
//		D3DXVec3Normalize(&addVec, &addVec);
//		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
//		vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
//		D3DXVec3Normalize(&vForce, &vForce);
//		newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed() / 2.f;
//
//		pPlayer->GetGameObjDeligate()->SetNextPosition(newPos);
//		pPlayer->GetGameObjDeligate()->SetCurrentPosition(curPos);
//
//		if (pPlayer->GetGameObjDeligate()->CalGradient(pPlayer))
//		{
//			newPos = curPos;
//		}
//
//		D3DXVECTOR3 vGravity(0.f, 0.f, 0.f);
//		vGravity = pPlayer->GetGameObjDeligate()->GravityForce() + addVec;
//
//		newPos += vGravity*fDelta;
//
//		pPlayer->SetPosition(newPos);
//	}
//	else
//	{
//		pPlayer->SetMoveSpeed(10.f);
//	}
//}
//
//void cPlayerMove::MoveRight(cGamePlayableObject* pPlayer, float fDelta)
//{
//	D3DXVECTOR3 newPos;
//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
//	D3DXVECTOR3 addVec = (pPlayer->GetFront());
//
//	if (g_pControlManager->GetInputInfo('D')){
//		D3DXMATRIXA16 matR;
//		float angle = pPlayer->GetPlayerAngle();
//		angle += D3DX_PI / 2;
//		D3DXMatrixRotationY(&matR, angle);
//		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
//
//		D3DXVECTOR3 curPos = pPlayer->GetPosition();
//		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);
//
//		pPlayer->SetYangle(angle);
//
//		if (g_pControlManager->GetInputInfo('S'))
//		{
//			pPlayer->SetYangle(angle - D3DX_PI / 4 - D3DX_PI / 2);
//		}
//		else
//		{
//			pPlayer->SetYangle(angle - D3DX_PI / 4);
//		}
//
//		D3DXVec3Normalize(&addVec, &addVec);
//		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
//		vForce = (pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer))*1.5f + addVec;
//
//		vDir = vDir + vForce;
//		D3DXVec3Normalize(&vDir, &vDir);
//
//		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed();
//
//		pPlayer->SetPosition(newPos);
//	}
//
//}
//
//void cPlayerMove::MoveLeft(cGamePlayableObject* pPlayer, float fDelta)
//{
//	D3DXVECTOR3 newPos;
//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
//	D3DXVECTOR3 addVec = (pPlayer->GetFront());
//
//	if (g_pControlManager->GetInputInfo('A')){
//		D3DXMATRIXA16 matR;
//		float angle = pPlayer->GetPlayerAngle();
//		angle -= D3DX_PI / 2;
//		D3DXMatrixRotationY(&matR, angle);
//		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
//
//		D3DXVECTOR3 curPos = pPlayer->GetPosition();
//		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);
//
//		if (g_pControlManager->GetInputInfo('S'))
//		{
//			pPlayer->SetYangle(angle + D3DX_PI / 4 + D3DX_PI / 2);
//		}
//		else
//		{
//			pPlayer->SetYangle(angle + D3DX_PI / 4);
//		}
//		D3DXVec3Normalize(&addVec, &addVec);
//		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
//		vForce = (pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer))*1.5f + addVec;
//
//		vDir = vDir + vForce;
//		D3DXVec3Normalize(&vDir, &vDir);
//
//		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed();
//
//		pPlayer->SetPosition(newPos);
//	}
//}