Filter ���丮�� Ŭ���� ����
_STDAFX
- ���� ������ �� ���

Core
- DreamCoast3D : WinApi Entry
- cGameCore : cMainGame�� ���� ������ �ϴ� ���� �Ŵ���
- cGameTimer : ��ü ���� Ÿ�̸� // �̱��� ó���� �Ҽ� ������, 
				Update�� ���ڷ� �޾Ƽ� �������� �� �����ӿ�ũ �ۼ��ڰ� ��ȣ
- Resource : WinApi ���ҽ�
- Targetver : ???

Singletons
- cDeviceManager 
	Direct3D ����̽� �Ŵ���, ��Ʈ���� �ݽ� COM��ü�� �����ֳ� Ȯ���Ѵ�, 
	�������� �� Assert Fail ȣ��
- cObjectManager 
	cObject �ν�ź������ �����ϴ� �Ŵ���, ��Ʈ���� �ݽ� cObject �ν�ź������ �����ֳ� Ȯ���Ѵ�. 
	�������� �� Assert Fail ȣ��
- cControlManager 
	����� ��ġ, ��ǲ ��Ʈ�� �������� �����ϴ� �̱��� Ŭ����. 
	��� ����� ������ ���⿡�� ����ǰ� ���⼭ Ȯ���Ѵ�.
- cFontManager 
	��Ʈ �Ŵ���, ��Ʈ�� �ݽ� �ش� ��Ʈ�� ������ ���� ����, ������ ã�Ƽ� �������ش�.
- cSkinnedMeshManager
	��Ų�� �޽� �Ŵ���, ��Ų�� �޽��� D3DXFRAME�� �����Ͽ� �ν�ź���Ѵ�. 
	�ִϸ��̼� ��Ʈ���� ���� ��Ʈ���� Ŭ�δ�(Deep Copy)�ؼ� �Ѱ��ش�.
	- cAllocateHierarchy 
		ID3DXAllocateHierarchy�� ����ؼ� Ȯ���� Ŭ����. 
		XXX : D3DXMESH�� Ȯ���� ST_BONE �� D3DXMESHCONTAINER�� Ȯ���� ST_BONE_MESH�� �����Ѵ�.

Object
- cObject
	������Ʈ ���̽� Ŭ����

	- cGameObjManager 
		cGameObject���� ������, �����ϴ� �Ŵ���, Frustom Culling�� ���⼭ ó��, 
		���� GameObj ��������Ʈ(�浹ó��)�� ���⼭ ó���ұ� ������

		- cGameObject
			��ġ�� ������ ������ ��� ������Ʈ�� ���̽� Ŭ����
		
			- cTransForm 
				��ġ�� ������ ������Ʈ�� ���� Ʈ������ Ŭ����
			- cFrustum 
				�������� �ø��� ���� Ŭ����, �� GameObjManager�� �ش� Ŭ������ �ϳ��� ������ �ø��Ѵ�.

		- cGameActionObject 
				�����̰ų� �Էµ� ��Ŀ� ���� �ൿ�� �ǽõǴ� ���� ������Ʈ(ĳ����, ���� ���)

			- Action 
				- cAction 
					���� ��� ������Ʈ �ൿ�� ���� ���̽� Ŭ����
				- cActionMove 
					������ �Էµ� �������� ������ ���ؼ� �̵�
				- cActionSeq 
					�׼ǵ��� ������ ������, �� �ൿ�� ������ �ٸ� �ൿ�� �ǽ��Ҽ� �ִ� �׼� ������
				- cActionRepeat
					���� �׼� �������� �Ѱ��� �׼��� �ݺ�

			- cGameActionSkinnedMeshObj
				cGameActionObject�� ��ӹް�, D3DXFRAME�� ���� SkinnedMesh �ִϸ��̼��� ������ ���ӿ�����Ʈ
				 - cSkinnedMesh
					- D3DXFRAME�� Animation Control�� ������ ���� ĳ���� �ִϸ��̼��� �ǽ��ϴ� ������Ʈ
					- TODO : ���� ���Ǿ ��忡 ���϶� ���⼭ �ٿ��� cGameActionSkinnedMeshObj�� �����ϴ� ����� ���
				 - cAnimationSet
					- �ִϸ��̼� ��Ʈ���� ���� �ִϸ��̼��� �ǽ��� ��Ʈ

		- cGameBillBoardingObject
			�׻� ȭ���� �������� �ٶ󺸴� ������Ʈ			

		- cGameMapObject 
			iMap �������̽��� ��ӹ��� ���Ӹʿ�����Ʈ, ������ ������ �������ָ�, 
			���� ���ּҸ� ������ �ִ�.
				-iMap : ���� ���̸� �������ִ� ǻ���Լ��� ���� �������̽�

	- cUIObjManager
		UI������Ʈ���� �����ϰ� ������Ʈ�ϰų� ��������Ʈ�� �������ִ� �Ŵ���
		���� �������� ���������μ� ���� ���� ���̾ ������
		���� ���θ޴� ���� �� ������ ������ ������ ���δ�.
		- cUIObject 
			UI���� Object��, ��� cUIObject�� ��� ������ �����Ͽ� ���ο� UI�� �����.
				- cUIImageButton
					�̹�����ư, ��������Ʈ ����Ʈ�� ������ ������, Ŭ���� �ش� ��������Ʈ�� ���� ó��
				- cUIImageView
					�׳� �ؽ��� ��������Ʈ �̹����� ����ϴ� UI������Ʈ
				- cTextView
					������ �ؽ�Ʈ�� ����ϴ� UI������Ʈ
				- iButtonDelegate
					��ư�� ������ ���ϴ� �ൿ��Ŀ� ���õ� �������̽�

