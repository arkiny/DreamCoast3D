#pragma once
#include "iPlayerState.h"
#include "cGamePlayableObject.h"

//��� ������ �������� �帧�� ������ �ִ� ���̽� Ŭ����
class cPlayerCommon : public iPlayerState
{
public:
	
protected:
	//SYNTHESIZE(int, m_nCurrentStateType, CurrentStateType);//(�������̽� ȣȯ�� ������ Synthesize���Ұ�)
	int m_nCurrentStateType;	//���� ���� Enum�� �����ϴ� ��ȣ. �� ���� Ŭ������ �����ڿ��� �� ����� �ʱ�ȭ �ؾ��Ѵ�.
	//SYNTHESIZE(bool, m_IsRestart, IsRestart);	//���°� �Ϸ�Ǹ� ó������ �ٽ� ���۵Ǵ� �������� ����. �� ���� Ŭ������ �����ڿ��� �� ����� �ʱ�ȭ �ؾ��Ѵ�.
	
	//std::map < int, ECANCELTYPE > m_mapCancelInfo;		//���� ������ ���� �������� ���µ� ĵ�� ���� �Ұ��� ����
	std::map < int, ECANCELTYPE > m_mapCancelInfo;		//���� ������ ���� �������� ���µ� ĵ�� ���� �Ұ��� ����
	bool m_IsRestart;	//�ݺ��� �����ΰ�
	bool m_IsDoing;		//������ �������ΰ�
	ESTATEGROUP m_eStateType;								//� ������ ��������
	
public:
	cPlayerCommon();
	~cPlayerCommon();
	virtual void Start(cGamePlayableObject* pPlayer);
	virtual void Execute(cGamePlayableObject* pPlayer, float fDelta);
	virtual void Exit(cGamePlayableObject* pPlayer);
	virtual int GetCurrentStateType() { return m_nCurrentStateType; }
	virtual std::map < int, ECANCELTYPE > GetCancelInfo(){ return m_mapCancelInfo; }
	virtual bool GetIsRestart(){ return m_IsRestart; }
	virtual void SetIsRestart(bool bRestart){ m_IsRestart = bRestart; }
	virtual bool GetIsDoing(){ return m_IsDoing; }
	virtual void SetIsDoing(bool bDoing){ m_IsDoing = bDoing; }
<<<<<<< HEAD
	virtual ESTATEGROUP GetStateType(){ return m_eStateType; }
	virtual void SetStateType(ESTATEGROUP eStateType){ m_eStateType = eStateType; }
=======
	//virtual bool GetIsRestart(){ return m_IsRestart; }
	//virtual void SetIsRestart(bool IsRestart){ m_IsRestart = IsRestart; };


private:
	float m_fDeadAccumTime;
>>>>>>> origin/master
};

