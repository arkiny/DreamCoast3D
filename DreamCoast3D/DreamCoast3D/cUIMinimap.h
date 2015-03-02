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

	virtual void SetObject(std::vector<cGameObject*> m_setGameObject);
	virtual void SetPlayerPosition(D3DXVECTOR3* vPlayerPosition);
	virtual void SetSight(float fSightRange);

private:

	std::set<cGameObject*> m_setGameObject;
	std::vector<cGameObject*> m_vecGameObjectInSight;
	ST_BOUNDING_SPHERE m_stPlayerSightSphere;
	std::vector<D3DXVECTOR3> m_vecPositionGameObjectInSight;



	std::vector<ST_PC_VERTEX> m_vecParticle;

	D3DXMATRIXA16 m_mat;

private:

	virtual std::vector<cGameObject*> UpdateInSightObject(ST_BOUNDING_SPHERE stSphere);
	virtual void UpdateMinimap();
	virtual void ParticleSetup();
	virtual void ParticleRender();

	
};

