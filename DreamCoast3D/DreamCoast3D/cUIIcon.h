#pragma once
class cUIIcon : public cUIObject
{
protected:
	//�������� ����
	//�κ��丮 �ȿ��� ����Ŭ��/��Ŭ�� ������ �����̳� ���ƿ��� ��ų�� �������� �����ϱ� ����

	/*
	//������ Ŭ������°� �����ϰ�, �ű⼭ ������ ���� �������� �� ������ ��û�ؾ� �´°� ����.
	
	enum EICONTYPE
	{
		EICONTYPE_ITEM,
		EICONTYPE_EQUIPMENT,
		EICONTYPE_SKILL,
		EICONTYPE_MAX
	};
	EICONTYPE eIconType;
	int nNum;					//������/��ų ��ȣ
	bool isStackable;			//�������� ����������
	int nMaxStack;				//�ִ� � ���� ����������
	int nCurrStack;				//���� � ����������
	*/
	bool m_isClick;				//Ŭ�� �������� ���콺�� Ŭ�� �Ǿ��°�
	bool m_isFloating;			//���콺�� �پ� �ִ� ���ΰ�(�巡�� ���ΰ�)
public:
	cUIIcon();
	~cUIIcon();
};

