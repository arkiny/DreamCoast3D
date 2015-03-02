#pragma once

class cUIMinimap : public cUIWindow
{
protected:

public:
	cUIMinimap();
	virtual ~cUIMinimap();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// MS



	//cUIImageView* m_pMiniMapObject;
	//LPD3DXSPRITE m_pMiniMapSprite;

	//cUIObject* m_pMapRoot;


private:

	virtual void SetObject(std::set<cGameObject*> setGameObject);
	virtual void SetPlayerPosition(D3DXVECTOR3* vPlayerPosition);
	virtual void SetSight(float fSightRange);

	std::vector<D3DXVECTOR3> m_vecPositionGameObjectInSight;
	std::set<cGameObject*> m_setGameObject;

	virtual std::vector<cGameObject*> UpdateInSightObject(ST_BOUNDING_SPHERE stSphere);
	virtual void UpdateMinimap();
	virtual void ObjectPositionUpdate();

	std::vector<cGameObject*> m_vecGameObjectInSight;
	ST_BOUNDING_SPHERE m_stPlayerSightSphere;

	float m_fMiniMapSize;

	virtual void Start();

	LPD3DXSPRITE		 m_pSpriteMiniMap;
	D3DXIMAGE_INFO		 m_stImageInfo;
	LPDIRECT3DTEXTURE9	 m_pTexture;
private:


};

