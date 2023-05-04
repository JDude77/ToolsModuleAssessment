#pragma once
#include "afxdialogex.h"
#include "../Resources/resource.h"
#include "afxwin.h"
#include "../Tool/SceneObject.h"
#include <vector>

class SelectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SelectDialogue)

public:
	SelectDialogue(CWnd* pParent, std::vector<SceneObject>* sceneGraph);   // Modal - takes in our scenegraph in the constructor
	SelectDialogue(CWnd* pParent = nullptr);
	virtual ~SelectDialogue();
	void SetObjectData(std::vector<SceneObject>* sceneGraph, int* selection);	//Passing in pointers to the data the class will operate on
	
//Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); //DDX/DDV support
	afx_msg void End();		//Kill the dialog box
	afx_msg void Select();	//Item has been selected

	std::vector<SceneObject> * m_sceneGraph{};
	int* m_currentSelection{};

	DECLARE_MESSAGE_MAP()
public:
	//Control variable for more efficient access of the listbox
	CListBox m_listBox;
	int m_startSelected;
	BOOL m_active = false;
	virtual BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList1();
};

INT_PTR CALLBACK SelectProc( HWND   hwndDlg,UINT   uMsg,WPARAM wParam,LPARAM lParam);