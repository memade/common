#if !defined(AFX_SKSTU_1604538404_H__3B607_92885312_D6097FC4124CC50_21F3__HEAD__)
#define AFX_SKSTU_1604538404_H__3B607_92885312_D6097FC4124CC50_21F3__HEAD__

#define CEF_USE_SANDBOX
//#define CEF_USE_ATL
#if 0
#if !defined(_DEBUG)
#if defined(CEF_USE_SANDBOX)
#pragma comment(lib,"../../gpl/cef/Release/cef_sandbox.lib")
#endif
#pragma comment(lib,"../../gpl/cef/Release/libcef.lib")
#pragma comment(lib,"../../bin/lib/x64/Release/libcef.lib")
#else
#if defined(CEF_USE_SANDBOX)
#pragma comment(lib,"cef_sandbox.lib")
#endif
#pragma comment(lib,"libcef.lib")
#pragma comment(lib,"libcef_dll_wrapper.lib")
#endif
#endif

#ifdef IsMinimized
#undef IsMinimized
#endif
#ifdef IsMaximized
#undef IsMaximized
#endif
#ifdef GetNextSibling
#undef GetNextSibling
#endif
#ifdef GetFirstChild
#undef GetFirstChild
#endif
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_parser.h"
#include "include/cef_browser.h"
#include "include/base/cef_bind.h"
#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_closure_task.h"

namespace sk {




}///namespace sk


/// /*新生联创（上海）**/
/// /*2020年11月5日 9:20:23|700**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_1604538404_H__3B607_92885312_D6097FC4124CC50_21F3__TAIL__
