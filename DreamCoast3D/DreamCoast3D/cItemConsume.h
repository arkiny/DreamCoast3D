#pragma once
#include "cItemWithEffect.h"

class cItemConsume : public cItemWithEffect
{
public:
	enum E_CONSUME_TYPE{
		E_CONSUME_HP_UP,
		E_CONSUME_MP_UP
	};
	SYNTHESIZE(E_CONSUME_TYPE, m_eConsumeType, ConsumeType);
	SYNTHESIZE(float, m_fItemCoolTime, ItemCoolTime);
	SYNTHESIZE(int, m_nCount, CountItem);


public:
	cItemConsume();
	virtual ~cItemConsume();

	virtual void Activate(
		iGameObjectDelegate*	pGameObjgDeligate,
		iEffectManagerDelegate* pEffectDelegate,
		iSceneDelegate*			pSceneDeligate,
		iUIManagerDeligate*		pUIManagerDeligate,
		iGameEventDelegate*		pEventDelegate
		) override;

	virtual float GetCoolTime() {
		return m_fItemCoolTime;
	};

	virtual std::string GetTexturePath();
	int GetCount() { return m_nCount; }
};

