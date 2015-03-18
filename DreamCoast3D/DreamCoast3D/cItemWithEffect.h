#pragma once

class cItemWithEffect : public cObject, public iActivatable
{
private:
	
public:
	cItemWithEffect();
	virtual ~cItemWithEffect();

	virtual void Activate(
		iGameObjectDelegate* pGameObjgDeligate,
		iEffectManagerDelegate* pEffectDelegate,
		iSceneDelegate* pSceneDeligate,
		iUIManagerDeligate* pUIManagerDeligate,
		iGameEventDelegate* pEventDelegate
		) override;
	virtual float GetCoolTime() {
		return 0.5f;
	};
	std::string GetTexturePath() override { return std::string(""); }
	int GetCount() override { return 65535; }
};

