#pragma once

class cGameObject;

class cAction : public cObject
{
protected:
	float m_fPassedTime;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(cGameObject*, m_pOwner, Owner);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(bool, m_bNoMove, isNoMove);

public:
	cAction();
	virtual ~cAction();
	virtual void Start() {}

	//Same as Execute;
	virtual void Update(float delta) = 0;
	virtual void Exit() {}
};

