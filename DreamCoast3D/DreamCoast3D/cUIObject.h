#pragma once

// UI에 쓰일 스프라이트 오브젝트들
class cUIObject : public cObject
{
protected:
	D3DXMATRIXA16			m_matWorld;
	std::vector<cUIObject*> m_vecChild;
	cUIObject*				m_pParent;
	LPD3DXSPRITE			m_pSprite;

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE_PASS_BY_REF(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(int, m_nTag, Tag);

public:
	cUIObject();
	cUIObject(LPD3DXSPRITE pSprite);
	virtual ~cUIObject();


	virtual void AddChild(cUIObject* pChild);
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void Destroy();

	virtual void SetPosition(float x, float y)
	{
		m_vPosition.x = x;
		m_vPosition.y = y;
	}
};

