// outputbar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class COutputBar : public LFDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:
	LFTabWnd	_wndTabs;
	
	LFListCtrl	_wndList1;
	LFListCtrl	_wndList2;
	LFListCtrl	_wndList3;

// Operations
public:

// Overrides

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
