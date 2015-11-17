// AutoWatcher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AutoWatcher.h"
#include "MainFrm.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoWatcherApp

BEGIN_MESSAGE_MAP(CAutoWatcherApp, LFWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
END_MESSAGE_MAP()


// CAutoWatcherApp construction

CAutoWatcherApp::CAutoWatcherApp()
{
	AddVisualTheme(BCGP_VISUAL_THEME_SCENIC, ID_VIEW_APPLOOK_SCENIC);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_LIGHT, ID_VIEW_APPLOOK_VS2013_LIGHT);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_DARK, ID_VIEW_APPLOOK_VS2013_DARK);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_BLUE, ID_VIEW_APPLOOK_VS2013_BLUE);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_COLORFUL, ID_VIEW_APPLOOK_2016_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_DARK_GRAY, ID_VIEW_APPLOOK_2016_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_WHITE, ID_VIEW_APPLOOK_2016_3);

	SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_DARK_GRAY);


	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAutoWatcherApp object

CAutoWatcherApp theApp;


// CAutoWatcherApp initialization

BOOL CAutoWatcherApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	LFWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("LP向导程序"));

	SetRegistryBase(_T("设置"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	LFMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_AutoWatcherTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_AutoWatcherTYPE));





	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

// CAutoWatcherApp message handlers

int CAutoWatcherApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return LFWinApp::ExitInstance();
}
void CAutoWatcherApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_AutoWatcherTYPE, m_hMDIMenu, m_hMDIAccel);
}

// CAboutDlg dialog used for App About

class CAboutDlg : public LFDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	LFURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : LFDialog(CAboutDlg::IDD)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	LFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, LFDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CAutoWatcherApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CAutoWatcherApp message handlers


void CAutoWatcherApp::PreLoadState()
{
}

BOOL CAutoWatcherApp::SaveAllModified()
{
	if (!LFWinApp::SaveAllModified())
	{
		return FALSE;
	}
	
	LFMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST(LFMDIFrameWnd, m_pMainWnd);
	if (pMainFrame != NULL)
	{
		pMainFrame->SaveMDIState(GetRegSectionPath());
	}
	
	return TRUE;
}
