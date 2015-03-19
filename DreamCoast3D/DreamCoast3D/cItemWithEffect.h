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
	virtual std::string GetTexturePath() override { return std::string(""); }
	virtual int GetCount() override { return 65535; }
	virtual void SetCount(int n) {};
};

