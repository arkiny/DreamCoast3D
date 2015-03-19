#pragma once
#include "cItemWithEffect.h"

class cSkillIcons : public cItemWithEffect
{
public:
	enum E_SKILL_TYPE{
		E_SKILL_1,
		E_SKILL_2,
		E_SKILL_TYPE_MAX
	};

	SYNTHESIZE(E_SKILL_TYPE, m_eSkilltype, SkillType);

public:
	cSkillIcons();
	virtual ~cSkillIcons();

	virtual void Activate(
		iGameObjectDelegate* pGameObjgDeligate,
		iEffectManagerDelegate* pEffectDelegate,
		iSceneDelegate* pSceneDeligate,
		iUIManagerDeligate* pUIManagerDeligate,
		iGameEventDelegate* pEventDelegate
		) override;

	virtual float GetCoolTime();

	virtual std::string GetTexturePath() override;
	virtual int GetCount() override;
	virtual void SetCount(int n);
};

