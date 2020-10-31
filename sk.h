#if !defined(AFX_SKSTU_SK_H__227DA_9909074D_465CFC406C9909F_E3D0__HEAD__)
#define AFX_SKSTU_SK_H__227DA_9909074D_465CFC406C9909F_E3D0__HEAD__

namespace sk {
#if defined(_UNICODE)
	typedef std::wstring string;
	typedef wchar_t tchar;
#define tostring std::to_wstring
#else
	typedef char tchar;
	typedef std::string string;
#define tostring std::to_string
#endif

#define tostringw std::to_wstring
#define tostringa std::to_string

	typedef std::string stringa;
	typedef std::wstring stringw;
	typedef std::intptr_t intptr;
	typedef std::intptr_t lptr;
	typedef std::intptr_t nptr;
	typedef std::int64_t int64;
	typedef std::int32_t int32;
	typedef std::int16_t int16;
	typedef std::int8_t int8;
	typedef std::uint8_t uint8;
	typedef std::uint64_t uint64;
	typedef std::uint32_t uint32;
	typedef std::uint16_t uint16;
	typedef std::int64_t timestamp;
	typedef std::double_t real64;
	typedef std::float_t real32;
	typedef std::uint64_t size;
	typedef std::int64_t command;
	typedef std::string packet;
	typedef char cchar;
	typedef wchar_t wchar;


	typedef unsigned char byte;

#define TypeToPtr(data) reinterpret_cast<sk::intptr>(data)
#define PtrToType(type,data) reinterpret_cast<type*>(data)
	static const tchar global_sk_ftime_utc_format[] = TEXT("%Y-%m-%dT%H:%M:%SZ %A");
	static const char global_sk_ftime_utc_format_a[] = "%Y-%m-%dT%H:%M:%SZ %A";
	static const wchar_t global_sk_ftime_utc_format_w[] = L"%Y-%m-%dT%H:%M:%SZ %A";
	static const unsigned char global_sk_team_tenet[] = { 0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x20,0xB0,0xB2,0xC8,0xAB,0xA1,0xA2,0xCE,0xC8,0xB6,0xA8,0xA1,0xA2,0xC9,0xF9,0xD3,0xFE,0x20,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x00 };
	static const unsigned char global_sk_team_sign[] = { 0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x20,0xD0,0xC2,0xC9,0xFA,0xC1,0xAA,0xB4,0xB4,0xA3,0xA8,0xC9,0xCF,0xBA,0xA3,0xA3,0xA9,0x20,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x00 };
}///namespace sk


/// /*新生联创（上海）**/
/// /*2019年07月29日 9:27:35|234**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_SK_H__227DA_9909074D_465CFC406C9909F_E3D0__TAIL__