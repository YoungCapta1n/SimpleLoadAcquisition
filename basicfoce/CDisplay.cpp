// CDisplay.cpp: 实现文件
//

#include "stdafx.h"
#include "basicfoce.h"
#include "CDisplay.h"
#include "afxdialogex.h"


// CDisplay 对话框

IMPLEMENT_DYNAMIC(CDisplay, CDialogEx)

CDisplay::CDisplay(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIADISPLAY, pParent)
{

}

CDisplay::~CDisplay()
{
}

void CDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHANNELVALUE, m_listchannelvalue);
}


BEGIN_MESSAGE_MAP(CDisplay, CDialogEx)
END_MESSAGE_MAP()


// CDisplay 消息处理程序
BOOL CDisplay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitializeLIST(m_listchannelvalue);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDisplay::InitializeLIST(CReportCtrl &m_list) {

	// Initilize the channel list
	m_list.DeleteAllItems();
	// m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);
	m_list.SetExtendedStyle(LVS_EX_DOUBLEBUFFER);
	// m_list.ModifyStyle(LVS_SINGLESEL, 0);
	m_list.InsertColumn(0, _T("Channel"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(1, _T("Voltage"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(2, _T("Load"), LVCFMT_LEFT, 100);
	m_list.SetGridLines(TRUE); // SHow grid lines
							   // m_list.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
}



void UpdateListusingRawData(CDisplay &t_cdisplay,
	const CalibrationData &t_cablirationdata,
	const std::vector<double> &t_rawdata)
{
	// Update Listcontrol
	CString str4Display;
	CString str4Display_Force;
	std::vector<double>::const_iterator it_begin = t_rawdata.begin();
	for (std::vector<double>::const_iterator it = it_begin; it != t_rawdata.end();
		++it) {
		int index = it - it_begin;
		str4Display.Format(_T("%2.4f"), *it);
		int t_index = t_cablirationdata.v_selected_channel[index];
		double t_force =
			t_rawdata[index] * t_cablirationdata.calibration_slope[t_index] +
			t_cablirationdata.calibration_intercept[t_index];
		str4Display_Force.Format(_T("%2.4f"), t_force);
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 1, str4Display);
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 2, str4Display_Force);
	}

}