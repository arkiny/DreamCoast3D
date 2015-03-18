#pragma once
#include "cEffectParticle.h"

class cEffectSkill1 : public cEffectParticle
{

public:
	cEffectSkill1();
	virtual	~cEffectSkill1();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
};

