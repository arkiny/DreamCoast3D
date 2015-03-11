#pragma once
#include "cUIImageView.h"
#include "cUIIcon.h"

class cDataItem;

class cUISlot : public cUIImageView
{
protected:
	SYNTHESIZE(int, m_nNum, Num);			//�ڱ��ڽ��� ��ȣ(�����̳� �ε����� ����)
	SYNTHESIZE(cUIIcon*, m_pIcon, Icon);
	SYNTHESIZE(int, m_nItemNum, ItemNum);	//�ڱⰡ ������ �ִ� ������ ��ȣ(���� ã���) //HACK: map�̸� �ξ� ���ϰڴ�.
	SYNTHESIZE(cDataItem*, m_pItem, Item);	//�ڱⰡ ������ �ִ� ������ ������
public:
	cUISlot();
	cUISlot(LPD3DXSPRITE pSprite);
	~cUISlot();
	//virtual void Update(float fDelta);
	virtual void Render();
};