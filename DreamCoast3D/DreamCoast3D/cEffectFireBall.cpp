#include "stdafx.h"
#include "cEffectFireBall.h"
#include "cEffectManager.h"

cEffectFireBall::cEffectFireBall()
{
}


cEffectFireBall::~cEffectFireBall()
{
}

void cEffectFireBall::Setup(){
	for (int i = 0; i < 100; ++i)
	{
		ST_PC_VERTEX v;
		v.c = D3DCOLOR_XRGB(30, 80, 128);
		/*ST_PARTICLE stP;
		stP._vertex.c = D3DCOLOR_XRGB(30, 80, 128);*/
		//v.p = D3DXVECTOR3(0, 0, 100 - 10 * i);
		//v.p = D3DXVECTOR3(0, 0, 10 * i);

		D3DXVECTOR3 vDirection(0, 1, 0);
		D3DXMATRIXA16 rot, rotX, rotY, rotZ;
		D3DXMatrixRotationX(&rotX, D3DXToRadian(rand() % 361));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(rand() % 361));
		D3DXMatrixRotationZ(&rotZ, D3DXToRadian(rand() % 361));
		rot = rotX*rotY*rotZ;
		D3DXMATRIXA16 localMat;
		localMat = rot;
		D3DXVECTOR3 localPos;
		D3DXVec3TransformCoord(&localPos, &vDirection, &localMat);
		localPos = localPos * ((rand() % 200) / 2000.0f);
		v.p = localPos;

		ST_PARTICLE stP;
		stP._vInitialPos = v.p;
		stP._lifeSpan = (rand() % 400) / 200.0f;
		stP._currentTime = 0.0f;
		stP._speed = (rand() % 200) / 200.0f;

		D3DXMatrixRotationX(&rotX, D3DXToRadian(rand() % 361));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(rand() % 361));
		D3DXMatrixRotationZ(&rotZ, D3DXToRadian(rand() % 361));

		rot = rotX*rotY*rotZ;
		localMat = rot;
		D3DXVECTOR3 vDir;
		D3DXVec3TransformNormal(&vDir, &vDirection, &localMat);
		stP._direction = vDir;

		m_vecVertex.push_back(v);
		m_vecMoveInfo.push_back(stP);
	}
	SetPosition(D3DXVECTOR3(128.0f, 10.0f, 128.0f));
}

void cEffectFireBall::Start(){
}

void cEffectFireBall::Update(float fDelta){
	/*if (GetKeyState(VK_SPACE) & 0x8000){*/
	m_fPassedTime += fDelta;
	for (size_t i = 0; i < m_vecVertex.size(); i++){
		m_vecVertex[i].p = m_vecVertex[i].p + (m_vecMoveInfo[i]._direction * m_vecMoveInfo[i]._speed * fDelta);
		float alpha = 1.0f - (m_vecMoveInfo[i]._currentTime*2.0f / m_vecMoveInfo[i]._lifeSpan);

		if (alpha < 0.0f){
			alpha = 0.0f;
		}
		m_vecVertex[i].c = D3DXCOLOR(128 / 255.0f, 20 / 255.0f, 20 / 255.0f, alpha);
		m_vecMoveInfo[i]._currentTime += fDelta;
	}
	//}
	/*else{
		for (size_t i = 0; i < m_vecVertex.size(); i++){
			m_vecVertex[i].p = m_vecVertex[i].p + (m_vecMoveInfo[i]._direction * m_vecMoveInfo[i]._speed * fDelta);

			float alpha = 1.0f - (m_vecMoveInfo[i]._currentTime / m_vecMoveInfo[i]._lifeSpan);

			if (alpha < 0.0f){
				alpha = 0.0f;
			}
			m_vecVertex[i].c = D3DXCOLOR(128 / 255.0f, 20 / 255.0f, 20 / 255.0f, alpha);
			m_vecMoveInfo[i]._currentTime += fDelta;

			if (m_vecMoveInfo[i]._currentTime > m_vecMoveInfo[i]._lifeSpan){
				m_vecMoveInfo[i]._currentTime = 0.0f;
				m_vecVertex[i].p = m_vecMoveInfo[i]._vInitialPos;

				m_vecMoveInfo[i]._lifeSpan = (rand() % 400) / 200.0f;
				m_vecMoveInfo[i]._currentTime = 0.0f;
				m_vecMoveInfo[i]._speed = (rand() % 200) / 200.0f;
				D3DXMATRIXA16 rot, rotX, rotY, rotZ;
				D3DXMatrixRotationX(&rotX, D3DXToRadian(rand() % 361));
				D3DXMatrixRotationY(&rotY, D3DXToRadian(rand() % 361));
				D3DXMatrixRotationZ(&rotZ, D3DXToRadian(rand() % 361));

				rot = rotX*rotY*rotZ;

				D3DXVECTOR3 vDirection(0, 1, 0);
				D3DXMATRIXA16 localMat;
				localMat = rot;
				D3DXVECTOR3 vDir;
				D3DXVec3TransformNormal(&vDir, &vDirection, &localMat);
				m_vecMoveInfo[i]._direction = vDir;
			}
		}
	}*/

	if (m_fPassedTime > m_fLifeTime){
		this->Exit();
	}
}

void cEffectFireBall::Render(){
	// ����Ʈ�� Ȯ�� ��� �� �� �ְ� ����
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// ����Ʈ �����ϸ� Factor�� ����
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(50.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(100.0f));

	// ����Ʈ�� �ؽ��ĸ� ���� �� �ְ� ����
	// subtraction ��ο����� ��
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// �ؽ��� ���� �ɼ� ����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// ���ĺ����� ��� ����.
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	// ����Ʈ ������ ����
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(10.0f));

	// ���ĺ����� ��� �����ϰ� ����.
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture("../Resources/Effect/Particle.tga");
	g_pD3DDevice->SetTexture(0, pTexture);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, m_vecVertex.size(), &m_vecVertex[0], sizeof(ST_PC_VERTEX));
	
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}