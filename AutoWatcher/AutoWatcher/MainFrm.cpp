// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AutoWatcher.h"

#include "MainFrm.h"
#include "BackStagePageInfo.h"

#include "../WatchCore/IntValue.h"
#include "../Logger/AppLogger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, LFMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, LFMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_WORKSPACE2, OnViewWorkspace2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE2, OnUpdateViewWorkspace2)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewPropGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewPropGrid)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_REGISTERED_MESSAGE(BCGM_ON_BEFORE_RIBBON_BACKSTAGE_VIEW, OnBeforeShowRibbonBackstageView)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (LFMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LFMDITabParams mdiTabParams;

	mdiTabParams.m_bTabIcons = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	mdiTabParams.m_style = LFTabWnd::STYLE_3D_SCROLLED;
	mdiTabParams.m_closeButtonMode = LFTabWnd::TAB_CLOSE_BUTTON_HIGHLIGHTED_COMPACT;

	EnableMDITabbedGroups (TRUE, mdiTabParams);

	EnableTearOffMDIChildren();

	if (!CreateRibbonBar ())
	{
		TRACE0("创建功能区失败\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("创建状态栏失败\n");
		return -1;      // fail to create
	}

	// TODO: add your status bar panes here:
	m_wndStatusBar.AddElement (new LFRibbonStatusBarPane (
		ID_STATUSBAR_PANE1, _T("面板 1"), TRUE), _T("面板 1"));
	m_wndStatusBar.AddExtendedElement (new LFRibbonStatusBarPane (
		ID_STATUSBAR_PANE2, _T("面板 2"), TRUE), _T("面板 2"));

	// Load control bar icons:
	LFToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.Load (IDB_WORKSPACE);
	imagesWorkspace.SmoothResize(globalData.GetRibbonImageScale());

	if (!m_wndWorkSpace.Create (_T("视图  1"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("创建工作区失败\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	if (!m_wndWorkSpace2.Create (_T("视图 2"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE2,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("创建工作区失败 2\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);


	if (!m_wndOutput.Create (_T("输出"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("创建输出窗口失败\n");
		return -1;      // fail to create
	}
	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);

	if (!m_wndPropGrid.Create (_T("属性"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_VIEW_PROPERTIES,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("创建属性栏失败\n");
		return FALSE;		// fail to create
	}

	m_wndPropGrid.SetIcon (imagesWorkspace.ExtractIcon (3), FALSE);

	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPropGrid.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndWorkSpace);
	m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);
	DockControlBar(&m_wndOutput);
	DockControlBar(&m_wndPropGrid);

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable windows navigator (activated by Ctrl+Tab/Ctrl+Shift+Tab):
	EnableWindowsNavigator();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !LFMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	LFMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	LFMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

BOOL CMainFrame::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu (bDrop ? IDR_POPUP_DROP_MDITABS : IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_VERT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	LFPopupMenu* pPopupMenu = new LFPopupMenu;
	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->Create (this, point.x, point.y, pPopup->GetSafeHmenu ());

	return TRUE;
}

void CMainFrame::OnMdiMoveToNextGroup() 
{
	MDITabMoveToNextGroup ();
}

void CMainFrame::OnMdiMoveToPrevGroup() 
{
	MDITabMoveToNextGroup (FALSE);
}

void CMainFrame::OnMdiNewHorzTabGroup() 
{
	MDITabNewGroup (FALSE);
}

void CMainFrame::OnMdiNewVertGroup() 
{
	MDITabNewGroup ();
}

void CMainFrame::OnMdiCancel() 
{
	// TODO: Add your command handler code here
	
}

BOOL CMainFrame::CreateRibbonBar ()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	if (!m_wndRibbonBar.LoadFromXML (_T("IDR_BCGP_RIBBON_XML")))
	{
		return FALSE;
	}

	LFRibbonCategory* pBackstageCategory = m_wndRibbonBar.GetBackstageCategory ();

	if (pBackstageCategory != NULL)
	{
		LFRibbonBackstageViewPanel* pBackstagePanel = DYNAMIC_DOWNCAST (LFRibbonBackstageViewPanel, pBackstageCategory->GetPanel (0));

		if (pBackstagePanel != NULL)
		{
			pBackstagePanel->AttachViewToItem (5, new LFRibbonBackstageViewItemForm(IDD_FORM_INFO, RUNTIME_CLASS(CBackStagePageInfo)), FALSE);
			pBackstagePanel->AttachRecentViewToItem(6, BCGP_SHOW_FOLDERS_LIST | BCGP_SHOW_PINS, FALSE);
		}
	}
 // USE_RIBBON_DESIGNER

	m_wndRibbonBar.EnablePrintPreview(FALSE);

	return TRUE;
}

LRESULT CMainFrame::OnRibbonCustomize (WPARAM wp, LPARAM /*lp*/)
{
	ShowOptions (wp == 0 ? 1 : 0);
	return 1;
}

void CMainFrame::OnToolsOptions()
{
	ShowOptions (0);
}

void CMainFrame::ShowOptions (int nPage)
{
	// Create custom categories:
	
	// "Popular" items:
	
	CList<UINT, UINT> lstPopular;
	
	lstPopular.AddTail (ID_FILE_NEW);
	lstPopular.AddTail (ID_FILE_OPEN);
	lstPopular.AddTail (ID_FILE_SAVE);
	lstPopular.AddTail (ID_EDIT_UNDO);
	
	// Hidden commands:
	CList<UINT,UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList (lstHidden, TRUE);
	
	// All commands:
	CList<UINT,UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList (lstAll);

	// Create "Customize QAT" page:
	LFRibbonCustomizeQATPage pageCustomizeQAT(&m_wndRibbonBar);
	
	pageCustomizeQAT.AddCustomCategory (_T("常用命令"), lstPopular);
	pageCustomizeQAT.AddCustomCategory (_T("未放置到功能区的命令"), lstHidden);
	pageCustomizeQAT.AddCustomCategory (_T("所有命令"), lstAll);
	
	// Create "Customize Ribbon" page:
	LFRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);
	
	pageCustomizeRibbon.AddCustomCategory (_T("常用命令"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory (_T("未放置到功能区的命令"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory (_T("所有命令"), lstAll);
	
	// Create property sheet:
	LFPropertySheet propSheet (_T("设置"), this, nPage);
	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.EnableLayout();
	
	propSheet.SetLook(LFPropertySheet::PropSheetLook_List, 150 /* List width */);
	
	propSheet.AddPage (&pageCustomizeRibbon);
	
	propSheet.AddPage (&pageCustomizeQAT);

	// TODO: add your option pages here:
	// COptionsPage1 pageOptions1;
	// propSheet.AddPage (&pageOptions1);
	//
	// COptionsPage1 pageOptions2;
	// propSheet.AddPage (&pageOptions2);
	
	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}

void CMainFrame::OnViewWorkspace() 
{
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}

void CMainFrame::OnViewWorkspace2() 
{
	ShowControlBar (&m_wndWorkSpace2,
					!(m_wndWorkSpace2.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace2.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}

 // WORKSPACEBAR
void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}
 // OUTPUTBAR

void CMainFrame::OnViewPropGrid() 
{
	ShowControlBar (&m_wndPropGrid,
					!(m_wndPropGrid.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewPropGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndPropGrid.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}
 // PROPERTYBAR
 // UI_TYPE_RIBBON

LRESULT CMainFrame::OnBeforeShowRibbonBackstageView(WPARAM, LPARAM)
{
	theApp.CreateScreenshot(m_bmpAppPreview);
	return 0;
}

