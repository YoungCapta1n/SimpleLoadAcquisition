#pragma once


// CDisplay 对话框

class CDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplay)

public:
	CDisplay(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDisplay();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIADISPLAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	void InitializeLIST(CReportCtrl &m_list);
	CReportCtrl m_listchannelvalue;
	
};
