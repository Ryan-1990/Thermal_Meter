
// TempDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <stdlib.h>
#include "Temp.h"
#include "TempDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define OL "L"
#define CL "l"
#define OS "S"
#define CS "s"
#define BT "B"
#define CT "b"

#define Normal RGB(93,207,66)
#define Error  RGB(240,88,41)

#define UpperLine  RGB(255,238,98)
#define LowerLine  RGB(240,88,41)


//��������
void UpdateUIThread(CTempDlg *p);		//��ȡ�����߳�
void UpdateTimer(CTempDlg *p);		//����ʱ���߳�
int OpenCom(CString id);
void writedata(CString data);
CString readdata();

double LowLimit = 20.0;
double HighLimit = 30.0;
double Max = 70.0;
double Min = 10.0;
int Hour = 0;
int Minute = 0;
int Second = 1;

int clickCnt = 0;
int MeasureFlag = 0;
CString CurrentTime = "";
CTime currentTime;
CTime previousTime;
CTime lastTime;
CTimeSpan span;
CString Data = "";//������������
CString value = "";
double Value = 0.0;
HANDLE hCom;//�򿪴��ھ��


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTempDlg �Ի���




CTempDlg::CTempDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTempDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	curSetValue = 1;
}

void CTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	DDX_Control(pDX, IDC_RADIO1, m_ctrlRadio1);
	DDX_Control(pDX, IDC_RADIO3, m_ctrlRadio3);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_SPIN1, m_SpinLow);
	DDX_Control(pDX, IDC_SPIN2, m_SpinHigh);
	DDX_Control(pDX, IDC_SPIN3, m_SpinHour);
	DDX_Control(pDX, IDC_SPIN4, m_SpinMinute);
	DDX_Control(pDX, IDC_SPIN5, m_SpinSecond);
	DDX_Control(pDX, IDC_SIGNAL, m_Signal1);
}

BEGIN_MESSAGE_MAP(CTempDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTempDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO1, &CTempDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTempDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CTempDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CTempDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON3, &CTempDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTempDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CTempDlg::OnBnClickedButton2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTempDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTempDlg::OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_LOW, &CTempDlg::OnEnChangeLow)
	ON_EN_CHANGE(IDC_HIGH, &CTempDlg::OnEnChangeHigh)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTempDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CTempDlg::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTempDlg::OnDeltaposSpin5)
	ON_EN_CHANGE(IDC_HOUR, &CTempDlg::OnEnChangeHour)
	ON_EN_CHANGE(IDC_MINUTE, &CTempDlg::OnEnChangeMinute)
	ON_EN_CHANGE(IDC_SECOND, &CTempDlg::OnEnChangeSecond)
END_MESSAGE_MAP()


// CTempDlg ��Ϣ�������

