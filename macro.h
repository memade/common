#if !defined(AFX_SKSTU_macro_H__7332D_E275DCF6_E2EDC395B182A88_3D86__HEAD__)
#define AFX_SKSTU_macro_H__7332D_E275DCF6_E2EDC395B182A88_3D86__HEAD__

#define SK_CLASS_NEW(name) Class##name * temp = new Class#name();
#define SK_CLASS_MALLOC(name,outptr) auto p = new name(); outptr= reinterpret_cast<long long>(p);


#define SK_DELETE_PTR(p) {if(p) delete p;p=nullptr;}
#define SK_DELETE_PTR_BUFFER(pbuf) {if(pbuf) delete[] pbuf;pbuf=nullptr;}

#define SK_FUNCTION __FUNCTION__
#define SK_FUNCTIONW __FUNCTIONW__
#define SK_CRLF	"\r\n"

#define SK_SHARED_PTR(type,name,size) std::shared_ptr<type> name(new type[size],[](type *pbuffer){ delete[] pbuffer; })

#define SK_NET_STARTUP \
{\
WORD wVersionRequested;\
WSADATA wsaData;\
int err;\
wVersionRequested = MAKEWORD(2, 2);\
err = WSAStartup(wVersionRequested, &wsaData);\
if (err == 0) {\
if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {\
WSACleanup();\
}\
}\
}\

#define SK_NET_CLEANUP \
WSACleanup();\

/// /*新生联创（上海）**/
/// /*2019年05月18日 16:2:59|100**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_macro_H__7332D_E275DCF6_E2EDC395B182A88_3D86__TAIL__