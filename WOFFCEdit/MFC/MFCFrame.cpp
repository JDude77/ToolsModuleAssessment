#include "MFCFrame.h"
#include "../Resources/resource.h"

BEGIN_MESSAGE_MAP(CMyFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_TOOL
};

//Frame initialiser
CMyFrame::CMyFrame()
{
	//Set to an obviously-wrong selection ID, to verify it's working
	m_selectionID = 999; 
}//End default constructor

void CMyFrame::SetCurrentSelectionID(const int ID)
{
	m_selectionID = ID;
}//End SetCurrentSelectionID

void CMyFrame::OnUpdatePage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	CString strPage;
	strPage.Format(_T("%d"), m_selectionID);
	pCmdUI->SetText(strPage);
}//End OnUpdatePage

//OnCreate - called after init but before window is shown
int CMyFrame::OnCreate(const LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

	//Create a view to occupy the client area of the frame
	//This is where DirectX is rendered
	if (!m_directXView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}//End if

	m_menu1.LoadMenuW(IDR_MENU1);
	SetMenu(&m_menu1);
	
	if (!m_toolBar.CreateEx(this, TBSTYLE_TRANSPARENT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}//End if
	
	CRect rect;
	GetClientRect(&rect);
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}//End if

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	//Set width of status bar panel
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_TOOL, SBPS_NORMAL, rect.Width() - 500);

	return 0;
}//End OnCreate