#pragma once

class cEffect;

class cEffectManager : public cObject, public iEffectManagerDelegate
{
protected:
	std::set<cEffect*>		m_setEffects;

	std::vector<cEffect*>	m_vecEffectTobeDeleted;

	std::queue<cEffect*>	m_qeueuHPPotionEffectPool;
	std::queue<cEffect*>	m_qeueuOnHitEffectPool;

	std::queue<cEffect*>	m_qeueuSkill1EffectPool;

	std::queue<cEffect*>	m_qeueuOnGetHitEffectPool;

	std::queue<cEffect*>	m_qeueuTrailEffectPool;

public:
	cEffectManager();
	virtual ~cEffectManager();

	virtual void Setup();
	virtual void Start();

	virtual void Update(float fDelta);
	virtual void Render();
	
	virtual void Exit();

	virtual void AddEffect(UINT eType, D3DXVECTOR3 vPos) override;
	virtual void DeleteEffect(cEffect* pEffect);

	virtual void Destroy();
};

