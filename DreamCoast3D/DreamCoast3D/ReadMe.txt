Filter ���丮�� Ŭ���� ����
_STDAFX
- ���� ������ �� ���

Core
- DreamCoast3D : WinApi Entry
- cGameCore : cMainGame�� ���� ������ �ϴ� ���� �Ŵ���
- cGameTimer : ��ü ���� Ÿ�̸� // �̱��� ó���� �Ҽ� ������, Update�� ���ڷ� �޾Ƽ� �������� �� �����ӿ�ũ �ۼ��ڰ� ��ȣ
- Resource : WinApi ���ҽ�
- Targetver : ???

Singletons
- cDeviceManager : Direct3D ����̽� �Ŵ���, ��Ʈ���� �ݽ� COM��ü�� �����ֳ� Ȯ���Ѵ�, �������� �� Assert Fail ȣ��
- cObjectManager : cObject �ν�ź������ �����ϴ� �Ŵ���, ��Ʈ���� �ݽ� cObject �ν�ź������ �����ֳ� Ȯ���Ѵ�. �������� �� Assert Fail ȣ��
- cControlManager : ����� ��ġ, ��ǲ ��Ʈ�� �������� �����ϴ� �̱��� Ŭ����. ��� ����� ������ ���⿡�� ����ǰ� ���⼭ Ȯ���Ѵ�.
- cFontManager : ��Ʈ �Ŵ���, ��Ʈ�� �ݽ� �ش� ��Ʈ�� ������ ���� ����, ������ ã�Ƽ� �������ش�.

Object
- cObject : ������Ʈ �ν�ź�� ���̽� Ŭ����
	- cGameObjManager : cGameObject���� ������, �����ϴ� �Ŵ���, Frustom Culling�� ���⼭ ó��, ���� GameObj ��������Ʈ(�浹ó��)�� ���⼭ ó���ұ� ������
		- cGameObject : ��ġ�� ������ ������ ��� ������Ʈ�� ���̽� Ŭ����
			- cTransForm : ��ġ�� ������ ������Ʈ�� ���� Ʈ������ Ŭ����
			- cFrustum : �������� �ø��� ���� Ŭ����, �� GameObjManager�� �ش� Ŭ������ �ϳ��� ������ �ø��Ѵ�.
		- cGameActionObject : �����̰ų� �Էµ� ��Ŀ� ���� �ൿ�� �ǽõǴ� ���� ������Ʈ(ĳ����, ���� ���)
			- Action 
				- cAction : ���� ��� ������Ʈ �ൿ�� ���� ���̽� Ŭ����
				- cActionMove : ������ �Էµ� �������� ������ ���ؼ� �̵�
				- cActionSeq : �׼ǵ��� ������ ������, �� �ൿ�� ������ �ٸ� �ൿ�� �ǽ��Ҽ� �ִ� �׼� ������
				- cActionRepeat : ���� �׼� �������� �Ѱ��� �׼��� �ݺ�
		- cGameBillBoardingObject : �׻� ȭ���� �������� �ٶ󺸴� ������Ʈ

Scene
- iSceneDelegate : ���鰣�� �̵��� ���� ��������Ʈ �������̽�
- cScene : ��, cGameObjManager�� cCamera, cLightSource�� ������ ó���Ѵ�.
- cSceneManager : ������ �����ϴ� �Ŵ���, ����������Ʈ�� ���⼭ ó��
- cCamera : ���� ���ߴ� ī�޶�, �� ���� �ΰ� �̻��� ī�޶� ������ ���� �� ������, �켱 1���� ó��
- cLightSource : ���� ����, ������ D3DX9 ���� ���ѷ��� ���� 8������ ���� �����Ƿ� ���� Ŭ������ ����

Examples - �����ӟp ������
- cSphere : cGameObject�� ��ӹ��� Ŭ������ ��뿹��
- cSphereAction : cGameActionObject�� ��ӹ��� Ŭ������ ��뿹��, ���뿡 ���� ��Ʈ���� ������ ���ӿ�����Ʈ�� ��������(@TODO: ���� ���� �߰�)
- cBillingExample : cGameBillBoardingObject�� ��ӹ��� Ŭ������ ��뿹��
- cSceneExample : cScene�� ��ӹ��� Ŭ������ ��뿹��, cActionMove, cActionRepeat, cActionSeq�� ��뿹��