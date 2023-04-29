#pragma once
#include <afxwin.h> 
#include <afxext.h>
#include "MFCRenderFrame.h"
#include "../Tool/ToolMain.h"

class CMyFrame : public CFrameWnd
{
protected:
//	DECLARE_DYNAMIC(CMainFrame)
public:
	CMenu			m_menu1;
	CStatusBar		m_wndStatusBar;
	CToolBar		m_toolBar;
	CChildRender	m_directXView;

public:
	CMyFrame();
	void SetCurrentSelectionID(int ID);
	afx_msg void OnUpdatePage(CCmdUI *pCmdUI);

private:
	int	m_selectionID;

	//Note: the afx_msg keyword is linking this method to message map access
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//Required macro for message map functionality - one per class
	DECLARE_MESSAGE_MAP()	
};