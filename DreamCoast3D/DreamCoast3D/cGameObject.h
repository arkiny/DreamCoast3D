#pragma once

// 게임 오브젝트, 맵을 포함한 모든 렌더링 되는 오브젝트들은
// 이 함수를 상속받아서 실시한다.
// TODO : 이동할때나 상황에 따라 바운딩 스피어가 움직이가 처리해야한다.
//		  현재 함수는 바운딩 스피어의 Y축 이동이 불완전한 상태
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
		E_EMPTY,
		E_QUEST,
		E_TALK,
		E_STORE,
		E_PORTAL,
		E_BLOCK,
		E_BLOCK_EDIT,
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
	// Height맵에 대한 편의를 위한 함수
	// TODO 차후 바운딩 스피어를 이동시켜야 한다. 끄앙
	//		혹은 업데이트시때마다 수정하게 하는 걸로..?
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
	//virtual ST_BOUNDING_SPHERE* GetAttackSphere(){ return NULL;	}
	virtual std::map<std::string, ST_BOUNDING_SPHERE>* GetAttackSpheres() { return NULL; }

	
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

	virtual float GetSkill1CoolTime() { return 0.0f; }
	virtual void SetSkill1CoolTime(float fCool) {}
	virtual float GetSkill1DelayTime() { return 0.0f; }


    // MS
    virtual void SetHP(float fHP) {};
	//SYNTHESIZE(float, m_fHP, HP);
};

