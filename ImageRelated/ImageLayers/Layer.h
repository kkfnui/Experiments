#pragma once

class CLayer
{
public:
    CLayer(void);
	CLayer(int nZIndex, HBITMAP hBitmap);
    ~CLayer(void);
	void SetArea(const CRect& rect);
	void SetZoom(const float& zoom);
	float GetZoom();
    CRect GetArea() const;
	HBITMAP m_hBitmap;

private:
    int m_nZIndex;
    CRect m_rcArea;
	double m_dZoom;
};
