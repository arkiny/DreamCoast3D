#pragma once
#include "cEffect.h"

struct ST_PARTICLE{
	D3DXVECTOR3		_direction;
	D3DXVECTOR3		_vInitialPos;

	float			_speed;

	float			_lifeSpan;
	float			_currentTime;
};

class cEffectFireBall : public cEffect
{
protected:
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	std::vector<ST_PARTICLE>	m_vecMoveInfo;
	float m_fPassedTime = 0.0f;
	float m_fLifeTime = 2.0f;
	// Utility Function
	DWORD FtoDw(float f)
	{
		return *((DWORD*)&f);
	}

public:
	cEffectFireBall();
	~cEffectFireBall();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
};

