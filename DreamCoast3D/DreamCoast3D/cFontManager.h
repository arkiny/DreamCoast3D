#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
public:
	enum eFontType
	{
		FONT_DEFAULT,
		FONT_QUEST
	};

private:
	std::map<eFontType, LPD3DXFONT> m_mapFont;

public:
	SINGLETONE(cFontManager);

	LPD3DXFONT GetFont(eFontType ft);

	void Destroy();
};

