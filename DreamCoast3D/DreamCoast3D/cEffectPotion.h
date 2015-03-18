#pragma once
#include "cEffectParticle.h"
class cEffectPotion : public cEffectParticle
{
public:
	cEffectPotion();
	virtual ~cEffectPotion();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
};

