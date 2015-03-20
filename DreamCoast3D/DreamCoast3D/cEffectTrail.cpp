#include "stdafx.h"
#include "cEffectTrail.h"


cEffectTrail::cEffectTrail()
{
	m_eEffectType = E_EFFECT_TRAIL;
}


cEffectTrail::~cEffectTrail()
{
}

void cEffectTrail::Setup(){
	m_vecVertex.clear();
	m_vecMoveInfo.clear();
	m_fPassedTime = 0;
	for (int i = 0; i < 1; ++i)
	{
		ST_PC_VERTEX v;
		v.c = D3DCOLOR_XRGB(188, 20, 20);

		D3DXVECTOR3 vDirection(0, 0, 0);
		v.p = D3DXVECTOR3(0, 0, 0); 

		ST_PARTICLE stP;
		stP._vInitialPos = v.p;
		stP._lifeSpan = 5.0f;
		stP._currentTime = 0.0f;
		stP._speed = 0.0f;

		D3DXVECTOR3 vDir;
		stP._direction = vDir * 0.0f;

		m_vecVertex.push_back(v);
		m_vecMoveInfo.push_back(stP);
	}

	m_pTexture = g_pTextureManager->GetTexture("../Resources/Effect/traileffect.png");
}

void cEffectTrail::Start(){

}

void cEffectTrail::Update(float fDelta){
	m_fPassedTime += fDelta;
	for (size_t i = 0; i < m_vecVertex.size(); i++){
		m_vecVertex[i].p = m_vecVertex[i].p + (m_vecMoveInfo[i]._direction * m_vecMoveInfo[i]._speed * fDelta);
		float alpha = 1.0f - (m_vecMoveInfo[i]._currentTime*2.0f / m_vecMoveInfo[i]._lifeSpan);

		if (alpha < 0.0f){
			alpha = 0.0f;
		}
		m_vecVertex[i].c = D3DXCOLOR(180 / 255.0f, 20 / 255.0f, 20 / 255.0f, alpha);
		m_vecMoveInfo[i]._currentTime += fDelta;
	}
	if (m_fPassedTime > m_fLifeTime){
		this->Exit();
	}
}

void cEffectTrail::Render(){
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

	// ���ĺ��� ��� ����.


	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	// ����Ʈ ������ ����
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(2.0f));

	// ���ĺ��� ��� �����ϰ� ����.
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