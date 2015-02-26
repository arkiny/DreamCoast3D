#pragma once

// ���� ������Ʈ, ���� ������ ��� ������ �Ǵ� ������Ʈ����
// �� �Լ��� ��ӹ޾Ƽ� �ǽ��Ѵ�.
// TODO : �̵��Ҷ��� ��Ȳ�� ���� �ٿ�� ���Ǿ �����̰� ó���ؾ��Ѵ�.
//		  ���� �Լ��� �ٿ�� ���Ǿ��� Y�� �̵��� �ҿ����� ����
class cTransform;

class cGameObject : public cObject
{
protected:
	SYNTHESIZE(iGameObjectDelegate*, m_pGameObjDeligate, GameObjDeligate);

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

	virtual cTransform*			GetTransform();
	virtual D3DXMATRIXA16*		GetTransformMatrix();
	
	virtual ST_BOUNDING_SPHERE*	GetBoundingSphere();

	virtual ST_BOUNDING_SPHERE*	GetCollisionSphere(){
		return NULL;
	}

	virtual ST_BOUNDING_SPHERE* GetAttackSphere(){
		return NULL;
	}

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

	//
	virtual iGridTileSystem* GetGridTileSystem() { return m_pGridCallback; }

	// override
	virtual void ChangeState(int nState){}
};

