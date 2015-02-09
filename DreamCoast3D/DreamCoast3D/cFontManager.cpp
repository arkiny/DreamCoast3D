#include "stdafx.h"
#include "cFontManager.h"

cFontManager::cFontManager(void)
{
}


cFontManager::~cFontManager(void)
{
}

LPD3DXFONT cFontManager::GetFont(eFontType ft)
{
	if (m_mapFont.find(ft) == m_mapFont.end())
	{
		if (FONT_DEFAULT == ft)
		{
			//��Ʈ ����
			D3DXFONT_DESC fd;
			ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
			fd.Height = 50;
			fd.Width = 25;
			fd.Weight = FW_MEDIUM;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			strcpy_s(fd.FaceName, "�ü�ü");//�۲� ��Ÿ��
			// ttf ����ϱ�
			//AddFontResource("umberto.ttf");
			//strcpy_s(fd.FaceName, "umberto");//�۲� ��Ÿ��

			HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_mapFont[ft]);
			_ASSERT(S_OK == hr && "Font �ε� ����");
		}
	}

	return m_mapFont[ft];
}

void cFontManager::Destroy()
{
	for each(auto p in m_mapFont)
	{
		SAFE_RELEASE(p.second);
	}
}
