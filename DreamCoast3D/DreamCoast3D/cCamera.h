#pragma once

// 일단 체이싱 카메라
// 고정 카메라는 필요시 확장
#include "iMap.h"
#include "iCameraDeligate.h"
#include "cGamePlayableObject.h"

class cGamePlayableObject;

class cCamera : public cObject, public iCameraDeligate
{
protected:
	SYNTHESIZE(iMap*, m_pMap, Map);
	SYNTHESIZE(iUIManagerDeligate*, m_pUIDelegate, UIDelegate);

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

	float m_fFixedAngleX;
	float m_fFixedAngleY;
	float m_fDirection;
	bool m_isMove;
	bool m_isAttack;
	float m_fPassTime;

	cGamePlayableObject* m_pPlayer;

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

	//virtual void Reset();
	virtual void Update(float delta);	
	virtual void Render() {}
	
	void SetTarget(D3DXVECTOR3* pvTarget);
	D3DXMATRIXA16& GetTransformMatrix();
	D3DXVECTOR3	GetLookAt(){ return m_vLookAt; }
	//void AdjustYPositionByHeightMap(iMap* pMap);

	// MS
	void UpdateAngle(float fAngle);
    virtual void AttackCameraMoving();

	int m_nRunout;

	void SetPlayerForCamera(cGameObject* pPlayer);

	void PlayerFrontUpdateOnMove(float fSensitive);
	void MouseMove(float fSensitive);

	void MouseTrap();

	int m_nCustomAngle;

	POINT ptSave;

	SYNTHESIZE(bool, m_isTrap, Trap);


};

