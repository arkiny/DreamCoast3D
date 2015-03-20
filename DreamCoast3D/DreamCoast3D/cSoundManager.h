#pragma once
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

#define g_pSoundManager cSoundManager::GetInstance()

class cSoundManager
{
	SINGLETONE(cSoundManager);

public:
	void init();
	void executeBGM();
	void executeBGMinGame();
	void executeBeginSpell();
	void executeAttack();
	void executeSkill(int);
	void stopBeginSpell();
	void stopBGM();
	void executeOnHit(int);
	void executeOnDie(int);

	void ExecuteSound(std::string sSoundPath, bool isLoop);
	void StopSound();


private:
	irrklang::ISoundEngine* engine;
	int m_bgmplayinglist;
	bool m_init = true;
	std::string m_sCurrentPlayBGM;
};

