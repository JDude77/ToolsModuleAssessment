#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "../Renderer/pch.h"
#include "../Renderer/Game.h"
#include "../Tool/ToolMain.h"
#include "../Resources/resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame*		m_frame{};				//Handle to the frame where all our UI is
	HWND			m_toolHandle{};			//Handle to the MFC window
	ToolMain		m_toolSystem;			//Instance of Tool System that we interface to
	CRect			windowRect;				//Window area rectangle
	SelectDialogue	m_toolSelectDialogue;	//For modeless dialogue, declare it here

	int m_width{};		
	int m_height{};
	
	//Interface funtions for menu, toolbar, etc
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void MenuEditUndo();
	afx_msg void MenuEditRedo();
	afx_msg	void ToolBarSave();

	//Required macro for message map functionality - one per class
	DECLARE_MESSAGE_MAP()	
};