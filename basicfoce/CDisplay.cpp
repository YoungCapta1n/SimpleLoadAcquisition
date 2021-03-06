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
	m_list.InsertColumn(1, _T("Voltage(V)"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(2, _T("Load(kg)"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, _T("Min (kg)"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("Max (kg)"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, _T("Avg (kg)"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, _T("Std (kg)"), LVCFMT_LEFT, 100);
	m_list.SetGridLines(TRUE); // SHow grid lines
							   // m_list.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
}



void UpdateListusingRawData(CDisplay &t_cdisplay,
	const CalibrationData &t_cablirationdata)
{
	// Update Listcontrol
	CString str4Display;
	CString str4Display_Force;
	computerealtimeLoad(t_cablirationdata, realtimeVoltage, realtimeLoad);
	std::vector<double>::const_iterator it_begin = realtimeVoltage.begin();
	for (std::vector<double>::const_iterator it = it_begin; it != realtimeVoltage.end();
		++it) {
		int index = it - it_begin;
		str4Display.Format(_T("%2.4f"), *it);
		str4Display_Force.Format(_T("%2.4f"), realtimeLoad[index]);
		int t_index = t_cablirationdata.v_selected_channel[index];
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 1, str4Display);
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 2, str4Display_Force);
	}

}

void computerealtimeLoad(const CalibrationData &t_cablirationdata,
	const std::vector<double> &t_rawdata, std::vector<double> &_loaddata){
	std::vector<double>::const_iterator it_begin = t_rawdata.begin();
	for (std::vector<double>::const_iterator it = it_begin; it != t_rawdata.end();
		++it) {
		int index = it - it_begin;
		int t_index = t_cablirationdata.v_selected_channel[index];
		_loaddata[index] =
			t_rawdata[index] * t_cablirationdata.calibration_slope[t_index] +
			t_cablirationdata.calibration_intercept[t_index];
	}
}

void showstatisticsdata(CDisplay &t_cdisplay, const CalibrationData &t_cablirationdata, 
     database &_database)
{
	// Update Listcontrol
	CString str4Display_min;
	CString str4Display_max;
	CString str4Display_avg;
	CString str4Display_std;
	size_t length = realtimeVoltage.size();
	std::vector<double> max_onetable(length, 0);
	std::vector<double> min_onetable(length, 0);
	std::vector<double> avg_onetable(length, 0);
	std::vector<double> std_onetable(length, 0);
	_database.computestatistics(max_onetable, min_onetable, avg_onetable, std_onetable);

	std::vector<double>::const_iterator it_begin = realtimeVoltage.begin();
	for (size_t it = 0; it != length; ++it) {
		str4Display_min.Format(_T("%2.4f"), min_onetable[it]);
		str4Display_max.Format(_T("%2.4f"), max_onetable[it]);
		str4Display_avg.Format(_T("%2.4f"), avg_onetable[it]);
		str4Display_std.Format(_T("%2.4f"), std_onetable[it]);
		int t_index = t_cablirationdata.v_selected_channel[it];
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 3, str4Display_min);
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 4, str4Display_max);
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 5, str4Display_avg);
		t_cdisplay.m_listchannelvalue.SetItemText(t_index, 6, str4Display_std);
	}

}