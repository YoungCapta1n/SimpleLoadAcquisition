
// basicfoceDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "basicfoce.h"
#include "basicfoceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CbasicfoceDlg 对话框

CbasicfoceDlg::CbasicfoceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BASICFOCE_DIALOG, pParent), total_num_channel(16), 
	_data_acquisition(total_num_channel), calibration_data(total_num_channel), 
	time_inteval(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbasicfoceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBODEVICE, m_Devices);
	DDX_Control(pDX, IDC_BUT_SELECTDEVICE, m_selectdevice);
	DDX_Control(pDX, IDC_EDIT_COLLECTIONRATE, m_collectrate);
	DDX_Control(pDX, IDC_BUT_SETUPANDSTART, m_startcollection);
	DDX_Control(pDX, IDC_BUT_PAUSE, m_puase);
	DDX_Control(pDX, IDC_BUT_STOPCOLLECTION, m_stopcollection);
	DDX_Control(pDX, IDC_STATIC_FREQINFO, m_frequencyinformation);
	DDX_Control(pDX, IDC_LIST_LOGINFO, m_outputinformation);
	DDX_Control(pDX, IDC_MAINTAB, m_maintab);
	DDX_Control(pDX, IDC_BUT_COLLECTSHORT, m_collectshort);
	DDX_Control(pDX, IDC_BUT_CALCULATE, m_calculate);
	DDX_Control(pDX, IDC_BUT_CLEARCALI, m_clearcalibration);
	DDX_Control(pDX, IDC_BUT_FINISHCALI, m_finishcalibration);
}

BEGIN_MESSAGE_MAP(CbasicfoceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTAB, &CbasicfoceDlg::OnTcnSelchangeMaintab)
	ON_BN_CLICKED(IDC_BUT_SELECTDEVICE, &CbasicfoceDlg::OnBnClickedButSelectdevice)
	ON_BN_CLICKED(IDC_BUT_SETUPANDSTART, &CbasicfoceDlg::OnBnClickedButSetupandstart)
	ON_BN_CLICKED(IDC_BUT_PAUSE, &CbasicfoceDlg::OnBnClickedButPause)
	ON_BN_CLICKED(IDC_BUT_STOPCOLLECTION, &CbasicfoceDlg::OnBnClickedButStopcollection)
	ON_BN_CLICKED(IDC_BUT_COLLECTSHORT, &CbasicfoceDlg::OnBnClickedButCollectshort)
	ON_BN_CLICKED(IDC_BUT_CALCULATE, &CbasicfoceDlg::OnBnClickedButCalculate)
	ON_BN_CLICKED(IDC_BUT_CLEARCALI, &CbasicfoceDlg::OnBnClickedButClearcali)
	ON_BN_CLICKED(IDC_BUT_FINISHCALI, &CbasicfoceDlg::OnBnClickedButFinishcali)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CbasicfoceDlg 消息处理程序

BOOL CbasicfoceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// Tab control
	m_maintab.InsertItem(0, _T("Calibration"));
	m_maintab.InsertItem(1, _T("Real-time"));
	m_tabcalibration.Create(IDD_DIA_CALIBRATION, GetDlgItem(IDC_MAINTAB));
	m_tabDisplay.Create(IDD_DIADISPLAY, GetDlgItem(IDC_MAINTAB));
	m_maintab.SetCurSel(0);

	// Current path
	GetCurrentEXEPath();

	// Initialize the calibration list
	((CalibrationDiag *)GetParent())
		->InitializeReportlist(calibration_data, current_path,
			m_tabcalibration.m_calibrationlist);

	// Data Acquisition
	_data_acquisition.InitilizeDaqCom(m_Devices);

	CString TempRate;
	TempRate.Format(_T("%d"), _data_acquisition.get_UpdateRate());
	m_collectrate.SetWindowTextW(TempRate);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbasicfoceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbasicfoceDlg::OnPaint()
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
		CRect rs;
		m_maintab.GetClientRect(&rs);
		//调整Tab大小
		rs.top += 21;
		rs.bottom -= 1;
		rs.left += 1;
		rs.right -= 1;

		//移动并显示
		m_tabcalibration.MoveWindow(&rs);
		m_tabDisplay.MoveWindow(&rs);

		m_tabcalibration.ShowWindow(TRUE);
		m_tabDisplay.ShowWindow(FALSE);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CbasicfoceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CbasicfoceDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_maintab.GetCurSel();
	m_tabcalibration.ShowWindow(CurSel == 0);
	m_tabDisplay.ShowWindow(CurSel == 1);
	*pResult = 0;

}


