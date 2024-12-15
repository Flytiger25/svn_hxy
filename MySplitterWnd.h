#if !defined(AFX_MYSPLITTERWND_H__C50FD508_CEFA_11D1_9B01_0060970863EE__INCLUDED_)
#define AFX_MYSPLITTERWND_H__C50FD508_CEFA_11D1_9B01_0060970863EE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// MySplitterWnd.h

class CMySplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySplitterWnd)

// Implementation
public:
	CMySplitterWnd();
	~CMySplitterWnd();
	BOOL ReplaceView(CRuntimeClass * pViewClass);
};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MYSPLITTERWND_H__C50FD508_CEFA_11D1_9B01_0060970863EE__INCLUDED_)