BOOL CTempDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	CRect rect;
	GetDlgItem(IDC_OSCOPE)->GetWindowRect(rect) ;
	ScreenToClient(rect) ;

	//create the control
	m_OScopeCtrl.Create(WS_VISIBLE | WS_CHILD, rect, this) ; 

	// customize the control
	m_OScopeCtrl.SetRange(Min, Max, 2) ;
	m_OScopeCtrl.SetYUnits("��") ;
	m_OScopeCtrl.SetXUnits("Time") ;
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_OScopeCtrl.SetAlertColor(LowerLine, UpperLine) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
	m_OScopeCtrl.SetPlotColor(RGB(255, 255, 255)) ;
	m_OScopeCtrl.SetAlert(LowLimit, HighLimit) ;


	CString limit;
	limit.Format("%.1f",LowLimit);
	SetDlgItemText(IDC_LOW,limit);
	limit.Format("%.1f",HighLimit);
	SetDlgItemText(IDC_HIGH,limit);
	limit.Format("%d",Hour);
	SetDlgItemText(IDC_HOUR,limit);
	limit.Format("%d",Minute);
	SetDlgItemText(IDC_MINUTE,limit);
	limit.Format("%d",Second);
	SetDlgItemText(IDC_SECOND,limit);

	m_SpinLow.SetBuddy(GetDlgItem(IDC_LOW)); 
	m_SpinLow.SetRange(Min,Max);
	m_SpinHigh.SetBuddy(GetDlgItem(IDC_HIGH)); 
	m_SpinHigh.SetRange(Min,Max);
	m_SpinHour.SetBuddy(GetDlgItem(IDC_HOUR)); 
	m_SpinHour.SetRange(0,24);
	m_SpinMinute.SetBuddy(GetDlgItem(IDC_MINUTE)); 
	m_SpinMinute.SetRange(0,59);
	m_SpinSecond.SetBuddy(GetDlgItem(IDC_SECOND)); 
	m_SpinSecond.SetRange(0,59);



	lastTime=currentTime=CTime::GetCurrentTime();
	CurrentTime=currentTime.Format("%Y��%m��%d��       %X");
	SetDlgItemText(IDC_TIME,CurrentTime);

	m_button2.EnableWindow(0);
	m_button4.EnableWindow(0);
	m_button3.EnableWindow(0);

	//��ֹ�û��ı�Ի����С
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED|SWP_DRAWFRAME);


	//����Ĭ�Ͽ���
	m_ctrlRadio1.SetCheck(1);
	m_ctrlRadio3.SetCheck(1);

	hThread_timer=CreateThread(NULL,  0,  (LPTHREAD_START_ROUTINE)UpdateTimer,  this,  0,  (unsigned long *)&ThreadID1);

	//ö�ٳ����п��õ�COM�˿ڣ���������ӵ���Ͽ���
	CArray<SSerInfo,SSerInfo&> asi;
	EnumSerialPorts(asi,FALSE);
	m_combo1.ResetContent();
	for (int ii=0; ii<asi.GetSize(); ii++) 
	{
		CString PortName;
		int index1,index2;

		PortName=asi[ii].strFriendlyName;
		index1=PortName.Find(_T("("));
		index2=PortName.Find(_T(")"));
		PortName = PortName.Mid(index1+1,index2-index1-1);
	    
		m_combo1.AddString(PortName);
	}
	m_combo1.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTempDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTempDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTempDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�򿪴���
int OpenCom(CString id)
{
	/*���ڵĳ�ʼ��*/
	CString str_com = id;
	
	hCom=CreateFile(str_com,GENERIC_READ|GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,0,NULL);   //�򿪴���
	if(hCom==(HANDLE)-1)
	{
		return 0;
	}
	else
	{
		SetupComm(hCom,100,100); //���뻺����������������Ĵ�С����100
		COMMTIMEOUTS TimeOuts;
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout=MAXDWORD;
		TimeOuts.ReadTotalTimeoutMultiplier=2;
		TimeOuts.ReadTotalTimeoutConstant=2;


		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier=100;
		TimeOuts.WriteTotalTimeoutConstant=500;
		SetCommTimeouts(hCom,&TimeOuts); //���ó�ʱ
		DCB dcb;
		GetCommState(hCom,&dcb);
		dcb.BaudRate=9600; //������Ϊ9600
		dcb.ByteSize=8; //ÿ���ֽ���8λ
		dcb.Parity=NOPARITY; //����żУ��λ
		dcb.StopBits=ONESTOPBIT ; //ֹͣλ
		SetCommState(hCom,&dcb);
		PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);
		return 1;
	}
}


