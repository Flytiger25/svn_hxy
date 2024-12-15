// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CB97F4C3_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
#define AFX_STDAFX_H__CB97F4C3_1752_11D2_B134_00C04FB9CA2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
double max(double a, double b);
double min(double a, double b);
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glu.h>
//#include <afxdhtml.h>
//#include <GL\glaux.h>
#include <vector>
#include "Point3D.h"
//#include <afxdlgs.h>
class CFormView3;
#define TOLER 1e-8
#define ZeroP2D Point2D(0.0, 0.0)
#define ZeroP3D Point3D(0.0, 0.0, 0.0)
typedef double* PTRDOUBLE;
#define CS_EPS 1e-8
double SQR(double x);

double Factorial(double n, double i);
class CSDIViewSwitchDoc;
extern int m_SetCameraMode;
//#define GPU_ACCELERATE
std::vector<double> operator + (const std::vector<double> &v1,const std::vector<double> &v2);
std::vector<double> &operator += (std::vector<double> &v1,const std::vector<double> &v2);
std::vector<double> operator / (const std::vector<double> &v1,double d);
std::vector<double> operator * (const std::vector<double> &v1,double d);
std::vector<double> operator * (double d,const std::vector<double> &v1);
Vector3D operator * (double num, Vector3D v) ;
extern CFormView3 *p_FormView3;

class CNurbsSuface;

enum reparatype{ none,mobius1,compositemobius,bilinear,quadratic,generalbilinear,Bspline};
#include "resource.h"
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxctl.h>

extern HWND g_hwndMain;
extern CWinThread* g_pThread;
extern CSDIViewSwitchDoc* pDoc;

#define WM_MESSAGE				WM_USER+50
#define WM_UPDATE_TREECONTROL   WM_USER+51
#define WM_SETAREA              WM_USER+52
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CB97F4C3_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
