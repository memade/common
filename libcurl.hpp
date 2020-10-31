#if !defined(AFX_SKSTU_libcurl_H__672C3_2BFD88A7_FC199B490C961ED_8CDA__HEAD__)
#define AFX_SKSTU_libcurl_H__672C3_2BFD88A7_FC199B490C961ED_8CDA__HEAD__

#if defined _M_IX86
#if defined(_DEBUG)
#pragma comment(lib,"../Common/Gpl/curl/builds/libcurl-vc16-x86-debug-static-ipv6-sspi-winssl/lib/libcurl_a_debug.lib")
#else
#pragma comment(lib,"../Common/Gpl/curl/builds/libcurl-vc16-x86-release-static-ipv6-sspi-winssl/lib/libcurl_a.lib")
#endif
#elif defined _M_X64
#if defined(_DEBUG)
#pragma comment(lib,"../Common/Gpl/curl/builds/libcurl-vc16-x64-debug-static-ipv6-sspi-winssl/lib/libcurl_a_debug.lib")
#else
#pragma comment(lib,"../Common/Gpl/curl/builds/libcurl-vc16-x64-release-static-ipv6-sspi-winssl/lib/libcurl_a.lib")
#endif
#else
#endif

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Normaliz.lib")
  
#define CURL_STATICLIB
#define BUILDING_LIBCURL
#define HTTP_ONLY

/// /*新生联创（上海）**/
/// /*2019年05月29日 12:0:57|917**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_libcurl_H__672C3_2BFD88A7_FC199B490C961ED_8CDA__TAIL__

