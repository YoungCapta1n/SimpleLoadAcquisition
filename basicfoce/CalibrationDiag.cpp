// CalibrationDiag.cpp: 实现文件
//

#include "stdafx.h"
#include "basicfoce.h"
#include "CalibrationDiag.h"
#include "afxdialogex.h"


// CalibrationDiag 对话框

IMPLEMENT_DYNAMIC(CalibrationDiag, CDialogEx)

CalibrationDiag::CalibrationDiag(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIA_CALIBRATION, pParent)
{

}

CalibrationDiag::~CalibrationDiag()
{
}

void CalibrationDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CALIBRATION, m_calibrationlist);
	DDX_Control(pDX, IDC_PICCALIBRATION, m_picFit);
	DDX_Control(pDX, IDC_EDITINPUT, m_inputvalue);
	DDX_Control(pDX, IDC_EDIT_AVERAGEVALUE, m_averagevalue);
}


BEGIN_MESSAGE_MAP(CalibrationDiag, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CALIBRATION, &CalibrationDiag::OnClickListCalibration)
END_MESSAGE_MAP()


// CalibrationDiag 消息处理程序


BOOL CalibrationDiag::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitializePIC(ChartDisplay, IDC_PICCALIBRATION, _T("Load(kg)"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CalibrationDiag::OnClickListCalibration(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// get the click position and check
	POSITION pos = m_calibrationlist.GetFirstSelectedItemPosition();
	selected_item = m_calibrationlist.GetNextSelectedItem(pos);
	if (m_calibrationlist.GetCheck(selected_item))
		m_calibrationlist.SetCheck(selected_item, FALSE);
	else
		m_calibrationlist.SetCheck(selected_item, TRUE);

	*pResult = 0;
}


void CalibrationDiag::InitializeReportlist(CalibrationData &t_calibrationdata,
	CString t_current_path,
	CReportCtrl &t_calibrationlist) {
	// Initilize the channel list
	t_calibrationlist.DeleteAllItems();
	t_calibrationlist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	// m_list.ModifyStyle(LVS_SINGLESEL, 0);
	t_calibrationlist.InsertColumn(0, _T(" "), LVCFMT_LEFT, 25);
	t_calibrationlist.InsertColumn(1, _T("Channel"), LVCFMT_LEFT, 80);
	t_calibrationlist.InsertColumn(2, _T("Coefficient(kg/V)"), LVCFMT_LEFT, 120);
	t_calibrationlist.InsertColumn(3, _T("Zero(V)"), LVCFMT_LEFT, 70);
	t_calibrationlist.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
	t_calibrationlist.SetGridLines(TRUE);                  // SHow grid lines
	t_calibrationlist.SetEditable(TRUE);                   // Allow sub-text edit
	t_calibrationlist.SortItems(1, TRUE); // sort the 2nd column, ascending

	t_calibrationdata.ReadConfigurationData(t_current_path);

	for (int it = 0; it != t_calibrationdata.total_num_channel; ++it) {
		CString temp_channel;
		temp_channel.Format(_T("%2d"), it);
		temp_channel = _T("channel ") + temp_channel;
		t_calibrationlist.InsertItem(it, _T(""));
		t_calibrationlist.SetItemText(it, 1, temp_channel);

	}

	UpdateList(t_calibrationdata, t_calibrationlist);
}

void CalibrationDiag::UpdateList(const CalibrationData &t_calibrationdata,
	CReportCtrl &t_calibrationlist) {

	for (int it = 0; it != t_calibrationdata.total_num_channel; ++it) {
		CString temp_slope;
		CString temp_intercept;
		temp_slope.Format(_T("%2.5f"), t_calibrationdata.calibration_slope[it]);
		temp_intercept.Format(_T("%2.5f"),
			t_calibrationdata.calibration_intercept[it]);
		t_calibrationlist.SetItemText(it, 2, temp_slope);
		t_calibrationlist.SetItemText(it, 3, temp_intercept);

		// update the list color according to
		// {empty--> purple; acquisition-->red; calculation-->green}
		switch (t_calibrationdata.calibration_status[it]) {
		case 0:
			t_calibrationlist.SetItemBkColor(it, 1, RGB(52, 168, 83));
			break;
		case 1:
			t_calibrationlist.SetItemBkColor(it, 1, RGB(234, 67, 53));
			break;
		case 2:
			t_calibrationlist.SetItemBkColor(it, 1, RGB(212, 153, 211));
			break;
		default:
			break;
		}
	}
}

void CalibrationDiag::InitializePIC(ChartDisplay_c &t_ChartDisplay,
	int ID_PicControl, CString str_Channel) {

	// Initialize the Picture Control
	GetDlgItem(ID_PicControl)->GetWindowRect(t_ChartDisplay.rect);
	GetDlgItem(ID_PicControl)->ShowWindow(SW_HIDE);
	ScreenToClient(t_ChartDisplay.rect);
	t_ChartDisplay.m_tab.Create(CMFCTabCtrl::STYLE_3D_ROUNDED,
		t_ChartDisplay.rect, this, 1,
		CMFCTabCtrl::LOCATION_TOP);
	t_ChartDisplay.m_tab.AutoSizeWindow(TRUE);

	t_ChartDisplay.m_HSChartCtrl.Create(&t_ChartDisplay.m_tab,
		t_ChartDisplay.rect, 2);
	t_ChartDisplay.m_HSChartCtrl.SetBackColor(RGB(80, 32, 121));

	// X axis on the left
	t_ChartDisplay.pAxisX =
		t_ChartDisplay.m_HSChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	t_ChartDisplay.pAxisX->SetAxisColor(RGB(255, 255, 255));
	t_ChartDisplay.pAxisX->SetTextColor(RGB(255, 255, 255));
	t_ChartDisplay.pAxisX->SetAutomatic(false);
	t_ChartDisplay.pAxisX->SetMinMax(-5, 5);
	t_ChartDisplay.pAxisX->SetInverted(false);
	t_ChartDisplay.pAxisX->SetVisible(true);
	// Y axis on the bottom
	t_ChartDisplay.pAxisY =
		t_ChartDisplay.m_HSChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	t_ChartDisplay.pAxisY->SetAutomatic(false);
	t_ChartDisplay.pAxisY->SetMinMax(-5, 5);
	t_ChartDisplay.pAxisY->SetTextColor(RGB(255, 255, 255));
	t_ChartDisplay.pAxisY->SetAxisColor(RGB(255, 255, 255));
	t_ChartDisplay.pAxisY->SetInverted(false);
	t_ChartDisplay.pAxisY->SetVisible(true);


	// Linear Fitting
	t_ChartDisplay.m_pLineSerie = t_ChartDisplay.m_HSChartCtrl.CreateLineSerie();
	t_ChartDisplay.m_pLineSerie->SetColor(RGB(255, 255, 255));

	UpdateData(FALSE);
	t_ChartDisplay.m_tab.AddTab(&t_ChartDisplay.m_HSChartCtrl, str_Channel);
	t_ChartDisplay.m_tab.SetActiveTab(0);
	t_ChartDisplay.m_tab.ShowWindow(SW_SHOWNORMAL);
}

void CalibrationDiag::RecordMap(CalibrationData &t_calibrationdata,
	std::vector<int> &t_sample_channel) {
	t_sample_channel.clear();
	t_calibrationdata.v_selected_channel.clear();
	for (int it = 0; it != t_calibrationdata.total_num_channel; ++it) {
		if (t_calibrationdata.calibration_status[it] == 0) {
			t_calibrationdata.v_selected_channel.push_back(it);
			t_sample_channel.push_back(it);
		}
	}
}

void CalibrationDiag::ChangeNumPoints(ChartDisplay_c &t_ChartDisplay,
	size_t t_num_point) {
	t_ChartDisplay.num_points = t_num_point;
}

void CalibrationDiag::InitializePointVector(ChartDisplay_c &t_ChartDisplay) {
	// Points of Original Data
	t_ChartDisplay.v_pXYSerie.clear();
	for (size_t it = 0; it != t_ChartDisplay.num_points; ++it) {
		t_ChartDisplay.v_pXYSerie.push_back(
			t_ChartDisplay.m_HSChartCtrl.CreateLineSerie());
		t_ChartDisplay.v_pXYSerie.back()->SetColor(RGB(255, 255, 0));
	}
}

void CalibrationDiag::ClearArrayAndDisplay_Multi(ChartDisplay_c &t_ChartDisplay) {
	// Line
	t_ChartDisplay.m_pLineSerie->ClearSerie();
	t_ChartDisplay.m_pLineSerie->AddPoints(
		&t_ChartDisplay.linedisplay_x_vector[0],
		&t_ChartDisplay.linedisplay_y_vector[0], t_ChartDisplay.m_c_arrayLength);

	// Points
	int t_index_map = 0;
	for (auto it = t_ChartDisplay.v_pXYSerie.begin();
		it != t_ChartDisplay.v_pXYSerie.end(); ++it) {
		(*it)->ClearSerie();
		(*it)->AddPoints(&t_ChartDisplay.map_vector_x[t_index_map].front(),
			&t_ChartDisplay.map_vector_y[t_index_map].front(),
			t_ChartDisplay.m_c_arrayLength);
		++t_index_map;
	}
}

void CalibrationDiag::Copy2ChartDisplay(ChartDisplay_c &t_ChartDisplay,
	const std::vector<double> &t_linevector_x,
	const std::vector<double> &t_linevector_y,
	const std::vector<double> &t_pointvector_x,
	const std::vector<double> &t_pointvector_y,
	double t_display_min,
	double t_display_max) {
	t_ChartDisplay.linedisplay_x_vector = t_linevector_x;
	t_ChartDisplay.linedisplay_y_vector = t_linevector_y;
	t_ChartDisplay.pointdisplay_x_vector = t_pointvector_x;
	t_ChartDisplay.pointdisplay_y_vector = t_pointvector_y;
	t_ChartDisplay.display_max = t_display_max;
	t_ChartDisplay.display_min = t_display_min;
}

void CalibrationDiag::MakeArrary(ChartDisplay_c &t_ChartDisplay) {
	double Radius =
		0.008 * (t_ChartDisplay.display_max - t_ChartDisplay.display_min);

	for (int it = 0; it != t_ChartDisplay.pointdisplay_x_vector.size(); ++it) {
		// Initialize the map
		t_ChartDisplay.map_vector_x.insert(
			std::make_pair(it, std::vector<double>{}));
		t_ChartDisplay.map_vector_y.insert(
			std::make_pair(it, std::vector<double>{}));

		// Center point
		double CenterX = t_ChartDisplay.pointdisplay_x_vector[it];
		double CenterY = t_ChartDisplay.pointdisplay_y_vector[it];

		for (size_t i = 0; i != t_ChartDisplay.m_c_arrayLength; ++i) {
			// Make a circle
			double Temp_x =
				CenterX +
				Radius * std::cos(2 * 3.1415926 * i / t_ChartDisplay.m_c_arrayLength);
			double Temp_y =
				CenterY +
				Radius * std::sin(2 * 3.1415926 * i / t_ChartDisplay.m_c_arrayLength);
			t_ChartDisplay.map_vector_x[it].push_back(Temp_x);
			t_ChartDisplay.map_vector_y[it].push_back(Temp_y);
		}
	}
}

void CalibrationDiag::ResetAxisRange(ChartDisplay_c &t_ChartDisplay) {
	t_ChartDisplay.pAxisX->SetMinMax(t_ChartDisplay.display_min,
		t_ChartDisplay.display_max);
	t_ChartDisplay.pAxisY->SetMinMax(t_ChartDisplay.display_min,
		t_ChartDisplay.display_max);
}


void CalibrationDiag::ShowFittingResults(
	ChartDisplay_c &t_ChartDisplay, size_t t_num_point, CString t_channel_name,
	const std::vector<double> &t_linevector_x, const std::vector<double> &t_linevector_y,
	const std::vector<double> &t_pointvector_x, const std::vector<double> &t_pointvector_y,
	double t_display_min, double t_display_max) {
	t_ChartDisplay.m_tab.SetTabLabel(0, t_channel_name);
	ChangeNumPoints(ChartDisplay, t_num_point);
	InitializePointVector(ChartDisplay);
	Copy2ChartDisplay(ChartDisplay, t_linevector_x, t_linevector_y,
		t_pointvector_x, t_pointvector_y, t_display_min,
		t_display_max);
	MakeArrary(ChartDisplay);
	ClearArrayAndDisplay_Multi(ChartDisplay);
	ResetAxisRange(ChartDisplay);
}


// Non-member function  //
void call_calibration_acquisition(CalibrationDiag &t_tabcali,
	CalibrationData &t_calidata,
	CString t_input_value, double t_average) {

	t_calidata.Add2Map(t_tabcali.selected_item, atof(t_input_value), t_average);
	CString c_average_value;
	c_average_value.Format(_T("%2.4f"), t_average);
	t_tabcali.m_averagevalue.SetWindowText(c_average_value);
	t_tabcali.UpdateList(t_calidata, t_tabcali.m_calibrationlist);
}

void call_calibration_calculate(CalibrationDiag &t_tabcali,
	CalibrationData &t_calidata,
	CString t_current_path) {
	int t_index_of_channel = t_tabcali.selected_item;
	t_calidata.LLSCalculate(t_index_of_channel);

	CString Filename = t_current_path + _T("\\calibration.txt");
	t_calidata.WriteConfigurationData(Filename);

	call_show_calibration(t_tabcali, t_calidata);

	t_tabcali.UpdateList(t_calidata, t_tabcali.m_calibrationlist);
}

void call_show_calibration(CalibrationDiag &t_tabcali, CalibrationData &t_calidata)
{
	int t_index_of_channel = t_tabcali.selected_item;
	std::vector<double> linevector_x;
	std::vector<double> linevector_y;
	std::vector<double> pointvector_x;
	std::vector<double> pointvector_y;
	double display_min;
	double display_max;
	int t_num_points;
	t_calidata.Convert4Display(t_index_of_channel, t_num_points, linevector_x,
		linevector_y, pointvector_x, pointvector_y,
		display_min, display_max);

	CString t_channel_name;
	t_channel_name.Format(_T("%d"), t_index_of_channel);
	t_channel_name = _T("Load(kg): channel ") + t_channel_name;
	t_tabcali.ShowFittingResults(
		t_tabcali.ChartDisplay, t_num_points, t_channel_name, linevector_x,
		linevector_y, pointvector_x, pointvector_y, display_min, display_max);

}

void call_calibration_clear(CalibrationDiag &t_tabcali, CalibrationData &t_calidata,
	CString t_current_path) {
	int t_index_of_channel = t_tabcali.selected_item;
	t_calidata.ClearOne(t_index_of_channel);
	CString Filename = t_current_path + _T("\\calibration.txt");
	t_calidata.WriteConfigurationData(Filename);
	t_tabcali.UpdateList(t_calidata, t_tabcali.m_calibrationlist);
}
