#pragma once
#include "cEffectParticle.h"

class cEffectTrail : public cEffectParticle
{
public:
	cEffectTrail();
	virtual ~cEffectTrail();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
};

