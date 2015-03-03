#pragma once

#define g_pEffectResourceManager cEffectResourceManager::GetInstance()

// todo
class cEffect;
class cEffectResourceManager
{
private:
	std::map<std::string, cEffect*> m_mapEffect;

public:
	SINGLETONE(cEffectResourceManager);

	cEffect* GetEffectResource(char* szFolder, char* szFilename);
	cEffect* GetEffectResource(std::string sFolder, std::string sFilename);

	void Destroy();
};

