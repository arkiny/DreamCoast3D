#pragma once

class cTransfrom;
class cEffectManager;

class cEffect : public cObject
{
public:
	enum E_EFFECT_TYPE {
		E_EFFECT_NONE,
		E_EFFECT_ONHIT,
		E_EFFECT_GETHIT,
		E_EFFECT_TYPE_MAX
	};

protected:
	cTransform* m_pTransform;
	SYNTHESIZE(cEffectManager*, m_pOwner, Owner);
	//SYNTHESIZE(iEffectManagerDelegate*, m_pEffectManagerDeligate, EffectMAnagerDeligate);
	E_EFFECT_TYPE m_eEffectType;
public:
	cEffect();
	virtual ~cEffect();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void Exit();

	virtual cTransform*			GetTransform();
	virtual D3DXMATRIXA16*		GetTransformMatrix();
	virtual void SetPosition(D3DXVECTOR3 vPos);
	virtual E_EFFECT_TYPE GetEffectType() { return m_eEffectType; }
};

