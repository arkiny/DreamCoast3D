#pragma once

class cUITextView;

class cUIExample : public cUIObject, public iButtonDelegate
{
protected:
	LPD3DXSPRITE			m_pSprite;
	cUIObject*				m_pUIRoot;
	cUITextView*			m_pTextview;

public:
	cUIExample();
	virtual ~cUIExample();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void OnClick(cUIImageButton* pSender);
};

