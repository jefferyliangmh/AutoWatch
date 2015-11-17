// PropertiesViewBar.h : interface of the LPPropBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class LFPropBar : public LFDockingControlBar  
{
// Construction
public:
	LFPropBar();

	void AdjustLayout ();

// Attributes
public:
	void SetVSDotNetLook (BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook (bSet);
		m_wndPropList.SetGroupNameFullWidth (bSet);
	}

protected:
	LFPropList	m_wndPropList;

	LFColor		m_BorderColor;
	LFBrush		m_FillBrush;
	LFBrush		m_TextBrush;

// Attributes
protected:

// Overrides

// Implementation
public:
	virtual ~LFPropBar();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnCommandClicked(WPARAM, LPARAM);
	afx_msg LRESULT OnMenuItemSelected(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	void InitPropList ();
};
