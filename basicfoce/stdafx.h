
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#if !defined(AFX_STDAFX_H__BC2FF314_38D8_4022_AE89_97623EFB4F20__INCLUDED_)
#define AFX_STDAFX_H__BC2FF314_38D8_4022_AE89_97623EFB4F20__INCLUDED_

#pragma warning(disable:4996)  

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

//IOtech needed declarations///////////////////////
#define _WIN32_DCOM
//End IOtech///////////////////////////////////////


#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <vector>

#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持



#include "ReportCtrl.h"        // List Control


//IOtech needed declarations/////////////////////////
#include <atlbase.h>
#include <atlconv.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

#import "DaqCOM2.tlb" rename("LoadString", "DC2LoadString")
using namespace DAQCOMLib;
//End IOtech declarations///////////////////////////////

// real-time load data
extern std::vector<double> realtimeVoltage;
extern std::vector<double> realtimeLoad;
extern std::vector<int> selected_channel;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#endif  //!defined(AFX_STDAFX_H__BC2FF314_38D8_4022_AE89_97623EFB4F20__INCLUDED_)