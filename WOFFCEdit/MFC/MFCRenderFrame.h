#pragma once
#include <afxwin.h> 
#include <afxext.h>

//CChildView window
class CChildRender : public CWnd
{
public:
	CChildRender();
	virtual ~CChildRender();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:
	//Generated message map functions
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};