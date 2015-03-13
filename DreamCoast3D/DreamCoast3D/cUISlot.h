#pragma once
#include "cUIImageView.h"
#include "cUIIcon.h"

class cDataItem;

class cUISlot : public cUIImageView
{
protected:
	SYNTHESIZE(int, m_nNum, Num);			//�ڱ��ڽ��� ��ȣ(�����̳� �ε����� ����)
	SYNTHESIZE(cUIIcon*, m_pIcon, Icon);	//������ ������ ������
	cDataItem* m_pItem;						//���Կ� �Ҵ�Ǿ� �ִ� ������
public:
	cUISlot();
	cUISlot(LPD3DXSPRITE pSprite);
	~cUISlot();
	virtual void Update(float fDelta) override;
	virtual void Render() override;
	virtual void SetItem(cDataItem* pDataItem);
	virtual cDataItem* GetItem();
	virtual void DeleteItem();

};