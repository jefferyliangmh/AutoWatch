// BackStagePageInfo.cpp : implementation file
//

#include "stdafx.h"
#include "AutoWatcher.h"
#include "MainFrm.h"
#include "BackStagePageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackStagePageInfo dialog

IMPLEMENT_DYNCREATE(CBackStagePageInfo, LFDialog)

CBackStagePageInfo::CBackStagePageInfo(CWnd* pParent /*=NULL*/)
	: LFDialog(CBackStagePageInfo::IDD, pParent)
{
	EnableLayout();
}


void CBackStagePageInfo::DoDataExchange(CDataExchange* pDX)
{
	LFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW, m_btnPreview);
}


BEGIN_MESSAGE_MAP(CBackStagePageInfo, LFDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackStagePageInfo message handlers

BOOL CBackStagePageInfo::OnInitDialog() 
{
	LFDialog::OnInitDialog();

	LFStaticLayout* pLayout = (LFStaticLayout*)GetLayout();
	ASSERT_VALID(pLayout);
	
	pLayout->AddAnchor(IDC_PATH_LABEL, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	
	pLayout->AddAnchor(IDC_SEPARATOR_1, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	pLayout->AddAnchor(IDC_TEXT1, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	
	pLayout->AddAnchor(IDC_SEPARATOR_2, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	pLayout->AddAnchor(IDC_TEXT2, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	
	pLayout->AddAnchor(IDC_SEPARATOR_3, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	pLayout->AddAnchor(IDC_TEXT3, LFStaticLayout::e_MoveTypeNone, LFStaticLayout::e_SizeTypeHorz);
	
	pLayout->AddAnchor(IDC_SEPARATOR_4, LFStaticLayout::e_MoveTypeHorz, LFStaticLayout::e_SizeTypeVert);
	
	pLayout->AddAnchor(IDC_INFO_LABEL, LFStaticLayout::e_MoveTypeHorz, LFStaticLayout::e_SizeTypeNone);
	pLayout->AddAnchor(IDC_SEPARATOR_5, LFStaticLayout::e_MoveTypeHorz, LFStaticLayout::e_SizeTypeNone);
	pLayout->AddAnchor(IDC_PREVIEW, LFStaticLayout::e_MoveTypeHorz, LFStaticLayout::e_SizeTypeNone);
	pLayout->AddAnchor(IDC_TEXT4, LFStaticLayout::e_MoveTypeHorz, LFStaticLayout::e_SizeTypeNone);

	CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();
	
	CFrameWnd* pFrame = pMainFrame->GetActiveFrame();
	if (pFrame != NULL)
	{
		CDocument* pDoc = pFrame->GetActiveDocument();
		if (pDoc != NULL)
		{
			m_strDocName = pDoc->GetTitle();
			m_strPath = pDoc->GetPathName();
		}
	}

	PreparePreviewBitmap();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBackStagePageInfo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	OnDraw(&dc);
}

void CBackStagePageInfo::OnDraw(CDC* pDC)
{
	CRect rectPath;

	GetDlgItem(IDC_PATH_LABEL)->GetWindowRect(rectPath);
	ScreenToClient(rectPath);

	CRect rectInfo;

	GetDlgItem(IDC_INFO_LABEL)->GetWindowRect(rectInfo);
	ScreenToClient(rectInfo);

	CFont* pOldFont = pDC->SelectObject(&globalData.fontCaption);
	ASSERT_VALID(pOldFont);

	pDC->SetTextColor(LFVisualManager::GetInstance()->GetRibbonBackstageInfoTextColor());
	pDC->SetBkMode(TRANSPARENT);
	
	CString strInfo = _T("пео╒");
	pDC->DrawText(strInfo, rectInfo, DT_SINGLELINE | DT_END_ELLIPSIS);
	
	int nTextHeight = pDC->DrawText(m_strDocName, rectPath, DT_SINGLELINE | DT_END_ELLIPSIS);
	
	rectPath.top += nTextHeight + 5;
	
	pDC->SelectObject(&globalData.fontRegular);
	
	pDC->DrawText(m_strPath, rectPath, DT_SINGLELINE | DT_END_ELLIPSIS);
	
	pDC->SelectObject(pOldFont);
}

void CBackStagePageInfo::PreparePreviewBitmap()
{
	CRect rectPreview;
	m_btnPreview.GetClientRect(rectPreview);
	
	CBitmap& bmpAppPreview = ((CMainFrame*)AfxGetMainWnd())->GetAppPreview();
	
	BITMAP bmp;
	bmpAppPreview.GetBitmap(&bmp);
	
	double dblScaleRatio = min((double)rectPreview.Width() / bmp.bmWidth, (double)rectPreview.Height() / bmp.bmHeight);
	
	CSize szDst((int)(bmp.bmWidth * dblScaleRatio), (int)(bmp.bmHeight * dblScaleRatio));
	
	HBITMAP hBitmap = LFDrawManager::CreateBitmap_24(szDst, NULL);
	
	{
		CDC dcDst;
		dcDst.CreateCompatibleDC(NULL);
		
		HBITMAP hBitmapOld = (HBITMAP)dcDst.SelectObject (hBitmap);
		
		LFDrawManager dm(dcDst);
		int nShadowSize = 5;
		
		szDst.cx -= 2 * nShadowSize;
		szDst.cy -= 2 * nShadowSize;
		
		LFVisualManager::GetInstance()->OnFillRibbonBackstageForm(&dcDst, this, rectPreview);
		dm.DrawShadow(CRect(CPoint(nShadowSize, nShadowSize), szDst), nShadowSize);
		
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(&dcDst);
		
		HBITMAP hBitmapViewOld = (HBITMAP)dcSrc.SelectObject(bmpAppPreview.GetSafeHandle());
		
		dcDst.SetStretchBltMode(HALFTONE);
		
		dcDst.StretchBlt(nShadowSize, nShadowSize, szDst.cx, szDst.cy, &dcSrc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		
		dcSrc.SelectObject (hBitmapViewOld);
		dcDst.SelectObject (hBitmapOld);
	}
	
	m_btnPreview.SetMouseCursorHand();
	m_btnPreview.SetDrawText(FALSE);
	m_btnPreview.m_bDrawFocus = FALSE;
	m_btnPreview.SetImage(hBitmap);
	m_btnPreview.m_bTransparent = TRUE;
	m_btnPreview.m_nFlatStyle = LFButton::BUTTONSTYLE_NOBORDERS;
	m_btnPreview.SizeToContent();
	
	bmpAppPreview.DeleteObject();
}

LRESULT CBackStagePageInfo::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if (lp & PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

void CBackStagePageInfo::OnPreview() 
{
	GetParent()->SendMessage(WM_CLOSE);
}

