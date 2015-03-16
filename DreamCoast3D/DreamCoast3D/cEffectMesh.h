#pragma once
#include "cEffect.h"

class cMesh;
class cEffectMesh	: public cEffect
{
protected:
	cMesh*			m_pEffectMesh;
	float			m_fLifeSpan;

public:
	cEffectMesh();
	virtual ~cEffectMesh();

	virtual void Setup() override;
	virtual void Start() override;
	virtual void Update(float fDelta) override;
	virtual void Render() override;
	virtual void Exit() override;
};