void CbasicfoceDlg::OnBnClickedButSelectdevice()
{
	// Select the device for data acquisition
	_data_acquisition.SelectDAQDevice(m_Devices)
		.SetupDAQCollection()
		.SetupDisplay(m_tabDisplay.m_listchannelvalue);
	_data_acquisition.set_UpdateRate(ConvertEditToDouble(m_collectrate));
	CString Str_ScanFreq;
	CString Str_SampleFreq;
	Str_ScanFreq.Format(_T("The Scan Freq (Hz):  %.1f\n"),
		_data_acquisition.get_rate());
	Str_SampleFreq.Format(_T("The Sample Freq (Hz):  %d\n"),
		_data_acquisition.get_UpdateRate());
	Str_ScanFreq = Str_ScanFreq + Str_SampleFreq;
	m_frequencyinformation.SetWindowTextW(Str_ScanFreq);
	m_collectrate.EnableWindow(FALSE);

	_data_acquisition.StopDAQCollection();

	// Button operation
	m_selectdevice.EnableWindow(false);
	m_collectshort.EnableWindow(true);
	m_maintab.EnableWindow(true);
	m_finishcalibration.EnableWindow(true);
}


void CbasicfoceDlg::OnBnClickedButSetupandstart()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CbasicfoceDlg::OnBnClickedButPause()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CbasicfoceDlg::OnBnClickedButStopcollection()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CbasicfoceDlg::OnBnClickedButCollectshort()
{
	CString c_input_value;
	m_tabcalibration.m_inputvalue.GetWindowTextW(c_input_value);
	double t_average = 0.0;
	std::thread tt_thread(&CbasicfoceDlg::IndependentThread4Calibration,
		this, m_tabcalibration.selected_item,
		std::ref(t_average));
	tt_thread.join();

	call_calibration_acquisition(m_tabcalibration, calibration_data,
		c_input_value, t_average);

	m_calculate.EnableWindow(true);
	m_clearcalibration.EnableWindow(true);
}


void CbasicfoceDlg::OnBnClickedButCalculate()
{
	call_calibration_calculate(m_tabcalibration, calibration_data, current_path);
}


void CbasicfoceDlg::OnBnClickedButClearcali()
{
	call_calibration_clear(m_tabcalibration, calibration_data, current_path);
}


void CbasicfoceDlg::OnBnClickedButFinishcali()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CbasicfoceDlg::DestroyWindow()
{
	_data_acquisition.CloseDAQCollection();
	return CDialogEx::DestroyWindow();
}

void CbasicfoceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
}




void CbasicfoceDlg::GetCurrentEXEPath() {
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	current_path = path.Left(pos);
}

int CbasicfoceDlg::ConvertEditToDouble(CEdit &m_edit) {
	CString Temp;
	m_edit.GetWindowTextW(Temp);
	return _ttoi(Temp);
}

void CbasicfoceDlg::IndependentThread4Calibration(int t_index_channel,
	double &t_average) {
	t_average = 0;
	_data_acquisition.StartDAQCollection();
	for (unsigned it = 0; it != calibration_data.sample_num; ++it) {
		std::vector<double> channelvalue;
		_data_acquisition.ScanAndGatherData(channelvalue);
		t_average += channelvalue[t_index_channel];
		// std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	_data_acquisition.StopDAQCollection();
	t_average /= calibration_data.sample_num;
}




