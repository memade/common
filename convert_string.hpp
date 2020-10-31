#if !defined(AFX_SKSTU_CONVERT_STRING_H__0A83D_DFA99D03_5516FD4E7A28283_4CF9__HEAD__)
#define AFX_SKSTU_CONVERT_STRING_H__0A83D_DFA99D03_5516FD4E7A28283_4CF9__HEAD__

#if defined WIN32
namespace sk {
	class StringConvert final {
	public:
		StringConvert() {}
		~StringConvert() = delete;
	public:
		static sk::string ToLower(const sk::string& src) {
			sk::string result(src);
			std::transform(result.begin(), result.end(), result.begin(), ::tolower);
			return result;
		}
		static sk::string ToUpper(const sk::string& src) {
			sk::string result(src);
			std::transform(result.begin(), result.end(), result.begin(), ::toupper);
			return result;
		}
		static sk::stringa ToLowerA(const sk::stringa& src) {
			sk::stringa result(src);
			std::transform(result.begin(), result.end(), result.begin(), ::tolower);
			return result;
		}
		static int ToLowerA(__inout sk::stringa& src) {
			if (!src.empty()) {
				std::transform(src.begin(), src.end(), src.begin(), ::tolower);
				return 0;
			}
			return -1;
		}
		static int ToUpperA(sk::stringa& src) {
			if (!src.empty()) {
				std::transform(src.begin(), src.end(), src.begin(), ::toupper);
				return 0;
			}
			return -1;
		}
		static sk::stringa ToUpperA(const sk::stringa& src) {
			sk::stringa result(src);
			std::transform(result.begin(), result.end(), result.begin(), ::toupper);
			return result;
		}
		static sk::stringa WStringToUTF8(const sk::stringw& src) {
			size_t iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)src.data(), -1, NULL, 0, NULL, NULL);
			char* pElementText = new char[iTextLen + 1];
			memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
			::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)src.data(), -1, pElementText, (int)iTextLen, NULL, NULL);
			std::string strReturn(pElementText);
			delete[] pElementText;
			return strReturn;
		}
		static std::wstring UTF8ToWString(const std::string& src) {
			size_t unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, NULL, 0);
			wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
			memset((void*)pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
			::MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, (LPWSTR)pUnicode, (int)unicodeLen);
			std::wstring wstrReturn(pUnicode);
			delete[] pUnicode;
			return wstrReturn;
		}
		static sk::stringa MBytesToUTF8(const sk::stringa& src) {
			return WStringToUTF8(MBytesToWString(src));
		}
		static sk::stringa UTF8ToMBytes(const sk::stringa& src) {
			return WStringToUTF8(UTF8ToWString(src));
		}
	public:
		static sk::stringa WStringToMBytes(const sk::stringw& src, const std::locale& locale = std::locale{ "" }) {
			size_t iTextLen = ::WideCharToMultiByte(CP_ACP, 0, src.data(), -1, NULL, 0, NULL, NULL);
			char* pElementText = new char[iTextLen + 1];
			memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
			::WideCharToMultiByte(CP_ACP, 0, src.data(), -1, pElementText, (int)iTextLen, NULL, NULL);
			std::string strReturn(pElementText);
			delete[] pElementText;
			return strReturn;
		}
		static sk::stringw MBytesToWString(const sk::stringa& src, const std::locale& locale = std::locale{ "" }) {
			size_t unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, src.data(), -1, NULL, 0);
			wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
			memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
			::MultiByteToWideChar(CP_ACP, 0, src.data(), -1, (LPWSTR)pUnicode, (int)unicodeLen);
			std::wstring wString = (wchar_t*)pUnicode;
			delete[] pUnicode;
			return wString;
		}
		static sk::string WStringToTString(const sk::stringw& src) {
#if defined(_UNICODE)
			return src;
#else
			return WStringToMBytes(src);
#endif
		}
		static sk::string MBytesToTString(const sk::stringa& src) {
#if defined(_UNICODE)
			return MBytesToWString(src);
#else
			return src;
#endif
		}
	};



}///namespace sk

#endif///WIN32


/// /*新生联创（上海）**/
/// /*2019年07月29日 9:16:48|730**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CONVERT_STRING_H__0A83D_DFA99D03_5516FD4E7A28283_4CF9__TAIL__