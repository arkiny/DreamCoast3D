#pragma once

// �ϴ� ü�̽� ī�޶�
// ���� ī�޶�� �ʿ�� Ȯ��

class cCamera : public cObject
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3*	m_pvTarget;

	POINT			m_ptPrevMouse;

	bool			m_isRButtonDown;
	float			m_fAngleX;
	float			m_fAngleY;
	float			m_fDist;

public:
	cCamera(void);
	~cCamera(void);

	void Setup();
	void Update(float delta);	
	void SetTarget(D3DXVECTOR3* pvTarget);
	D3DXMATRIXA16& GetTransformMatrix();
};

