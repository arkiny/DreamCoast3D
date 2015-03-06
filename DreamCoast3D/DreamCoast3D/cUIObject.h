#pragma once

// UI에 쓰일 스프라이트 오브젝트들
_declspec(align(16)) class cUIObject : public cObject
{
protected:
	D3DXMATRIXA16			m_matWorld;
	std::vector<cUIObject*> m_vecChild;
	cUIObject*				m_pParent;
	LPD3DXSPRITE			m_pSprite;

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE_PASS_BY_REF(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(int, m_nTag, Tag);
	SYNTHESIZE(RECT, m_rectDrawArea, DrawArea);
	SYNTHESIZE(float, m_fAlpha, Alpha);					//투명도 : 민우
	iGameObjectDelegate* m_pGameObjDelgate;
	iSceneDelegate* m_pSceneDeligate;
	iUIManagerDeligate* m_pUIManagerDeligate;
	/*SYNTHESIZE(iGameObjectDelegate*, m_pGameObjDelgate, GameObjDeligate);
	SYNTHESIZE(iSceneDelegate*, m_pSceneDeligate, SceneDeligate);*/

public:
	cUIObject();
	cUIObject(LPD3DXSPRITE pSprite);

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}

	virtual ~cUIObject();

	virtual void AddChild(cUIObject* pChild);
	virtual std::vector<cUIObject*>& GetChild();

	virtual void Setup(){}
	virtual void Start(){}

	virtual void Update(float fDelta);
	virtual void Render();
	virtual void Destroy();

	virtual void SetGameObjDeligate(iGameObjectDelegate* pGameDeligate);
	virtual void SetSceneDeligate(iSceneDelegate* pSceneDeligate);
	virtual void SetUIManagerDeligate(iUIManagerDeligate* pUImanagerDeligate);

	virtual void SetPosition(float x, float y)
	{
		m_vPosition.x = x;
		m_vPosition.y = y;
	}
};

