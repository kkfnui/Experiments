#include "StdAfx.h"
#include "Layer.h"

CLayer::CLayer(void)
{
}

CLayer::CLayer(int nZIndex, HBITMAP hBitmap ):m_nZIndex(nZIndex),m_hBitmap(hBitmap)
{

}

CLayer::~CLayer(void)
{
	if (m_hBitmap != INVALID_HANDLE_VALUE)
	{
		DeleteObject(m_hBitmap);
	}
}

void CLayer::SetArea( const CRect& rect )
{
	m_rcArea = rect;
}

CRect CLayer::GetArea() const
{
    return m_rcArea;
}
