// WordAna.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWordAnaApp:
// �йش����ʵ�֣������ WordAna.cpp
//

class CWordAnaApp : public CWinApp
{
public:
	CWordAnaApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWordAnaApp theApp;