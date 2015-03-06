#pragma once

class cSceneEditMode;

class cGameObjectEraser : public cGameObject
{
protected:
	LPD3DXMESH				m_pMesh;
	ST_BOUNDING_SPHERE		m_stSphere;
	float					m_fRadius = 5.0f;
	
	SYNTHESIZE(cSceneEditMode*, m_pOwner, Owner);

	virtual bool CheckCollided(){ return false; }
	virtual void EraseFromTheEditScene(){}

public:
	cGameObjectEraser();
	virtual ~cGameObjectEraser();

	virtual void Setup()				override;
	virtual void Update(float fDelta)	override;
	virtual void Render() override;

	
	virtual ST_BOUNDING_SPHERE*	GetBoundingSphere() override;
	void	SetPosition(D3DXVECTOR3& newPos) override;
};

