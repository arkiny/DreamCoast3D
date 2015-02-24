#pragma once

// 일단 체이싱 카메라
// 고정 카메라는 필요시 확장
#include "iMap.h"
class cCamera : public cObject
{
protected:
	SYNTHESIZE(iMap*, m_pMap, Map);
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3*	m_pvTarget;

	POINT			m_ptPrevMouse;

	bool			m_isRButtonDown;
	float			m_fAngleX;
	float			m_fAngleY;
	float			m_fDist;
	float			m_fMin;
	float			m_fMax;

public:
	cCamera(void);
	~cCamera(void);

	virtual void Setup();
	virtual void Setup(
		D3DXVECTOR3& vEye,
		D3DXVECTOR3& vLookAt,
		D3DXVECTOR3& vUp,
		float fAngleX,
		float fAngleY,
		float fDist,
		float fMin,
		float fMax);

	virtual void Update(float delta);	
	virtual void Render() {}
	
	void SetTarget(D3DXVECTOR3* pvTarget);
	D3DXMATRIXA16& GetTransformMatrix();

	//void AdjustYPositionByHeightMap(iMap* pMap);

	// MS
	void FixedCamera(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookAt);
	void SetSightTarget();
	void UpdateAngle(float fAngle);

    void UpdateFixedCamera();
	D3DXVECTOR3 m_vFixedEye;
	D3DXVECTOR3 m_vFixedLookAt;
	float m_fFixedAngleX;
    float m_fFixedAngleY;
    float m_fFixedDist;

    LPD3DXMESH m_pMesh;

    bool m_isSpcDown;
};

