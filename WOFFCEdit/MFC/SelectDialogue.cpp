// SelectDialogue.cpp : implementation file

#include "../stdafx.h"
#include "SelectDialogue.h"

IMPLEMENT_DYNAMIC(SelectDialogue, CDialogEx)

//Message map - just like MFCMAIN.cpp
//This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(SelectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SelectDialogue::End)					//OK button
	ON_BN_CLICKED(IDOK, &SelectDialogue::OnBnClickedOk)		
	ON_LBN_SELCHANGE(IDC_LIST1, &SelectDialogue::Select)	//Listbox
END_MESSAGE_MAP()

//Constructor used in modal
SelectDialogue::SelectDialogue(CWnd* pParent, std::vector<SceneObject>* sceneGraph)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_sceneGraph = sceneGraph;
}//End modal constructor

//Constructor used in modeless
SelectDialogue::SelectDialogue(CWnd * pParent)
	: CDialogEx(IDD_DIALOG1, pParent), m_sceneGraph(nullptr), m_currentSelection(nullptr)
{
} //End modeless constructor

SelectDialogue::~SelectDialogue()
{
}//End destructor

//Pass through pointers to the data in the tool we want to manipulate
void SelectDialogue::SetObjectData(std::vector<SceneObject>* sceneGraph, int * selection)
{
	m_sceneGraph = sceneGraph;
	m_currentSelection = selection;

	const int numSceneObjects = m_sceneGraph->size();
	//Iterate through all the objects in the scene graph and put an entry for each in the listbox
	for (int i = 0; i < numSceneObjects; i++)
	{
		//Easily possible to make the data string presented more complex, showing other columns
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}//End for
}//End SetObjectData

void SelectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
}//End DoDataExchange

void SelectDialogue::End()
{
	//Destroy the window properly, including the links and pointers created
	//This is so the dialogue can start again
	DestroyWindow();
}//End End

void SelectDialogue::Select()
{
	const int index = m_listBox.GetCurSel();
	CString currentSelectionValue;
	
	m_listBox.GetText(index, currentSelectionValue);

	*m_currentSelection = _ttoi(currentSelectionValue);
}//End Select

BOOL SelectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//Uncomment for modal only
	/*//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (size_t i = 0; i < numSceneObjects; i++)
	{
		//Easily possible to make the data string presented more complex, showing other columns
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}*/
	
	return TRUE;  //Return true, unless you set the focus to a control
				  //EXCEPTION: OCX Property Pages should return FALSE
}//End OnInitDialogue

void SelectDialogue::PostNcDestroy()
{
}//End PostNcDestroy

//SelectDialogue message handlers callback
//We only need this if the dailogue is being set up with createDialogue()
//We are doing it manually, so it's better to use the messagemap

/*INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		//	EndDialog(hwndDlg, wParam);
			DestroyWindow(hwndDlg);
			return TRUE;
			

		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
			break;
		}//End switch
	}//End switch
	
	return INT_PTR();
}//End SelectProc*/

void SelectDialogue::OnBnClickedOk()
{
	//TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}//End OnBnClickedOk