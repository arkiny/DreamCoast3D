#pragma once

class cGameEventManager;

class cGameEvent : public cObject
{
public:
	cGameEvent();
	virtual ~cGameEvent();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void Exit();
};

