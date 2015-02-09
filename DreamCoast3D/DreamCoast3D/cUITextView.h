#pragma once
class cUITextView : public cUIObject
{
protected:
	SYNTHESIZE(std::string, m_sText, Text);
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
	SYNTHESIZE(D3DCOLOR, m_dwTextColor, TextColor);

public:
	cUITextView(LPD3DXSPRITE pSprite);
	virtual ~cUITextView(void);

	virtual void Render() override;
};

