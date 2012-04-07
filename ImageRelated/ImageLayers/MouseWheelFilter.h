#pragma once
#include "stdafx.h"

class CMouseWheelFilter:public CMessageFilter
{
public:
	CMouseWheelFilter(void);
	~CMouseWheelFilter(void);

	virtual BOOL PreTranslateMessage( MSG* pMsg );

};
