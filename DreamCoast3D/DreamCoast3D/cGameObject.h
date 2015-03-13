#pragma once

// ���� ������Ʈ, ���� ������ ��� ������ �Ǵ� ������Ʈ����
// �� �Լ��� ��ӹ޾Ƽ� �ǽ��Ѵ�.
// TODO : �̵��Ҷ��� ��Ȳ�� ���� �ٿ�� ���Ǿ �����̰� ó���ؾ��Ѵ�.
//		  ���� �Լ��� �ٿ�� ���Ǿ��� Y�� �̵��� �ҿ����� ����
class cTransform;
class cGameEvent;

class cGameObject : public cObject
{
public:
	enum eGameObjectType{
		E_PLAYABLE,
		E_NPC,
		E_MOP,
		E_STATIC,
		E_NONE,
		E_EFFECT,
		E_MAP,
		E_SKY,
		E_ERASER,
		E_MAX
	};

	enum eEventType
	{
		E_QUEST,
		E_TALK,
		E_STORE,
		E_PORTAL,
		E_EMPTY,
		E_EVENTMAX
	};

protected:
	SYNTHESIZE(iGameObjectDelegate*, m_pGameObjDeligate, GameObjDeligate);
	SYNTHESIZE(iEffectManagerDelegate*, m_pEffectDelegate, EffectDelegate);
	SYNTHESIZE(eGameObjectType, m_eGameObjectType, GameObjectType);
	SYNTHESIZE(iGameEventDelegate*, m_pEventDelegate, EventDelegate);
	SYNTHESIZE(eEventType, m_eEventType, EventType);

	iGridTileSystem*	m_pGridCallback;
	ST_TILE_GRIDPOS		m_stGridPos;

	ST_BOUNDING_SPHERE	m_stBoundingSphere;
	
	ST_BOUNDING_BOX*	m_pBoundingBox;
	cTransform*			m_pTransform;


public:
	cGameObject();
	virtual ~cGameObject();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void RenderShadow();
	//virtual void Render(LPD3DXEFFECT pEffect);

	virtual cTransform*			GetTransform();
	virtual D3DXMATRIXA16*		GetTransformMatrix();
	
	virtual ST_BOUNDING_SPHERE*	GetBoundingSphere();

	virtual ST_BOUNDING_BOX*	GetBoundingBox();
	virtual void				SetBoundingBox(ST_BOUNDING_BOX* pBoundingBox) { 
		//m_pBoundingBox->vMin = pBoundingBox->vMin; 
		//m_pBoundingBox->vMax = pBoundingBox->vMax;
		m_pBoundingBox = pBoundingBox;
	}

	virtual void				SetPosition(D3DXVECTOR3& newPos);
	virtual D3DXVECTOR3&		GetPosition();
	// Height�ʿ� ���� ���Ǹ� ���� �Լ�
	// TODO ���� �ٿ�� ���Ǿ �̵����Ѿ� �Ѵ�. ����
	//		Ȥ�� ������Ʈ�ö����� �����ϰ� �ϴ� �ɷ�..?
	virtual void				SetYPosition(float y);

	virtual void				SetScale(D3DXVECTOR3& newScale);
	virtual void				SetXangle(float fAngleRad);
	virtual void				SetYangle(float fAngleRad);
	virtual void				SetZangle(float fAngleRad);
	virtual float				GetYangle();

	virtual D3DXVECTOR3&		GetScale();

	virtual void SetGridTileSystem(iGridTileSystem* pGrid);
	virtual iGridTileSystem* GetGridTileSystem() { return m_pGridCallback; }

	// Primitive Interfaces
	virtual void ChangeState(int nState){}
	virtual int GetState() { return -1; }
	virtual std::map<std::string, ST_BOUNDING_SPHERE>* GetUpdatedDetailedSphere() { return NULL; }
	virtual ST_BOUNDING_SPHERE*	GetCollisionSphere(){ return NULL; }
	virtual ST_BOUNDING_SPHERE* GetAttackSphere(){ return NULL;	}
	virtual void ForcedMoving(D3DXVECTOR3 vDir, float fSpeed) {}
	virtual float GetMoveSpeed() { return 1.0f; }
	virtual void SetTargetObject(cGameObject* pTarget) {}
	virtual void OnHitTarget(cGameObject* pTarget, float fDamage, D3DXVECTOR3 vHitPosition) {}
	virtual ST_STAT_INFO* GetStatInfo(){ return NULL; }
	virtual void Start() {}

	// have to be overrided by all savable object
	virtual std::string SaveAsStringInfo() { return std::string(""); }
	// have to be overrdied by all Clonning able object
	// And clone method have to deep copy to target
	// pTarget point has to be NULL
	virtual void Clone(OUT cGameObject** pTarget) {}

    // MS
    virtual void SetHP(float fHP) {};
};

