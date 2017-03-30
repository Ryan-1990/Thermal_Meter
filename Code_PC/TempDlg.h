
// TempDlg.h : 头文件
//



#pragma once


#include "OScopeCtrl.h"

#include "afxwin.h"
#include "EnumSerial.h"
#include <stdio.h>
#include "mmsystem.h"

#pragma comment( lib, "Winmm.lib" )
#pragma comment( lib, "setupapi.lib" )



// CTempDlg 对话框
class CTempDlg : public CDialogEx
{
// 构造
public:
	CTempDlg(CWnd* pParent = NULL);	// 标准构造函数

	int RecChar; //存储单片机发回的字符
	int curSetValue;

// 对话框数据
	enum { IDD = IDD_TEMP_DIALOG };
	
	COScopeCtrl m_OScopeCtrl;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void Timer1();
	HANDLE hThread_timer;
	HANDLE hThread_receiver;
	DWORD ThreadID;
	DWORD ThreadID1;
	afx_msg void OnBnClickedButton1();
	CButton m_button1;
	CComboBox m_combo1;
	afx_msg void OnClose();
	CButton m_ctrlRadio1;
	CButton m_ctrlRadio3;
	afx_msg void OnBnClickedRadio1();
	CListBox m_list1;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton m_button3;
	CButton m_button4;
	afx_msg void OnBnClickedButton2();
	CButton m_button2;
	CSpinButtonCtrl m_SpinLow;
	CSpinButtonCtrl m_SpinHigh;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeLow();
	afx_msg void OnEnChangeHigh();
	CStatic m_Signal1;
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeHour();
	afx_msg void OnEnChangeMinute();
	afx_msg void OnEnChangeSecond();
	CSpinButtonCtrl m_SpinHour;
	CSpinButtonCtrl m_SpinMinute;
	CSpinButtonCtrl m_SpinSecond;
};