void writedata(CString data)
{
	CStringA str_senddataA=data;
	DWORD dwBytesWrite=1;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(hCom,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(hCom,str_senddataA,dwBytesWrite,& 
		dwBytesWrite,NULL);
	if(!bWriteStat)
	{
		//MessageBox("д����ʧ��!");
	}
}

//������
CString readdata()
{
	char str[100];
	memset(str,'\0',100);
	DWORD wCount=100;//��ȡ���ֽ���
	BOOL bReadStat;
	
	bReadStat=ReadFile(hCom,str,wCount,&wCount,NULL);
	if(!bReadStat){
		//AfxMessageBox(L"������ʧ��!,���ȷ���رճ���");
		exit(0);
	}
	PurgeComm(hCom, PURGE_TXABORT|
	PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	return (CString)str;
}

void UpdateUIThread(CTempDlg *p)
{
	CString str="";
	CString CurTime="";
	CString previousStr="";
	
	while(1)
	{
		previousStr = str;
		str = readdata();
		PurgeComm(hCom,PURGE_RXCLEAR);
		if(strlen(str)>8) str = "";
		
		if(str != "")
		{
			if(str.Right(1)!="C") str = previousStr + str;
			else 
		    {
			    if(strlen(str)<7) str = previousStr + str;
				if(strlen(str)<7) writedata(BT);
				else
				{
					int index = str.Find("^");
					value = str.Left(index-1);
					Value = atof(value);
					p->m_OScopeCtrl.AppendPoint(Value);

					if(Value>HighLimit||Value<LowLimit)
					{
						CRect rect;
						p->m_Signal1.GetClientRect(&rect);
						FillRect(p->m_Signal1.GetDC()->GetSafeHdc(),&rect,CBrush(Error));
					}
					else
					{
						CRect rect;
						p->m_Signal1.GetClientRect(&rect);
						FillRect(p->m_Signal1.GetDC()->GetSafeHdc(),&rect,CBrush(Normal));
					}

					CurTime = currentTime.Format("%Y��%m��%d��  %X");
					str = CurTime + _T("   ") + str;
					p->m_list1.AddString(str);
					p->m_list1.SendMessage(WM_VSCROLL, SB_BOTTOM, NULL);  
					Data = Data + str + _T("\r\n");
				}
			}
		}
	}
}

void UpdateTimer(CTempDlg *p)
{
	CTimeSpan Span;

	while(1)
	{
		currentTime = CTime::GetCurrentTime();
		Span = currentTime - lastTime;

		if(Span.GetSeconds() == 1)
		{
			CurrentTime = currentTime.Format("%Y��%m��%d��       %X");
			p->SetDlgItemText(IDC_TIME,CurrentTime);
			lastTime = currentTime;

			if(MeasureFlag == 1)
			{
				span = currentTime - previousTime;
				if(span.GetTotalSeconds() >= p->curSetValue)
				{
					writedata(BT);
					previousTime = currentTime;
				}
			}
		}
	}
}

//�رնԻ���ʱִ�еĺ���
void CTempDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//timeKillEvent(timer_id1);
	if(clickCnt > 0) 
	{
		writedata(CL);
		CloseHandle(hThread_receiver);
		PurgeComm(hCom,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); //��ɾ����롢��������� 
	}
	CloseHandle(hThread_timer);
	CDialogEx::OnClose();
}


//���⿪
void CTempDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_list1.AddString("laser open");
	writedata(OL);
}

//�����
void CTempDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	writedata(CL);
}

//��Ļ��
void CTempDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	writedata(OS);
}

//��Ļ��
void CTempDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	writedata(CS);
}


//�����Ӵ��ڡ������������
void CTempDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡѡȡ��COM�˿ں�
	int CurrentIndex;
	CString CurrentPort;
	
	clickCnt = 1;
	CurrentIndex=m_combo1.GetCurSel();
	m_combo1.GetLBText(CurrentIndex,CurrentPort);
	CurrentPort=CurrentPort.Right(CurrentPort.GetLength()-3);
	//////////////////////////////////////////////////////////
	//�򿪻�ر���ѡ����
	int PortID;
	
	PortID=_tstoi(CurrentPort);
	CString ID;
	ID.Format(_T("COM%d"),PortID);

	if(OpenCom(ID)==1)
	{
		//�������̶߳�ȡ��������
		hThread_receiver=CreateThread(NULL,  0,  (LPTHREAD_START_ROUTINE)UpdateUIThread,  this,  0,  (unsigned long *)&ThreadID); 
		m_button1.EnableWindow(0);  //�����Ӵ��ڡ���ť����
		m_combo1.EnableWindow(0);  //
		m_button4.EnableWindow(0);
	    m_button3.EnableWindow(1);
	}
	else
	{
		MessageBox(_T("�˴��ڵ�ǰ�ѱ�ռ��"),_T("�Զ��¶ȼ����"),MB_ICONERROR|MB_OK);
	}
}

