// outputbar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "AutoWatcher.h"
#include "outputbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, LFDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here
}

COutputBar::~COutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (LFDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!_wndTabs.Create (LFTabWnd::STYLE_3D, rectDummy, this, 1))
	{
		TRACE0("创建输出窗口失败\n");
		return -1;      // fail to create
	}

	// Create list windows:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	_wndList1.m_bVisualManagerStyle = TRUE;
	_wndList2.m_bVisualManagerStyle = TRUE;
	_wndList3.m_bVisualManagerStyle = TRUE;

	if (!_wndList1.Create (dwViewStyle, rectDummy, &_wndTabs, 2) ||
		!_wndList2.Create (dwViewStyle, rectDummy, &_wndTabs, 3) ||
		!_wndList3.Create (dwViewStyle, rectDummy, &_wndTabs, 4))
	{
		TRACE0("创建输出视图错误\n");
		return -1;      // fail to create
	}

	// Attach list windows to tab:
	_wndTabs.AddTab (&_wndList1, _T("输出 1"), -1);
	_wndTabs.AddTab (&_wndList2, _T("输出 2"), -1);
	_wndTabs.AddTab (&_wndList3, _T("输出 3"), -1);

	return 0;
}

void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	LFDockingControlBar::OnSize(nType, cx, cy);
	_wndTabs.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
