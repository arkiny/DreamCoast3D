#pragma once

// ���� ������Ʈ, ���� ������ ��� ������ �Ǵ� ������Ʈ����
// �� �Լ��� ��ӹ޾Ƽ� �ǽ��Ѵ�.
// @TODO: �׼��� �ִ� ĳ���ʹ� cGameActionObject�� ��ӹް� ���� ����

class cTransform;

class cGameObject : public cObject
{
private:
	D3DXVECTOR3		m_vCenter; // ���Ǿ �̷�� ĳ������ �߽� 
	float			m_fRadius;
	cTransform*		m_pTransform;

public:
	cGameObject();
	virtual ~cGameObject();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual cTransform*			GetTransform();
	virtual D3DXMATRIXA16*		GetTransformMatrix();
	virtual void				SetPosition(D3DXVECTOR3& newPos);
	virtual void				SetScale(D3DXVECTOR3& newScale);
	virtual void				SetXangle(float fAngleRad);
	virtual void				SetYangle(float fAngleRad);
	virtual void				SetZangle(float fAngleRad);
};

