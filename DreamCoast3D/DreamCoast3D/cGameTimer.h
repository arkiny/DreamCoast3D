#pragma once
// Ÿ�̸� �ڵ� ��ó: Intro to DX11 by Frank D. Luna ��Ī �����å
class cGameTimer : public cObject
{
public:
	cGameTimer();
	virtual ~cGameTimer();

	float GameTime() const;		// in sec
	float DeltaTime() const;	// in sec
	float TotalTime() const;

	void Reset(); // Call before message loop, �޽��� �������� ��
	void Start(); // call when unpuased, ����Ÿ�̸Ӱ� ������ �����
	void Stop(); // call when paused ����Ÿ�̸� ����
	void Tick(); // Call every Frame �� �����Ӹ��� ��

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

