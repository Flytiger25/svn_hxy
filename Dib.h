// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__866B0261_0624_41AC_9D96_50335ADD9486__INCLUDED_)
#define AFX_DIB_H__866B0261_0624_41AC_9D96_50335ADD9486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDib : public CObject  
{
public:
	BYTE*    GetDibBitsPtr();
	LPBITMAPINFO GetDIbInfoPtr();
	LPBITMAPINFOHEADER GetDibInfoHeaderPtr();
	UINT	GetDibHeight();
	UINT	GetDibWidth();
	DWORD	GetDibSizeImage();
	CDib();
	CDib(const char* fileName);
	virtual ~CDib();
protected:
	LPBITMAPFILEHEADER	m_pBmFileHeader;
	LPBITMAPINFO		m_pBmInfo;
	LPBITMAPINFOHEADER	m_pBmInfoHeader;
	RGBQUAD*			m_pRGBTable;
	BYTE*				m_pDibBits;
	UINT				m_numColors;
	void	LoadBitmapFile(const char* fileName);
};

#endif // !defined(AFX_DIB_H__866B0261_0624_41AC_9D96_50335ADD9486__INCLUDED_)