Scene
- iSceneDelegate 
	���鰣�� �̵��� ���� ��������Ʈ �������̽�

- cScene 
	��, cGameObjManager�� cCamera, cLightSource, cGameMapObject(vector)�� ������ ó���Ѵ�.
	�� ���� �������Ǹ��� ������ �ִ�.

- cSceneManager 
	������ �����ϴ� �Ŵ���, ����������Ʈ�� ���⼭ ó��

- cCamera 
	���� ���ߴ� ī�޶�, �� ���� �ΰ� �̻��� ī�޶� ������ ���� �� ������, �켱 1���� ó��

- cLightSource 
	���� ����, ������ D3DX9 ���� ���ѷ��� ���� 8������ ���� �����Ƿ� ���� Ŭ������ ����

ParseAndLaoder
- cLoader
	ASE�ļ��� �δ��� �ణ Ʈ���� Ŭ����, ���� ��� �δ� Ŭ������ ���̽� Ŭ������ �ȴ�.
- cGameLoader
	�δ����� �� �����Ǹ� ���������� ��� ���ҽ��� �ϳ��� �������Ϸκ��� �ε��ؿ� Ŭ����
- cGameObjLoader
	���� ������Ʈ �δ�, ����� cGamActionSkinnedMeshObject�ε��� �����Ǿ� �ִ�.

Examples - �����ӟp ������
- cSphere 
	cGameObject�� ��ӹ��� Ŭ������ ��뿹��

- cSphereAction	
	cGameActionObject�� ��ӹ��� Ŭ������ ��뿹��, 
	���뿡 ���� ��Ʈ���� ������ ���ӿ�����Ʈ�� ��������( HACK : ���� ���� �߰� )

- cBillingExample 
	cGameBillBoardingObject�� ��ӹ��� Ŭ������ ��뿹��

- cMapExample 
	cGameMapObject�� ��ӹ��� Ŭ������ ��뿹�� �� �׸����

- cUIExample
	������ ����Ʈ �ڽ� �������� ���� cUIObject ��� ����
	���� ������ �ʿ��� ���δ�.

- cZealot
	cGameActionSkinnedMeshObj�� ��ӹ��� Ŭ������ ��뿹��

- cSceneExample 
	cScene�� ��ӹ��� Ŭ������ ��뿹��, cActionMove, cActionRepeat, cActionSeq�� ��뿹��
	cZealot�� ���� �������� �ν�ź�� �޾Ƽ� Ȱ���ϴ� �κ��� ���� �����ִ�.
	���� cGameObjLoader�� �̿��� ���ҽ��� �ε��ϴ� ���� ���� ���� ���ԵǾ���.