#include "MFCMain.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//Instantiate the MFC frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	//TODO: Change window name for own sanity
	m_frame->Create(nullptr,
	                _T("World Of Flim-Flam Craft Editor"),
	                WS_OVERLAPPEDWINDOW,
	                CRect(100, 100, 1024, 768),
	                nullptr,
	                nullptr,
	                0,
	                nullptr
	);

	//Show and set the window to run and update
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();

	//Handle of directX child window
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();
	//Get the rect from the MFC window so we can get its dimensions
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}//End InitInstance

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, nullptr, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE) != 0);
		}//End if
		else
		{
			bGotMsg = (GetMessage(&msg, nullptr, 0U, 0U) != 0);
		}//End else

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}//End if
		else
		{	
			int ID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: " + std::to_wstring(ID);
			m_ToolSystem.Tick(&msg);

			//Send current object ID to status bar in the main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}//End else
	}//End while

	return (int)msg.wParam;
}//End Run

void MFCMain::MenuFileQuit()
{
	//Will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}//End MenuFileQuit

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}//End MenuFileSaveTerrain

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);	//Create our dialoguebox
	//m_ToolSelectDialogue.DoModal();											//Start it up modal

	//Modeless dialogue must be declared in the class - if we do local, it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//Show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}//End MenuEditSelect

void MFCMain::ToolBarButton1()
{
	m_ToolSystem.onActionSave();
}//End ToolBarButton1

MFCMain::MFCMain()
{
}//End default constructor

MFCMain::~MFCMain()
{
}//End destructor