//����ʼ���¡������������
void CTempDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MeasureFlag = 1;
	currentTime=CTime::GetCurrentTime();
	previousTime = currentTime;
	m_button2.EnableWindow(0);
	m_button3.EnableWindow(0);
	m_button4.EnableWindow(1);
	writedata(OL);
	writedata(BT);
}

//���������¡������������
void CTempDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MeasureFlag = 0;
	writedata(CL);
	m_button2.EnableWindow(1);
	m_button4.EnableWindow(0);
	m_button3.EnableWindow(1);
}

//��������
void CTempDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog cfileDlg(false);//�ļ��Ի���  
    cfileDlg.m_ofn.lpstrDefExt="txt";  
    cfileDlg.m_ofn.lpstrTitle="�����¶�����";  
    cfileDlg.m_ofn.lpstrFilter="Text File(*.txt)\0*.txt\0All File(*.*)\0*.*\0\0";  
    //���ù���������һ��һ�Ե�ֵ��ÿ���ַ��������и�\0,������ʱ����Ҫ����\0  
  
    if(IDOK==cfileDlg.DoModal())  
    {  
        CFile cfile;
		if(cfile.Open(cfileDlg.GetPathName(),CFile::modeWrite) == 0)  
			cfile.Open(cfileDlg.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		cfile.SeekToEnd();
        cfile.Write(Data+"\r\n",strlen(Data+"\r\n"));  
        cfile.Close(); 
		m_list1.ResetContent();
		Data = "";
    }  
}

void CTempDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString low;
	GetDlgItemText(IDC_LOW,low);
	double Low = atof(low);

	if(pNMUpDown-> iDelta == 1) // �����ֵΪ1 , ˵�������Spin�����¼�ͷ 
	{  
		Low = Low + 1;
		if(Low > Max) Low = Max;
		CString NewLow;
		NewLow.Format("%.1f",Low);
		SetDlgItemText(IDC_LOW,NewLow);
	} 
	else if(pNMUpDown-> iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ 
	{ 
		Low = Low - 1;
		if(Low < Min) Low = Min;
		CString NewLow;
		NewLow.Format("%.1f",Low);
		SetDlgItemText(IDC_LOW,NewLow);
	}
	*pResult = 0;
}


void CTempDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString high;
	GetDlgItemText(IDC_HIGH,high);
	double High = atof(high);

	if(pNMUpDown-> iDelta == 1) // �����ֵΪ1 , ˵�������Spin�����¼�ͷ 
	{  
		High = High + 1;
		if(High > Max) High = Max;
		CString NewHigh;
		NewHigh.Format("%.1f",High);
		SetDlgItemText(IDC_HIGH,NewHigh);
	} 
	else if(pNMUpDown-> iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ 
	{ 
		High = High - 1;
		if(High < Min) High = Min;
		CString NewHigh;
		NewHigh.Format("%.1f",High);
		SetDlgItemText(IDC_HIGH,NewHigh);
	}
	*pResult = 0;
}


void CTempDlg::OnEnChangeLow()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString NewLow;
	GetDlgItemText(IDC_LOW,NewLow);
	LowLimit = atof(NewLow);
	m_OScopeCtrl.SetAlert(LowLimit, HighLimit) ;
}


