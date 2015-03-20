#pragma once
#include "iPlayerState.h"
#include "cGamePlayableObject.h"

//모든 동작의 공통적인 흐름을 가지고 있는 베이스 클래스
class cPlayerCommon : public iPlayerState
{
public:
	
protected:
	//SYNTHESIZE(int, m_nCurrentStateType, CurrentStateType);//(인터페이스 호환성 문제로 Synthesize사용불가)
	int m_nCurrentStateType;	//현재 상태 Enum에 대응하는 번호. 각 하위 클래스는 생성자에서 이 멤버를 초기화 해야한다.
	//SYNTHESIZE(bool, m_IsRestart, IsRestart);	//상태가 완료되면 처음부터 다시 시작되는 상태인지 여부. 각 하위 클래스는 생성자에서 이 멤버를 초기화 해야한다.
	
	//std::map < int, ECANCELTYPE > m_mapCancelInfo;		//현재 동작이 다음 동작으로 가는데 캔슬 가능 불가능 여부
	std::map < int, ECANCELTYPE > m_mapCancelInfo;		//현재 동작이 다음 동작으로 가는데 캔슬 가능 불가능 여부
	bool m_IsRestart;	//반복될 동작인가
	bool m_IsDoing;		//동작이 진행중인가
	ESTATEGROUP m_eStateType;								//어떤 형태의 상태인지
	
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

