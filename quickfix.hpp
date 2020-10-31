#if !defined(AFX_SKSTU_QUICKFIX_H__7C16D_880D4E6B_E63D4F3BCA393AE_35E4__HEAD__)
#define AFX_SKSTU_QUICKFIX_H__7C16D_880D4E6B_E63D4F3BCA393AE_35E4__HEAD__

#if defined _M_IX86
#if defined _DEBUG
#pragma message("★★★★★comment <quickfix.lib(Debug)>★★★★★")
#pragma comment(lib,"..\\lib\\x86\\debug\\quickfix.lib")
#else
#pragma message("★★★★★comment <quickfix.lib(Release)>★★★★★")
#pragma comment(lib,"..\\lib\\x86\\release\\quickfix.lib")
#endif
#elif defined _M_X64
#if defined _DEBUG
#pragma message("★★★★★comment <quickfix.lib(Debug)>★★★★★")
#pragma comment(lib,"..\\lib\\x64\\debug\\quickfix.lib")
#else
#pragma message("★★★★★comment <quickfix.lib(Release)>★★★★★")
#pragma comment(lib,"..\\lib\\x64\\release\\quickfix.lib")
#endif
#else
#endif

#define HAVE_STD_UNIQUE_PTR
#define HAVE_SSL 1
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "../common/gpl/openssl/object/openssl-1.0.2t/out32/libeaycompat32.lib")
#pragma comment(lib, "../common/gpl/openssl/object/openssl-1.0.2t/out32/ssleay32.lib")
#pragma comment(lib, "../common/gpl/openssl/object/openssl-1.0.2t/out32/libeayfips32.lib")

#pragma warning(disable : 4503 4355 4786 5040 4267)
#include <Application.h>
#include <MessageCracker.h>
#include <Values.h>
#include <Utility.h>
#include <Mutex.h>
#include <Log.h>
#include <FileStore.h>
#include <Session.h>
#include <SessionSettings.h>

#if (HAVE_SSL > 0)
#include <ThreadedSSLSocketAcceptor.h>
#include <ThreadedSSLSocketInitiator.h>
#include <ThreadedSSLSocketConnection.h>
#include <SSLSocketAcceptor.h>
#include <SSLSocketInitiator.h>
#include <SSLSocketConnection.h>
#endif
#include <ThreadedSocketInitiator.h>
#include <ThreadedSocketAcceptor.h>
#include <SocketAcceptor.h>
#include <SocketInitiator.h>

#include <fix40/NewOrderSingle.h>
#include <fix40/ExecutionReport.h>
#include <fix40/OrderCancelRequest.h>
#include <fix40/OrderCancelReject.h>
#include <fix40/OrderCancelReplaceRequest.h>

#include <fix41/NewOrderSingle.h>
#include <fix41/ExecutionReport.h>
#include <fix41/OrderCancelRequest.h>
#include <fix41/OrderCancelReject.h>
#include <fix41/OrderCancelReplaceRequest.h>

#include <fix42/NewOrderSingle.h>
#include <fix42/ExecutionReport.h>
#include <fix42/OrderCancelRequest.h>
#include <fix42/OrderCancelReject.h>
#include <fix42/OrderCancelReplaceRequest.h>

#include <fix43/NewOrderSingle.h>
#include <fix43/ExecutionReport.h>
#include <fix43/OrderCancelRequest.h>
#include <fix43/OrderCancelReject.h>
#include <fix43/OrderCancelReplaceRequest.h>
#include <fix43/MarketDataRequest.h>

#include <fix44/NewOrderSingle.h>
#include <fix44/ExecutionReport.h>
#include <fix44/OrderCancelRequest.h>
#include <fix44/OrderCancelReject.h>
#include <fix44/OrderCancelReplaceRequest.h>
#include <fix44/MarketDataRequest.h>
#include <fix44/TestRequest.h>
#include <fix44/MarketDataSnapshotFullRefresh.h>
#include <fix44/Logon.h>
#include <fix44/Logout.h>

#include <fix50/NewOrderSingle.h>
#include <fix50/ExecutionReport.h>
#include <fix50/OrderCancelRequest.h>
#include <fix50/OrderCancelReject.h>
#include <fix50/OrderCancelReplaceRequest.h>
#include <fix50/MarketDataRequest.h>




/// /*新生联创（上海）**/
/// /*2019年11月6日 20:31:55|124**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_QUICKFIX_H__7C16D_880D4E6B_E63D4F3BCA393AE_35E4__TAIL__