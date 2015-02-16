#pragma once

// �ϴ� ü�̽� ī�޶�
// ���� ī�޶�� �ʿ�� Ȯ��
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

};
