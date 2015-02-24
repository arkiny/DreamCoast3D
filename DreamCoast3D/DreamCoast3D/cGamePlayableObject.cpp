#include "stdafx.h"
#include "cGamePlayableObject.h"
#include "cPlayerIdle.h"
#include "cPlayerMove.h"
#include "cPlayerAttack.h"
#include "cPlayerOnHit.h"

cGamePlayableObject::cGamePlayableObject()
	:m_vecFront(0, 0, -1),
	m_fMoveSpeed(30.0f),
	m_fPlayerAngleDegree(0.0f)
{
}

cGamePlayableObject::~cGamePlayableObject()
{
	for (auto p : m_vecStates){
		delete p;
	}
}

void cGamePlayableObject::Setup(
	std::string sFolder, std::string sFile,
	std::string sFolderHead, std::string sFileHead,
	std::string sFolderHair, std::string sFileHair
	){
	cGameSMeshBodyObject::Setup(sFolder, sFile, sFolderHead, sFileHead, sFolderHair, sFileHair);

	m_vecStates.resize(EPLAYABLESTATE::EPLAYABLESTATE_MAX);
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = new cPlayerIdle;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_MOVE] = new cPlayerMove;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ATTACK] = new cPlayerAttack;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ONHIT] = new cPlayerOnHit;

	m_pCurrentState = m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE];
	m_pCurrentState->Start(this);
}

void cGamePlayableObject::Update(float fDelta){
	m_pCurrentState->Execute(this, fDelta);
}

void cGamePlayableObject::ChangeState(EPLAYABLESTATE eNewState){
	m_pCurrentState->Exit(this);
	m_pCurrentState = m_vecStates[eNewState];
	m_pCurrentState->Start(this);
}

//�浹�� �ٿ�� ���Ǿ��� ũ��� ��ġ�� ����Ѵ�. : �ο�
void cGamePlayableObject::CalcCollisionSphere(ST_BONE_MESH* pBoneMesh)
{
	D3DXMATRIXA16 matS, matT;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matT);

	//ù �������� �������� OriginRadius��� �ϰ� �̰��� �������� ��� ���̸�ŭ �����ϸ� �Ѵ�.
	float fOffset = pBoneMesh->fRadius / pBoneMesh->fOriginRadius;
	D3DXMatrixScaling(&matS, fOffset, fOffset, fOffset);

	//���Ǿ��� �߽������� ��ȯ
	D3DXMatrixTranslation(&matT,
		pBoneMesh->vCenter.x, pBoneMesh->vCenter.y, pBoneMesh->vCenter.z);

	pBoneMesh->matSphereW = matS*matT;
}
//�浹�� �ٿ�� ���Ǿ �׸���. : �ο�
void cGamePlayableObject::RenderCollisionSphere(ST_BONE_MESH* pBoneMesh)
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBoneMesh->matSphereW);
	pBoneMesh->pSphereMesh->DrawSubset(0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
