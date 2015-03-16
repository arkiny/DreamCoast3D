#pragma once
#include "cEffect.h"
class cEffectMesh	: public cEffect
{
protected:
	LPD3DXMESH			m_pEffectMesh;
	LPDIRECT3DTEXTURE9	m_pTexture;
	float		m_fLifeSpan;

public:
	cEffectMesh();
	virtual ~cEffectMesh();

	virtual void Setup() override;
	virtual void Start() override;
	virtual void Update(float fDelta) override;
	virtual void Render() override;
	virtual void Exit() override;
};

