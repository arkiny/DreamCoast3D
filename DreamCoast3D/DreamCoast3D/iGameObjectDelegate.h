#pragma once

class cGameObject;

__interface iGameObjectDelgate{
	bool isCollideWith(cGameObject* pSender);
};