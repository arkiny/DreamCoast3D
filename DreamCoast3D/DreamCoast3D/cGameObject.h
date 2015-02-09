#pragma once

// 게임 오브젝트, 맵을 포함한 모든 렌더링 되는 오브젝트들은
// 이 함수를 상속받아서 실시한다.
// @TODO: 액션이 있는 캐릭터는 cGameActionObject를 상속받게 만들 예정

class cTransform;

class cGameObject : public cObject
{
protected:
	ST_BOUNDING_SPHERE	m_stBoundingSphere;
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

	virtual void				SetPosition(D3DXVECTOR3& newPos);
	virtual void				SetScale(D3DXVECTOR3& newScale);
	virtual void				SetXangle(float fAngleRad);
	virtual void				SetYangle(float fAngleRad);
	virtual void				SetZangle(float fAngleRad);
	
};

