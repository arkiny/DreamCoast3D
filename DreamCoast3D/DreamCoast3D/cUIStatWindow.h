#pragma once

class cObjectPlayerable;
class cUIImageView;

class cUIStatWindow : public cUIWindow
{
protected:
	cUIImageView*			m_pHealth;
	cUIImageView*			m_pMana;

	// TODO : ���� �����ͷ� Ÿ���� ������ ����
	float					m_fMaxHealth;
	float					m_fCurrentHealth;

	float					m_fMaxMana;
	float					m_fCurrentMana;

	float*					m_fTargetMaxHealth;
	float*					m_fTargetCurrentHealth;
							
	float*					m_fTargetMaxMana;
	float*					m_fTargetCurrentMana;

public:
	cUIStatWindow();
	virtual ~cUIStatWindow();

	virtual void Setup();
	
	// TODO : ���� Ÿ���� ���Ҷ� �� �Լ�
	virtual void SetTarget(cObjectPlayerable* pPlayer);

	virtual void Update(float fDelta);
	virtual void Render();
};
