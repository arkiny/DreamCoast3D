#pragma once

class cEffect;

class cEffectManager : public cObject, public iEffectManagerDelegate
{
protected:
	std::set<cEffect*>		m_setEffects;
	std::vector<cEffect*>	m_vecEffectTobeDeleted;

public:
	cEffectManager();
	virtual ~cEffectManager();

	virtual void Setup();
	virtual void Start();

	virtual void Update(float fDelta);
	virtual void Render();
	
	virtual void Exit();

	virtual void AddEffect(cEffect* pEffect);
	virtual void DeleteEffect(cEffect* pEffect);

	virtual void Destroy();
};

