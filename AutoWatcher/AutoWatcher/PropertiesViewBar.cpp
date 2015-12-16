// PropertiesViewBar.cpp : implementation of the LFPropBar class
//

#include "stdafx.h"
#include "AutoWatcher.h"
#include "PropertiesViewBar.h"
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// LFPropBar class

LFPropBar::LFPropBar()
{
	m_BorderColor = LFColor::Orange;
	m_FillBrush.SetColors(LFColor::LightSteelBlue, LFColor::White, LFBrush::BCGP_GRADIENT_RADIAL_TOP_LEFT, 0.75);
	m_TextBrush.SetColor(LFColor::SteelBlue);
}

LFPropBar::~LFPropBar()
{
}

BEGIN_MESSAGE_MAP(LFPropBar, LFDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_COMMAND_CLICKED, OnCommandClicked)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_MENU_ITEM_SELECTED, OnMenuItemSelected)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void LFPropBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width (), rectClient.Height (), SWP_NOACTIVATE | SWP_NOZORDER);
}

int LFPropBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (LFDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndPropList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("创建属性窗口失败 \n");
		return -1;      // fail to create
	}

	InitPropList ();
	AdjustLayout ();

	return 0;
}

void LFPropBar::OnSize(UINT nType, int cx, int cy) 
{
	LFDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout ();
}

void LFPropBar::InitPropList ()
{
	// Add commands:
	CStringList lstCommands;
	lstCommands.AddTail (_T("命令 1..."));
	lstCommands.AddTail (_T("命令 2..."));

	m_wndPropList.SetCommands (lstCommands);

	// Add custom menu items:
	CStringList lstCustomMenuItem;
	lstCustomMenuItem.AddTail (_T("自定义菜单项 1"));
	lstCustomMenuItem.AddTail (_T("自定义菜单项 2"));
	lstCustomMenuItem.AddTail (_T("自定义菜单项 3"));

	m_wndPropList.SetCustomMenuItems(lstCustomMenuItem);

	// Setup general look:
	m_wndPropList.EnableToolBar();
	m_wndPropList.EnableSearchBox();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.EnableContextMenu();

	// Add properties:
	LFProp* pGroup1 = new LFProp (_T("外观"));

	pGroup1->AddSubItem (new LFProp (_T("三维边框"), (_variant_t) false,
		_T("设置对话框的字体为非黑体，控件呈现三维样式")));

	LFProp* pProp = new LFProp (_T("边框"), _T("对话框"),
		_T("无边框, 瘦边框, 可变边框, 或者对话框边框中之一"));
	pProp->AddOption (_T("无边框"));
	pProp->AddOption (_T("瘦边框"));
	pProp->AddOption (_T("可变边框"));
	pProp->AddOption (_T("对话框边框"));
	pProp->AllowEdit (FALSE);

	pGroup1->AddSubItem (pProp);
	pGroup1->AddSubItem (new LFProp (_T("标题"), (_variant_t) _T("关于"),
		_T("设置在对话框标题栏中显示的文本")));

	m_wndPropList.AddProperty (pGroup1);

	LFProp* pSize = new LFProp (_T("窗口大小"), 0, TRUE);

	pProp = new LFProp (_T("高度"), (_variant_t) 250l,
							_T("设置对话框高度"));
	pProp->EnableSpinControl (TRUE, 0, 1000);
	pSize->AddSubItem (pProp);

	pProp = new LFProp (	_T("宽度"), (_variant_t) 150l,
							_T("设置对话框的宽度"));
	pProp->EnableSpinControl (TRUE, 0, 1000);
	pSize->AddSubItem (pProp);

	m_wndPropList.AddProperty (pSize);

	LFProp* pGroup2 = new LFProp (_T("字体"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle ((HFONT) GetStockObject (DEFAULT_GUI_FONT));
	font->GetLogFont (&lf);

	lstrcpy (lf.lfFaceName, _T("宋体"));

	pGroup2->AddSubItem (new LFFontProp (_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("设置对话框默认字体")));
	pGroup2->AddSubItem (new LFProp (_T("使用系统字体"), (_variant_t) true, _T("设置对话字体为宋体")));

	m_wndPropList.AddProperty (pGroup2);

	LFProp* pGroup3 = new LFProp (_T("画刷"));

	LFColorProp* pColorProp = new LFColorProp (_T("边颜色"), m_BorderColor, NULL, _T("设置对话框默认颜色"));
	pColorProp->EnableOtherButton (_T("其他..."));
	pColorProp->EnableAutomaticButton (_T("默认"), globalData.clrBarShadow);
	pGroup3->AddSubItem (pColorProp);

	pGroup3->AddSubItem (new LFBrushProp (_T("背景"), m_FillBrush, NULL,
		_T("设置默认背景色")));

	pGroup3->AddSubItem (new LFBrushProp (_T("前景"), m_TextBrush, NULL,
		_T("设置默认前景色")));

	m_wndPropList.AddProperty (pGroup3);

	LFProp* pGroup4 = new LFProp (_T("其他"));
	pProp = new LFProp (_T("(名称)"), _T("IDD_ABOUT_BOX (对话框)"));
	pProp->Enable (FALSE);
	pGroup4->AddSubItem (pProp);

	static TCHAR BASED_CODE szFilter[] = _T("图标文件 (*.ico)|*.ico|所有文件 (*.*)|*.*||");
	pGroup4->AddSubItem (new LFFileProp (_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the dialog icon")));

	pGroup4->AddSubItem (new LFFileProp (_T("文件夹"), _T("c:\\")));

	COleDateTime date = COleDateTime::GetCurrentTime ();
	pGroup4->AddSubItem (new LFDateTimeProp (_T("日期"), date,
		_T("Set a date"), 0, LFDateTimeCtrl::DTM_DATE));

	pGroup4->AddSubItem (new LFDateTimeProp (_T("时间"), date,
		_T("Set a time"), 0, LFDateTimeCtrl::DTM_TIME));

	m_wndPropList.AddProperty (pGroup4);

	LFProp* pGroup5 = new LFProp (_T("水平"));

	LFProp* pGroup51 = new LFProp (_T("第一水平"));
	pGroup5->AddSubItem (pGroup51);

	LFProp* pGroup511 = new LFProp (_T("第二水平"));
	pGroup51->AddSubItem (pGroup511);

	pGroup511->AddSubItem (new LFProp (_T("项目 1"), (_variant_t) _T("数值 1"),
		_T("This is a description")));
	pGroup511->AddSubItem (new LFProp (_T("项目 2"), (_variant_t) _T("数值 2"),
		_T("This is a description")));
	pGroup511->AddSubItem (new LFProp (_T("项目 3"), (_variant_t) _T("数值 3"),
		_T("This is a description")));

	pGroup5->Expand (FALSE);
	m_wndPropList.AddProperty (pGroup5);
}

void LFPropBar::OnSetFocus(CWnd* pOldWnd) 
{
	LFDockingControlBar::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus ();
}

LRESULT LFPropBar::OnCommandClicked(WPARAM, LPARAM lp)
{
//	int nCommandIndex = (int)lp;

	// TODO: Add your command handler code here
	std::cout << "paing" << std::endl;

	return 0;
}

LRESULT LFPropBar::OnMenuItemSelected(WPARAM /*wp*/, LPARAM /*lp*/)
{
//	int nMenuIndex = (int)wp;

//	LFProp* pProp = (LFProp*)lp;
//	ASSERT_VALID(pProp);

	// TODO: Add your menu item processing code here

	return 0;
}
