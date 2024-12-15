// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sdiviewswitch.h"
#include "Dib.h"
#include "windowsX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()
{
	m_pBmFileHeader=NULL;
	m_pBmInfo=NULL;
	m_pBmInfoHeader=NULL;
	m_pRGBTable=0;
	m_pDibBits=0;
}
CDib::CDib(const char* fileName)
{
	LoadBitmapFile(fileName);
}
CDib::~CDib()
{
	GlobalFreePtr(m_pBmInfo);
}
void CDib::LoadBitmapFile(const char* fileName)
{
	CFile file(fileName,CFile::modeRead);

	BITMAPFILEHEADER bmFileHeader;
	file.Read((void*)&bmFileHeader,sizeof(bmFileHeader));
	if(bmFileHeader.bfType!=0x4d42)
	{
		AfxMessageBox("Not a bitmap file");
	}
	else
	{
		DWORD	fileLength=file.GetLength();
		DWORD	dibSize=fileLength-sizeof(bmFileHeader);
		BYTE*	pDib=(BYTE*)GlobalAllocPtr(GMEM_MOVEABLE,dibSize);
		pDib=new BYTE[dibSize];
		file.Read(pDib,dibSize);
		file.Close();
		m_pBmInfo=(LPBITMAPINFO)pDib;
		m_pBmInfoHeader=(LPBITMAPINFOHEADER)pDib;
		m_pBmInfoHeader->biSizeImage=GetDibSizeImage();
		m_pDibBits=pDib+m_pBmInfoHeader->biSize;

	}
}
DWORD CDib::GetDibSizeImage()
{
	if(m_pBmInfoHeader->biSizeImage==0)
	{
		DWORD byteWidth=(DWORD)GetDibWidth();
		DWORD height=(DWORD)GetDibHeight();
		DWORD imageSize=byteWidth*height;
		return imageSize;
	}
	else
		return m_pBmInfoHeader->biSizeImage;
}
UINT CDib::GetDibWidth()
{
	return (UINT)m_pBmInfoHeader->biWidth;
}
UINT CDib::GetDibHeight()
{
	return (UINT)m_pBmInfoHeader->biHeight;
}
LPBITMAPINFOHEADER CDib::GetDibInfoHeaderPtr()
{
	return m_pBmInfoHeader;
}
LPBITMAPINFO CDib::GetDIbInfoPtr()
{
	return	m_pBmInfo;
}
BYTE* CDib::GetDibBitsPtr()
{
	return m_pDibBits;
}


