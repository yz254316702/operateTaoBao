
// TaoBaoTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTaoBaoToolApp:
// �йش����ʵ�֣������ TaoBaoTool.cpp
//

class CTaoBaoToolApp : public CWinApp
{
public:
	CTaoBaoToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTaoBaoToolApp theApp;