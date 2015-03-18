#include "stdafx.h"
#include "cEffectPotion.h"


cEffectPotion::cEffectPotion()
{
	m_eEffectType = E_EFFECT_HPPOTION;
	m_vecVertex.resize(50);
	m_vecMoveInfo.resize(50);
}


cEffectPotion::~cEffectPotion()
{

}

void cEffectPotion::Setup(){

	m_fPassedTime = 0;
	D3DXCOLOR c = D3DCOLOR_XRGB(128, 20, 20);

	if (m_eEffectType == E_EFFECT_HPPOTION){
		c = D3DCOLOR_XRGB(128, 20, 20);
	}
	else if (m_eEffectType == E_EFFECT_MPPOTION){
		c = D3DCOLOR_XRGB(30, 80, 128);
	}

	for (int i = 0; i < 50; ++i)
	{
		//ST_PC_VERTEX v;
		
		m_vecVertex[i].c = c;

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
		m_vecVertex[i].p = localPos;

		ST_PARTICLE stP;
		stP._vInitialPos = m_vecVertex[i].p;
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

		//m_vecVertex.push_back(m_vecVertex[i]);
		m_vecMoveInfo[i] = stP;
	}

	m_pTexture = g_pTextureManager->GetTexture("../Resources/Effect/Particle.tga");
}

void cEffectPotion::Start(){

}

void cEffectPotion::Update(float fDelta){
	m_fPassedTime += fDelta;

	for (size_t i = 0; i < m_vecVertex.size(); i++){
		m_vecVertex[i].p = m_vecVertex[i].p + (m_vecMoveInfo[i]._direction * m_vecMoveInfo[i]._speed * fDelta);
		float alpha = 1.0f - (m_vecMoveInfo[i]._currentTime*2.0f / m_vecMoveInfo[i]._lifeSpan);

		if (alpha < 0.0f){
			alpha = 0.0f;
		}

		D3DXCOLOR c = D3DCOLOR_XRGB(128, 20, 20);
		if (m_eEffectType == E_EFFECT_HPPOTION){
			c = D3DCOLOR_XRGB(128, 20, 20);
		}
		else if (m_eEffectType == E_EFFECT_MPPOTION){
			c = D3DCOLOR_XRGB(30, 80, 128);
		}
		m_vecVertex[i].c = D3DXCOLOR(c.r, c.g, c.b, alpha);
		
		m_vecMoveInfo[i]._currentTime += fDelta;
	}

	if (m_fPassedTime > m_fLifeTime){
		this->Exit();
	}
}

void cEffectPotion::Render(){
	// 포인트를 확대 축소 할 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(50.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(100.0f));

	// 포인트에 텍스쳐를 입힐 수 있게 해줌
	// subtraction 어두워지는 것
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 텍스쳐 알파 옵션 설정

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정.


	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 포인트 사이즈 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(10.0f));

	// 알파블렌디 사용 가능하게 설정.
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);


	g_pD3DDevice->SetTexture(0, m_pTexture);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, m_vecVertex.size(), &m_vecVertex[0], sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetFVF(ST_PNT2_VERTEX::FVF);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}