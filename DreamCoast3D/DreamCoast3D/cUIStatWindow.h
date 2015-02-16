#pragma once

class cObjectPlayerable;
class cUIImageView;

class cUIStatWindow : public cUIObject
{
private:
	RECT					m_rectClickArea;
	LPD3DXSPRITE			m_pSprite;
	cUIObject*				m_pUIRoot;

	cUIImageView*			m_pHealth;
	cUIImageView*			m_pMana;

	// TODO : 차후 포인터로 타겟을 설정할 예정
	float					m_fMaxHealth;
	float					m_fCurrentHealth;

	float					m_fMaxMana;
	float					m_fCurrentMana;

	float					m_fMaxExp;
	float					m_fCurrentExp;

	float*					m_fTargetMaxHealth;
	float*					m_fTargetCurrentHealth;
							
	float*					m_fTargetMaxMana;
	float*					m_fTargetCurrentMana;

public:
	cUIStatWindow();
	virtual ~cUIStatWindow();

	virtual void Setup();
	
	// TODO : 차후 타겟을 정할때 쓸 함수
	virtual void SetTarget(cObjectPlayerable* pPlayer);

	virtual void Update(float fDelta);
	virtual void Render();
};