void CTempDlg::OnEnChangeHigh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString NewHigh;
	GetDlgItemText(IDC_HIGH,NewHigh);
	HighLimit = atof(NewHigh);
	m_OScopeCtrl.SetAlert(LowLimit, HighLimit) ;
}

void CTempDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString hour;
	GetDlgItemText(IDC_HOUR,hour);
	int Hour = atoi(hour);

	if(pNMUpDown-> iDelta == 1) // �����ֵΪ1 , ˵�������Spin�����¼�ͷ 
	{  
		Hour = Hour + 1;
		if(Hour > 24) Hour = 24;
		CString NewHour;
		NewHour.Format("%d",Hour);
		SetDlgItemText(IDC_HOUR,NewHour);
	} 
	else if(pNMUpDown-> iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ 
	{ 
		Hour = Hour - 1;
		if(Hour < 0) Hour = 0;
		CString NewHour;
		NewHour.Format("%d",Hour);
		SetDlgItemText(IDC_HOUR,NewHour);
	}
	*pResult = 0;
}


void CTempDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString minute;
	GetDlgItemText(IDC_MINUTE,minute);
	int Minute = atoi(minute);

	if(pNMUpDown-> iDelta == 1) // �����ֵΪ1 , ˵�������Spin�����¼�ͷ 
	{  
		Minute = Minute + 1;
		if(Minute > 59) Minute = 59;
		CString NewMinute;
		NewMinute.Format("%d",Minute);
		SetDlgItemText(IDC_MINUTE,NewMinute);
	} 
	else if(pNMUpDown-> iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ 
	{ 
		Minute = Minute - 1;
		if(Minute < 0) Minute = 0;
		CString NewMinute;
		NewMinute.Format("%d",Minute);
		SetDlgItemText(IDC_MINUTE,NewMinute);
	}
	*pResult = 0;
}


void CTempDlg::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString second;
	GetDlgItemText(IDC_SECOND,second);
	int Second = atoi(second);

	if(pNMUpDown-> iDelta == 1) // �����ֵΪ1 , ˵�������Spin�����¼�ͷ 
	{  
		Second = Second + 1;
		if(Second > 59) Second = 59;
		CString NewSecond;
		NewSecond.Format("%d",Second);
		SetDlgItemText(IDC_SECOND,NewSecond);
	} 
	else if(pNMUpDown-> iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ 
	{ 
		Second = Second - 1;
		if(Second < 0) Second = 0;
		CString NewSecond;
		NewSecond.Format("%d",Second);
		SetDlgItemText(IDC_SECOND,NewSecond);
	}
	*pResult = 0;
}


void CTempDlg::OnEnChangeHour()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString NewHour;
	GetDlgItemText(IDC_HOUR,NewHour);
	Hour = atoi(NewHour);
	if(Hour == 0 && Minute == 0 && Second == 0)
		MessageBox(_T("�������Ϊ0ʱ0��0��"),_T("�Զ��¶ȼ����"),MB_ICONERROR|MB_OK);
	else curSetValue = Hour*3600+Minute*60+Second;
}


void CTempDlg::OnEnChangeMinute()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString NewMinute;
	GetDlgItemText(IDC_MINUTE,NewMinute);
	Minute = atoi(NewMinute);
	if(Hour == 0 && Minute == 0 && Second == 0)
		MessageBox(_T("�������Ϊ0ʱ0��0��"),_T("�Զ��¶ȼ����"),MB_ICONERROR|MB_OK);
	else curSetValue = Hour*3600+Minute*60+Second;
}


void CTempDlg::OnEnChangeSecond()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString NewSecond;
	GetDlgItemText(IDC_SECOND,NewSecond);
	Second = atoi(NewSecond);
	if(Hour == 0 && Minute == 0 && Second == 0)
		MessageBox(_T("�������Ϊ0ʱ0��0��"),_T("�Զ��¶ȼ����"),MB_ICONERROR|MB_OK);
	else curSetValue = Hour*3600+Minute*60+Second;
}
