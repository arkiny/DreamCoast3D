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
	SYNTHESIZE(cUIIcon*, m_pUIIcon, UIIcon);//�κ��丮 �ȿ� ��� ������ ǥ�õ� ������

	EITEMTYPE	eItemType;
	int			nItemNum;		//������/��ų ��ȣ(��Ģ �ʿ�)
	bool		isStackable;	//�������� ����������
	int			nMaxStack;		//�ִ� � ���� ����������
	int			nCurrStack;		//���� � ����������

public:
	cDataItem();
	~cDataItem();
};

