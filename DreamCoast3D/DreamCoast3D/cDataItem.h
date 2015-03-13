#pragma once
#include "cObject.h"

class cUIIcon;

//HACK: Data��� �θ� �־�� �� ���� �𸣰ڴ�. : �ο�
//HACK: ������ ����� �����ִ� ����� ������ �� ���ڴ�. : �ο�
class cDataItem : public cObject
{
	//�������� ����_
	//�κ��丮 �ȿ��� ����Ŭ��/��Ŭ�� ������ �����̳� ���ƿ��� ��ų�� �������� �����ϱ� ����
	enum EITEMTYPE
	{
		EITEMTYPE_CONSUMABLE,	//�Ҹ�ǰ(ex ����)
		EITEMTYPE_EQUIPMENT,	//���(ex ����)
		EITEMTYPE_ETC,			//��Ÿ(��ɼ��� ���� ������)
		EITEMTYPE_MAX
	};
	static std::vector<cDataItem*> g_pDataItem;	//��� ������������ �� ������ ���� ���� ���� �����̳�
	SYNTHESIZE(cUIIcon*, m_pUIIcon, UIIcon);//�κ��丮 �ȿ� ��� ������ ǥ�õ� ������

	EITEMTYPE	m_eItemType;
	int			m_nItemNum;		//������/��ų ��ȣ(��Ģ �ʿ�)
	bool		m_isStackable;	//�������� ����������
	size_t		m_nMaxStack;	//�ִ� � ���� ����������(�⺻�� 5)
	size_t		m_nCurrStack;	//���� � ����������

public:
	cDataItem();
	cDataItem(cUIIcon* pUIIcon);
	~cDataItem();
	virtual void cDataItem::SetupItemData(EITEMTYPE	eItemType, int nItemNum, bool isStackable, size_t nMaxStack = 5);
	//virtual void cDataItem::SetupItemIcon(cUIIcon* pUIIcon);	//���� ��ü�ȴٸ� ������ ���� ����� �ϳ�? : �ο�
};

