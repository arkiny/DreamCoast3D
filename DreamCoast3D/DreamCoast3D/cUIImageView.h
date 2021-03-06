#pragma once
class cUIImageView : public cUIObject
{
protected:
	std::string m_sTextureFilename;
	D3DXIMAGE_INFO	m_stImageInfo;
	SYNTHESIZE(bool, m_bShowChild, isShowingChildren);

public:
	cUIImageView() : m_bShowChild(true) {};
	cUIImageView(LPD3DXSPRITE pSprite);
	virtual ~cUIImageView(void);
	virtual void Setup(std::string sTexturePath, int nDrawRectRight, int nDrawRectBottom, D3DXVECTOR3& vPos, D3DXVECTOR3& vScale);
	virtual std::string& cUIImageView::GetTextureFilename();
	virtual void SetTextureFilename(std::string& sTextureFilename);
	virtual void Render() override;
	virtual ST_SIZE GetSize() const override;
	
};

