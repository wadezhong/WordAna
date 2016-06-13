// WordAnaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WordAna.h"
#include "WordAnaDlg.h"

#include "TextSpliter.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWordAnaDlg �Ի���




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


// CWordAnaDlg ��Ϣ�������
static CString read_file_to_string( CString filepath ) 
{
    if (!PathFileExists(filepath))
        return _T("");
    CFile fFile(filepath,CFile::modeRead);  

    // ��Ϊ��Windows�³��õ��ı��������ΪANSI  
    // ���������ڵ���ѡ���ʱ��ѡ����Unicode����  
    // ������Ҫʹ��CStringA���͵ı�����  
    // CStringA������⣺http://baike.baidu.com/view/998109.htm  
    CStringA strFile ;  

    fFile.Read(strFile.GetBufferSetLength(fFile.GetLength()),fFile.GetLength());  
    fFile.Close();  
    strFile.ReleaseBuffer();  

    // �򻷾�ΪUnicdoe��ԭ��CStringA������ֱ����ʾ������ؼ���  
    // ����ʹ��CStringת��ΪUnicdoe�ַ�  
    return CString(strFile);
}
static void write_string_to_file( CString filepath, CString str ) 
{
    CFile fFile;  
    fFile.Open(filepath,CFile::modeCreate|CFile::modeWrite);

    //��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
    int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
    //Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
    char * pFileName = new char[len+1];   //���ֽ�Ϊ��λ
    //���ֽڱ���ת���ɶ��ֽڱ���
    WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);
    WideCharToMultiByte(CP_ACP,0,str,str.GetLength() + 1 ,pFileName,len + 1 ,NULL,NULL);
    pFileName[len] = 0;   //���ֽ��ַ���'/0'����

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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    GetDlgItem(IDC_EDIT_KNOWN_WORDS_FILE)->SetWindowText(GetLastConfig());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWordAnaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWordAnaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWordAnaDlg::OnBnClickedButton1()
{
  CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
        _TEXT("�ı��ļ�(*.*)|*.*|ȫ���ļ�(*.*)|*.*||"));  
  
    // ���� DoModal() �������򿪶Ի���  
    if (fDlg.DoModal() == IDOK)  
    {  
        // ���ñ༭��1������Ϊѡ���ļ���·��  
        // GetPathName() --> ����ѡ���ļ�������·��  
        SetDlgItemText(IDC_EDIT1,fDlg.GetPathName());  

    }
}




////////////////////////////////////////////////////////////////////////////////////////
void CWordAnaDlg::OnBnClickedButtonKnowWords()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
        _TEXT("�ı��ļ�(*.txt)|*.txt|ȫ���ļ�(*.*)|*.*||"));  

    // ���� DoModal() �������򿪶Ի���  
    if (fDlg.DoModal() == IDOK)  
    {  
        // ���ñ༭��1������Ϊѡ���ļ���·��  
        // GetPathName() --> ����ѡ���ļ�������·��  
        SetDlgItemText(IDC_EDIT_KNOWN_WORDS_FILE,fDlg.GetPathName());  
        WriteConfig(fDlg.GetPathName());
    }
}

CString GetFileTitleFromFileName(CString FileName, CString & FilePath, CString & FileTitle, CString& ExtendName) //��ȡ�ļ�������չ��  
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
        AfxMessageBox(_T("��ѡ��Ҫ������ı�"));
        return;
    }

    CTextSpliter  spliter;
    spliter.spliter_split_process(file_to_process, compose_result_file_path(file_to_process,_T("_splitwords"),_T(".txt")));

    CString file_know_words;
    GetDlgItem(IDC_EDIT_KNOWN_WORDS_FILE)->GetWindowText(file_know_words);
    if(file_know_words.GetLength() == 0)
        return;
    spliter.knower_new_words_process(file_know_words,compose_result_file_path(file_to_process,_T("_newwords"),_T(".txt")),spliter.WordsList);

    AfxMessageBox(_T("������ɣ��뵽ԭ�ļ�����Ŀ¼�����´ʣ�"));
}
