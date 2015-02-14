#pragma once

class cLightSource : public cObject
{
private:
	D3DLIGHT9		m_stLight;
	D3DXVECTOR3		m_vDir;
	int				m_nIndex;

public:
	cLightSource();
	~cLightSource();

	void Setup(D3DLIGHT9& stLight, D3DXVECTOR3& vDirection, int nIndex);
	void Start();
};

