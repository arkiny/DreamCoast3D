#pragma once
class cUIImageButton;

class cUIImageButton : public cUIObject
{
protected:
	enum eButtonState
	{
		E_NORMAL,
		E_MOUSE_OVER,
		E_SELECTED
	};


	eButtonState m_eButtonState;

	SYNTHESIZE(iButtonDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE_PASS_BY_REF(std::string, m_sNormal, Normal);
	SYNTHESIZE_PASS_BY_REF(std::string, m_sMouseOver, MouseOver);
	SYNTHESIZE_PASS_BY_REF(std::string, m_sSelected, Selected);



public:
	cUIImageButton(LPD3DXSPRITE pSprite);
	virtual ~cUIImageButton(void);

	virtual void Setup(
		std::string sNormal,
		std::string sMouseOver,
		std::string sSelected);

	virtual void Update(float fDelta) override;

	virtual void Render() override;


	enum eButtonType
	{
		E_HEALTH,
		E_MANA,
		E_MAX
	};
	SYNTHESIZE(eButtonType, m_eType, ButtonType);

};

