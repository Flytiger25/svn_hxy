// SDIViewSwitch.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "MainFrm.h"
#include "SDIViewSwitchDoc.h"
#include "SplitterView.h"
#include "Glut.h"
#include <SPAIArgumentParsing.h>
#include <kernapi.hxx>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDIViewSwitchApp

BEGIN_MESSAGE_MAP(CSDIViewSwitchApp, CWinApp)
	//{{AFX_MSG_MAP(CSDIViewSwitchApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CSDIViewSwitchApp::CSDIViewSwitchApp()
{
	m_nNumberOfInitialRecords = 7;
	m_pDoc = NULL;
}
/////////////////////////////////////////////////////////////////////////////
CSDIViewSwitchApp::~CSDIViewSwitchApp()
{
	api_stop_modeller();
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CSDIViewSwitchApp object
CSDIViewSwitchApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSDIViewSwitchApp initialization
BOOL CSDIViewSwitchApp::InitInstance()
{
	//glut 初始化
	int num = 1;
	char* name[1] = { "ssd" };
	glutInit(&num, name);

	//start the acis engine
	unlock_license();

	//// 启动ACIS模型处理器
	api_start_modeller(0);

	SetDialogBkColor(RGB(255, 255, 255), RGB(0, 0, 0));

	//Set the registry key under which our settings are stored.
	SetRegistryKey(_T("Geometric Engine"));

	LoadStdProfileSettings();  //Load standard INI file options (including MRU)

	// Register document templates
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSDIViewSwitchDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSplitterView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////
//When this function is called, either the app is just starting and it is called from 
//ProcessShellCommand(), or the user has selected File|New. By default, the MFC framework
//does not create a file for a new document until the user saves the document. Since  
//this app updates the file on a transaction basis, a file is required at the start:

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
		//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSDIViewSwitchApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////
