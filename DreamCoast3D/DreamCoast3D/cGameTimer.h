#pragma once
// 타이머 코드 출처: Intro to DX11 by Frank D. Luna 속칭 물방울책
class cGameTimer : public cObject
{
public:
	cGameTimer();
	virtual ~cGameTimer();

	float GameTime() const;		// in sec
	float DeltaTime() const;	// in sec
	float TotalTime() const;

	void Reset(); // Call before message loop, 메시지 루프전에 콜
	void Start(); // call when unpuased, 게임타이머가 정지후 재시작
	void Stop(); // call when paused 게임타이머 정지
	void Tick(); // Call every Frame 매 프레임마다 콜

private:
	double m_dSecondsPerCount;
	double m_dDeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_stopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_bStopped;
};

