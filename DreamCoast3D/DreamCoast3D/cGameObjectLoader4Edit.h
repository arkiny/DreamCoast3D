#pragma once
#include "cLoader.h"

class cSceneEditMode;
class cGamePlayableObject;

class cGameObjectLoader4Edit : public cLoader
{
protected:
	std::vector<std::string> m_vecsFolders;
	std::vector<std::string> m_vecsFiles;

	///
	void ParseAndLoadSkinnedMeshList();
	void ParseAndLoadSkinnedMeshtoManager(int nIndex);
	cGamePlayableObject* ParsePlayerbleObj();
	cGameAIObject* ParseGameAIObj();


	//cGameObject* ParseAndLoadSkinnedBodyMesh();
	//cGameObject* ParseGameActionSkinnedMeshObj();

	void ParseSkinnedCSList(OUT std::vector<std::string>& vecNodeList,
		OUT std::vector<float>& vecRadiusList);

	void ParseSkinnedASList(OUT std::vector<std::string>& vecNodeList,
		OUT std::vector<float>& vecRadiusList);
	///

public:
	cGameObjectLoader4Edit();
	virtual ~cGameObjectLoader4Edit();

	void LoadGameObjectsFromFile(OUT cSceneEditMode* pSceneEditMode, IN std::string sPath);
	void LoadGameObjectsFromFile(OUT cSceneEditMode* pSceneEditMode, IN std::string sFolder, IN std::string sFile);
};

