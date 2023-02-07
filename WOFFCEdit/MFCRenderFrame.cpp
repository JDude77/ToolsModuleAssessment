#include "MFCRenderFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView
CChildRender::CChildRender()
{
}//End default constructor

CChildRender::~CChildRender()
{
}//End destructor


BEGIN_MESSAGE_MAP(CChildRender, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//CChildView message handlers
BOOL CChildRender::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}//End PreCreateWindow

void CChildRender::OnPaint()
{
	CPaintDC dc(this); //Device context for painting
}//End OnPaint