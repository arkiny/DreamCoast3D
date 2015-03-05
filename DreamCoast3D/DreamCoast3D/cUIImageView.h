#pragma once
class cUIImageView : public cUIObject
{
protected:
	std::string m_sTextureFilename;
	D3DXIMAGE_INFO	m_stImageInfo;

	//cUIObject�� �ű�. UI���ҽ��� �̹��� ũ��� ���� ������ �ٸ� �̻� �� ����Ŭ�������� �ʿ��ѵ� : �ο�
	//SYNTHESIZE(RECT, m_rectDrawArea, DrawArea);

public:
	cUIImageView(LPD3DXSPRITE pSprite);
	virtual ~cUIImageView(void);
	virtual void SetTextureFilename(std::string& sTextureFilename);
	virtual void Render() override;
	virtual ST_SIZE GetSize() const override;
	
};

