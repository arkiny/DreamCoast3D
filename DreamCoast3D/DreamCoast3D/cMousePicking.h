#pragma once

struct ST_RAY
{
	ST_RAY()
	{
		ZeroMemory(&vecOrigin, sizeof(D3DXVECTOR3));
		ZeroMemory(&vecDirection, sizeof(D3DXVECTOR3));
	}
	D3DXVECTOR3 vecOrigin;
	D3DXVECTOR3 vecDirection;
};

class cMousePicking
{
protected:
	SYNTHESIZE(iEffectManagerDelegate*, m_pEffectDelegate, EffectDelegate);

public:
	cMousePicking();
	~cMousePicking();

	void SetVertex(std::vector<ST_PNT_VERTEX> Vertex);
	void Update();

private:
	void MouseUpdate();
	ST_RAY CalPickingRay(D3DXVECTOR2 vMouse);
	void TransformRay(ST_RAY* pRay, D3DXMATRIXA16* mat);
	void IntersetionTriUpdate();

private:
	std::vector<ST_PNT_VERTEX> m_vecVertex;
	bool m_isRightButton;
	D3DXVECTOR2 m_vMouse;
	D3DXVECTOR3 m_vClickedPosition;

};

