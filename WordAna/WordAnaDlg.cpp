// WordAnaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WordAna.h"
#include "WordAnaDlg.h"

#include "TextSpliter.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWordAnaDlg 对话框




CWordAnaDlg::CWordAnaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWordAnaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWordAnaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWordAnaDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, &CWordAnaDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON_KNOW_WORDS, &CWordAnaDlg::OnBnClickedButtonKnowWords)
    ON_BN_CLICKED(IDC_BUTTON_START, &CWordAnaDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CWordAnaDlg 消息处理程序
static CString read_file_to_string( CString filepath ) 
{
    if (!PathFileExists(filepath))
        return _T("");
    CFile fFile(filepath,CFile::modeRead);  

    // 因为在Windows下常用的文本保存编码为ANSI  
    // 而本工程在当初选择的时候选择了Unicode环境  
    // 所以需要使用CStringA类型的变量。  
    // CStringA具体详解：http://baike.baidu.com/view/998109.htm  
    CStringA strFile ;  

    fFile.Read(strFile.GetBufferSetLength(fFile.GetLength()),fFile.GetLength());  
    fFile.Close();  
    strFile.ReleaseBuffer();  

    // 因环境为Unicdoe的原因，CStringA不可以直接显示到界面控件上  
    // 所以使用CString转换为Unicdoe字符  
    return CString(strFile);
}
static void write_string_to_file( CString filepath, CString str ) 
{
    CFile fFile;  
    fFile.Open(filepath,CFile::modeCreate|CFile::modeWrite);

    //获取宽字节字符的大小，大小是按字节计算的
    int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
    //为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
    char * pFileName = new char[len+1];   //以字节为单位
    //宽字节编码转换成多字节编码
    WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);
    WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);
    pFileName[len] = 0;   //多字节字符以'/0'结束

    fFile.Write(pFileName,len+1);
    
    delete pFileName;
    fFile.Close();  
}

static CString ReturnCurPath()  
{   
    CString    sPath;   
    GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
    sPath.ReleaseBuffer    ();   
    int    nPos;   
    nPos=sPath.ReverseFind('\\');   
    sPath=sPath.Left(nPos+1);   
    return    sPath;   
}

static CString GetIniFilePath()
{
    return ReturnCurPath() + _T("WordAna.ini");
}

static CString GetLastConfig()
{
    return read_file_to_string(GetIniFilePath());
}

static void WriteConfig(CString KnownWordsFilePath)
{
    write_string_to_file(GetIniFilePath(),KnownWordsFilePath);
}

BOOL CWordAnaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

    GetDlgItem(IDC_EDIT_KNOWN_WORDS_FILE)->SetWindowText(GetLastConfig());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWordAnaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWordAnaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWordAnaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWordAnaDlg::OnBnClickedButton1()
{
  CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
        _TEXT("文本文件(*.*)|*.*|全部文件(*.*)|*.*||"));  
  
    // 调用 DoModal() 方法来打开对话框  
    if (fDlg.DoModal() == IDOK)  
    {  
        // 设置编辑框1的内容为选择文件的路径  
        // GetPathName() --> 返回选定文件的完整路径  
        SetDlgItemText(IDC_EDIT1,fDlg.GetPathName());  

    }
}




////////////////////////////////////////////////////////////////////////////////////////
void CWordAnaDlg::OnBnClickedButtonKnowWords()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
        _TEXT("文本文件(*.txt)|*.txt|全部文件(*.*)|*.*||"));  

    // 调用 DoModal() 方法来打开对话框  
    if (fDlg.DoModal() == IDOK)  
    {  
        // 设置编辑框1的内容为选择文件的路径  
        // GetPathName() --> 返回选定文件的完整路径  
        SetDlgItemText(IDC_EDIT_KNOWN_WORDS_FILE,fDlg.GetPathName());  
        WriteConfig(fDlg.GetPathName());
    }
}

CString GetFileTitleFromFileName(CString FileName, CString & FilePath, CString & FileTitle, CString& ExtendName) //获取文件名和扩展名  
{  
    FilePath   = _T("");
    FileTitle  = _T("");
    ExtendName = _T("");
    int Where;  
    Where=FileName.ReverseFind(_T('\\'));  
    if(Where==-1)  
    {  
        Where=FileName.ReverseFind(_T('/'));  
    }  
    FilePath =FileName.Left(Where+1);
    FileTitle=FileName.Right(FileName.GetLength()-1-Where);  
    int Which=FileTitle.ReverseFind('.');  
    ExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);  
    if (Which!=-1)  
    {  
        FileTitle=FileTitle.Left(Which);  
    }  
    return FileTitle;  
}  

CString compose_result_file_path(CString file_input, CString suffix, CString ext2use)
{
    CString FilePath;
    CString FileTitle;
    CString ExtendName;
    GetFileTitleFromFileName(file_input, FilePath, FileTitle, ExtendName);
    return FilePath+FileTitle+suffix+ext2use;
}

void CWordAnaDlg::OnBnClickedButtonStart()
{
    CString file_to_process;
    GetDlgItem(IDC_EDIT1)->GetWindowText(file_to_process);
    if(file_to_process.GetLength() == 0)
    {
        AfxMessageBox(_T("请选择要处理的文本"));
        return;
    }

    CTextSpliter  spliter;
    spliter.spliter_split_process(file_to_process, compose_result_file_path(file_to_process,_T("_splitwords"),_T(".txt")));

    CString file_know_words;
    GetDlgItem(IDC_EDIT_KNOWN_WORDS_FILE)->GetWindowText(file_know_words);
    if(file_know_words.GetLength() == 0)
        return;
    spliter.knower_new_words_process(file_know_words,compose_result_file_path(file_to_process,_T("_newwords"),_T(".txt")),spliter.WordsList);

    AfxMessageBox(_T("处理完成，请到原文件所在目录查找新词！"));
}
