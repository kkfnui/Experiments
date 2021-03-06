#include "StdAfx.h"
#include "Layer.h"

CLayer::CLayer(void):
m_dZoom(1.0)
{
}

CLayer::CLayer(int nZIndex, HBITMAP hBitmap ):m_nZIndex(nZIndex)
,m_hBitmap(hBitmap)
,m_dZoom(1.0)
{
	m_dZoom = 1.0;
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

void CLayer::SetZoom( const float& zoom )
{
	m_dZoom = zoom;
}

float CLayer::GetZoom()
{
	return m_dZoom;
}



