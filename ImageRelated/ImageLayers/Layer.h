#pragma once

class CLayer
{
public:
    CLayer(void);
	CLayer(int nZIndex, HBITMAP hBitmap);
    ~CLayer(void);
	void SetArea(const CRect& rect);
    CRect GetArea() const;

	HBITMAP m_hBitmap;

private:
    int m_nZIndex;
    CRect m_rcArea;
};
