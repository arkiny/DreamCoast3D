#pragma once
#include "cObject.h"
#include "cUIIcon.h"

//HACK: Data��� �θ� �־�� �� ���� �𸣰ڴ�. : �ο�
class cDataItem : public cUIIcon
{
	
	//�������� ����
	//�κ��丮 �ȿ��� ����Ŭ��/��Ŭ�� ������ �����̳� ���ƿ��� ��ų�� �������� �����ϱ� ����
	enum EITEMTYPE
	{
		EITEMTYPE_CONSUMABLE,	//�Ҹ�ǰ(ex ����)
		EITEMTYPE_EQUIPMENT,	//���(ex ����)
		EITEMTYPE_ETC,			//��Ÿ(��ɼ��� ���� ������)
		EITEMTYPE_MAX
	};
	EITEMTYPE eItemType;
	int nNum;					//������/��ų ��ȣ(��Ģ �ʿ�)
	bool isStackable;			//�������� ����������
	int nMaxStack;				//�ִ� � ���� ����������
	int nCurrStack;				//���� � ����������

public:
	cDataItem();
	~cDataItem();
};

