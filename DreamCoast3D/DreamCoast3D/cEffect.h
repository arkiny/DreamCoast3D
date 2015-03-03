#pragma once

class cTransfrom;
class cEffectManager;

class cEffect : public cObject
{
protected:
	cTransform* m_pTransform;
	SYNTHESIZE(cEffectManager*, m_pOwner, Owner);
	//SYNTHESIZE(iEffectManagerDelegate*, m_pEffectManagerDeligate, EffectMAnagerDeligate);

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
};

