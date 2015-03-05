#pragma once
class cUIImageView : public cUIObject
{
protected:
	std::string m_sTextureFilename;
	D3DXIMAGE_INFO	m_stImageInfo;

	//cUIObject로 옮김. UI리소스의 이미지 크기와 실제 영역이 다른 이상 더 상위클래스에서 필요한듯 : 민우
	//SYNTHESIZE(RECT, m_rectDrawArea, DrawArea);

public:
	cUIImageView(LPD3DXSPRITE pSprite);
	virtual ~cUIImageView(void);
	virtual void SetTextureFilename(std::string& sTextureFilename);
	virtual void Render() override;
	virtual ST_SIZE GetSize() const override;
	
};

