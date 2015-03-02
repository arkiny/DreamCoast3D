#pragma once

class cScene;

__interface iSceneDelegate {
	void SceneFinished(cScene* pSender);
	void ChangeScene(int nNextSceneIndex);
	void ChangeSceneFromLoader(cScene* pNextScene);
};