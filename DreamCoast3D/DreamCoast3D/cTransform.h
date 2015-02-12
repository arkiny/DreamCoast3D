#pragma once

// 월드 매트릭스를 가지는 함수
// 모든 위치를 가진 오브젝트들은 이 함수를 가지고 있고,
// 이 함수를 기준으로 월드 트랜스폼을 실시한다.

__declspec(align(16)) class cTransform : public cObject
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(float, m_fYAxisAngle, YAxisAngle);
	SYNTHESIZE(float, m_fXAxisAngle, XAxisAngle);
	SYNTHESIZE(float, m_fZAxisAngle, ZAxisAngle);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vScale, Scaling);
	D3DXMATRIXA16 m_matTransform;

public:
	cTransform();
	virtual ~cTransform();

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}

	D3DXVECTOR3* getPosPointer(){ return &m_vPosition; }

	D3DXMATRIXA16* GetTransformMatrix();
};