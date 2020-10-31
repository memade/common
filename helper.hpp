#if !defined(AFX_SKSTU_HELPER_H__F85DA_A25E5068_A25CCC88E211448_EB9C__HEAD__)
#define AFX_SKSTU_HELPER_H__F85DA_A25E5068_A25CCC88E211448_EB9C__HEAD__


//#ifdef WIN32

#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <shellapi.h>
#include <Iphlpapi.h>
#include <TlHelp32.h>
#include <psapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Psapi.lib")
#include "common.h"

namespace sk {
	CONST TCHAR CONST_NUMBER_REAL_NUM[] = _T(".0123456789-");
	CONST CHAR CONST_NUMBER_REAL_NUM_A[] = ".0123456789-";
	CONST WCHAR CONST_NUMBER_REAL_NUM_W[] = L".0123456789-";

	CONST CHAR CONST_NUMBER_REAL[] = "0123456789.";
	CONST CHAR CONST_NUMBER_CH[] = "0123456789";
	CONST CHAR CONST_LETTER_CH[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	CONST CHAR CONST_HEX_CH[] = "0123456789abcdefABCDEF";
	CONST CHAR CONST_FUNSIGN_CH[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

	__inline static size_t SafeCopyA(char* dest, const char* src, const size_t& ndest) {
		if (!dest) return 0;
		if (!src || (ndest <= 0))
		{
			dest[0] = 0;
			return 0;
		}
		::memset(dest, 0x00, ndest);
		size_t nsrc = ::strlen(src);
		size_t ncopy = nsrc > ndest ? ndest : nsrc;
		::memcpy(dest, src, ncopy);
		return ncopy;
	}
	__inline static size_t SafeCopyW(wchar_t* dest, const wchar_t* src, const size_t& ndest) {
		if (!dest) return 0;
		if (!src || (ndest <= 0))
		{
			dest[0] = 0;
			return 0;
		}
		::wmemset(dest, 0x00, ndest);
		size_t nsrc = ::wcslen(src);
		size_t ncopy = nsrc > ndest ? ndest : nsrc;
		::wmemcpy(dest, src, ncopy);
		return ncopy;
	}
	__inline static int SafeCmpA(const char* obj1, const char* obj2, const size_t& ncmp) {
		if (!obj1 || !obj2 || (ncmp <= 0)) return 0;
		return memcmp(obj1, obj2, strlen(obj1) > ncmp ? ncmp : strlen(obj1));
	}
	__inline static int SafeCmpW(const wchar_t* obj1, const wchar_t* obj2, const size_t& ncmp) {
		if (!obj1 || !obj2 || (ncmp <= 0)) return 0;
		return wmemcmp(obj1, obj2, wcslen(obj1) > ncmp ? ncmp : wcslen(obj1));
	}


#pragma pack(push,1)
	typedef struct tagMachineHardwareInformation
	{
		LONG Head;
		UINT64 CPUID;
		CHAR MACS[32][100];
		CHAR SerialNumber[512];
		LONG Tail;

		tagMachineHardwareInformation()
		{
			memset(this, 0x00, sizeof(*this));
			Head = 0xFEFF;
			Tail = 0x200B;
		}
		bool Verify() const { return Head == 0xFEFF && Tail == 0x200B; }

		void operator=(const tagMachineHardwareInformation& obj) { memcpy(this, &obj, sizeof(*this)); }
		bool operator==(const tagMachineHardwareInformation& obj) const
		{
			if (!Verify() || !obj.Verify())
				return false;
			if (CPUID != obj.CPUID)
				return false;
			else if (SafeCmpA(SerialNumber, obj.SerialNumber, _countof(SerialNumber)))
				return false;
			else
			{
				bool empty = true;
				for (int i = 0; i < _countof(MACS); ++i)
				{
					if (MACS[i][0])
					{
						empty = false;
						break;
					}
				}
				bool emptyObj = true;
				for (int i = 0; i < _countof(obj.MACS); ++i)
				{
					if (obj.MACS[i][0])
					{
						emptyObj = false;
						break;
					}
				}
				if (empty || emptyObj)
					return false;

				for (int i = 0; i < _countof(obj.MACS); ++i)
				{
					if (obj.MACS[i] == nullptr || obj.MACS[i][0] == 0)
						continue;

					bool found = false;
					for (int j = 0; j < _countof(MACS); ++j)
					{
						if (MACS[i] == nullptr || MACS[i][0] == 0)
							continue;
						if (!sk::SafeCmpA(MACS[i], obj.MACS[i], _countof(MACS[0])))
						{
							found = true;
							break;
						}
					}
					if (!found)
						return false;
				}
			}
			return true;
		}
		bool operator!=(const tagMachineHardwareInformation& obj) const
		{
			return !(*this == obj);
		}
	}MachineHardwareInformation; const size_t LENMACHINEHARDWAREINFORMATION = sizeof(MachineHardwareInformation);
#pragma pack(pop)




	class Helper final {
	public:
		Helper() {}
		~Helper() = delete;
	public:
		static DWORD WriteConsoleInputQ()
		{
			HANDLE hConIn = GetStdHandle(STD_INPUT_HANDLE);
			DWORD dwTmp;
			INPUT_RECORD ir[3];

			ir[0].EventType = KEY_EVENT;
			ir[0].Event.KeyEvent.bKeyDown = TRUE;
			ir[0].Event.KeyEvent.dwControlKeyState = 0;
			ir[0].Event.KeyEvent.uChar.UnicodeChar = 'q';
			ir[0].Event.KeyEvent.wRepeatCount = 1;
			ir[0].Event.KeyEvent.wVirtualKeyCode = 'Q';
			ir[0].Event.KeyEvent.wVirtualScanCode = MapVirtualKey('Q', MAPVK_VK_TO_VSC);

			ir[1].EventType = KEY_EVENT;
			ir[1].Event.KeyEvent.bKeyDown = TRUE;
			ir[1].Event.KeyEvent.dwControlKeyState = 0;
			ir[1].Event.KeyEvent.uChar.UnicodeChar = VK_RETURN;
			ir[1].Event.KeyEvent.wRepeatCount = 1;
			ir[1].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
			ir[1].Event.KeyEvent.wVirtualScanCode = MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);

			ir[2].EventType = KEY_EVENT;
			ir[2].Event.KeyEvent.bKeyDown = FALSE;
			ir[2].Event.KeyEvent.dwControlKeyState = 0;
			ir[2].Event.KeyEvent.uChar.UnicodeChar = VK_RETURN;
			ir[2].Event.KeyEvent.wRepeatCount = 1;
			ir[2].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
			ir[2].Event.KeyEvent.wVirtualScanCode = MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);

			dwTmp = 0;
			WriteConsoleInput(hConIn, ir, 3, &dwTmp);
			return dwTmp;
		}
		//! C++17 计时器
		template<typename T>
		static __int64 TickCountGet()
		{
			return std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
		template<typename T>
		static __int64 LocalTimestampGet()
		{
			return std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		static sk::stringa FloatStrToStrA(const char* _val, int nDecimalPlaces) {
			double val = strtod(_val, nullptr);
			char szDecimalPlaces[64] = { 0 };
			sprintf_s(szDecimalPlaces, ARRAYSIZE(szDecimalPlaces), "%%.%df", nDecimalPlaces);
			char szTemp[64] = { 0 };
			sprintf_s(szTemp, ARRAYSIZE(szTemp), szDecimalPlaces, val);
			return szTemp;
		}
		static sk::stringw FloatStrToStrW(const wchar_t* _val, int nDecimalPlaces) {
			double val = wcstod(_val, nullptr);
			wchar_t szDecimalPlaces[64] = { 0 };
			swprintf_s(szDecimalPlaces, ARRAYSIZE(szDecimalPlaces), L"%%.%df", nDecimalPlaces);
			wchar_t szTemp[64] = { 0 };
			swprintf_s(szTemp, ARRAYSIZE(szTemp), szDecimalPlaces, val);
			return szTemp;
		}
		static sk::string FloatStrToStr(const TCHAR* _val, int nDecimalPlaces) {
			double val = _tcstod(_val, nullptr);
			TCHAR szDecimalPlaces[64] = { 0 };
			_stprintf_s(szDecimalPlaces, ARRAYSIZE(szDecimalPlaces), _T("%%.%df"), nDecimalPlaces);
			TCHAR szTemp[64] = { 0 };
			_stprintf_s(szTemp, ARRAYSIZE(szTemp), szDecimalPlaces, val);
			return szTemp;
		}
		/**小数转换成字符串*/
		static sk::string FloatToStr(const double& _val, int nDecimalPlaces, bool reject_valid = true) {
			TCHAR szDecimalPlaces[64] = { 0 };
			_stprintf_s(szDecimalPlaces, ARRAYSIZE(szDecimalPlaces), _T("%%.%df"), nDecimalPlaces);
			TCHAR szTemp[64] = { 0 };
			_stprintf_s(szTemp, ARRAYSIZE(szTemp), szDecimalPlaces, _val);
			if (reject_valid && nDecimalPlaces > 0) {//! 舍弃无效的小数点位,比如小数点后无效的0
				for (size_t i = _tcslen(szTemp); i > 0; --i) {
					if (szTemp[i] == _T('\0')) {
						continue;
					}
					if (szTemp[i] == _T('0')) {
						szTemp[i] = 0;
					}
					else if (szTemp[i] == _T('.')) {
#if 0
						szTemp[i] = _T('0');
						szTemp[i + 2] = 0;
#else
						szTemp[i] = 0;//! 去掉 .0 情况
#endif
						break;
					}
					else {
						break;
					}
				}
			}
			return szTemp;
		}
		static sk::stringa FloatToStrA(const double& _val, int nDecimalPlaces, bool reject_valid = true) {
			char szDecimalPlaces[64] = { 0 };
			sprintf_s(szDecimalPlaces, ARRAYSIZE(szDecimalPlaces), "%%.%df", nDecimalPlaces);
			char szTemp[64] = { 0 };
			sprintf_s(szTemp, ARRAYSIZE(szTemp), szDecimalPlaces, _val);
			if (reject_valid && nDecimalPlaces > 0) {//! 舍弃无效的小数点位,比如小数点后无效的0
				for (size_t i = strlen(szTemp); i > 0; --i) {
					if (szTemp[i] == _T('\0')) {
						continue;
					}
					if (szTemp[i] == _T('0')) {
						szTemp[i] = 0;
					}
					else if (szTemp[i] == _T('.')) {
#if 0
						szTemp[i] = _T('0');
						szTemp[i + 2] = 0;
#else
						szTemp[i] = 0;//! 去掉 .0 情况
#endif
						break;
					}
					else {
						break;
					}
				}
			}
			return szTemp;
		}
		/**判断是否是函数签名需要的字符
*/
		static bool IsFunsignCh(char _Ch) {
			for (int i = 0; i < ARRAYSIZE(CONST_FUNSIGN_CH); ++i) {
				if (CONST_FUNSIGN_CH[i] == _Ch) {
					return true;
				}
			}
			return false;
		}
		/**判断是否是字母
		*/
		static bool IsLetterCh(char _Ch) {
			for (int i = 0; i < ARRAYSIZE(CONST_LETTER_CH); ++i) {
				if (CONST_LETTER_CH[i] == _Ch) {
					return true;
				}
			}
			return false;
		}
		//通用检测
		static bool IsDestStrCh(const TCHAR* dest, TCHAR src) {
			for (size_t i = 0; i < _tcslen(dest); ++i) {
				if (dest[i] == src) {
					return true;
				}
			}
			return false;
		}
		/*判断是否为实数(正)*/
		static bool IsRealNumber(TCHAR tch) {
			for (int i = 0; i < ARRAYSIZE(CONST_NUMBER_REAL); ++i) {
				if (CONST_NUMBER_CH[i] == tch) {
					return true;
				}
			}
			return false;
		}
		/**判断是否是数字
		*/
		static bool IsNumberCh(char _Ch) {
			for (int i = 0; i < ARRAYSIZE(CONST_NUMBER_CH); ++i) {
				if (CONST_NUMBER_CH[i] == _Ch) {
					return true;
				}
			}
			return false;
		}
		/**判断是否是十六进制字符
		*/
		static bool IsHexCh(char _Ch) {
			for (int i = 0; i < ARRAYSIZE(CONST_HEX_CH); ++i) {
				if (CONST_HEX_CH[i] == _Ch) {
					return true;
				}
			}
			return false;
		}
		/**判断是否是数字字符串
		*/
		static bool IsNumberStr(const TCHAR* _Str) {
			if (!_Str)
				return false;
			size_t nStr = _tcslen(_Str);

			int sub = 0;
			int add = 0;
			int point = 0;

			for (size_t i = 0; i < nStr; ++i)
			{
				bool found = false;
				for (size_t j = 0; j < ARRAYSIZE(CONST_NUMBER_REAL_NUM); ++j) {
					if (CONST_NUMBER_REAL_NUM[j] == _Str[i])
					{
						if (_Str[i] == _T('.'))
							++point;
						else if (_Str[i] == _T('-'))
							++sub;
						else if (_Str[i] == _T('+'))
							++add;
						found = true;
						break;
					}
				}
				if (!found)
					return false;
			}
			return sub <= 1 && add <= 1 && point <= 1;
		}
		static bool IsNumberStrA(const char* _Str) {
			if (!_Str)
				return false;
			size_t nStr = strlen(_Str);

			int sub = 0;
			int add = 0;
			int point = 0;

			for (size_t i = 0; i < nStr; ++i)
			{
				bool found = false;
				for (size_t j = 0; j < ARRAYSIZE(CONST_NUMBER_REAL_NUM_A); ++j) {
					if (CONST_NUMBER_REAL_NUM_A[j] == _Str[i])
					{
						if (_Str[i] == '.')
							++point;
						else if (_Str[i] == '-')
							++sub;
						else if (_Str[i] == '+')
							++add;
						found = true;
						break;
					}
				}
				if (!found)
					return false;
			}
			return sub <= 1 && add <= 1 && point <= 1;
		}
		static bool IsNumberStrW(const wchar_t* _Str) {
			if (!_Str)
				return false;
			size_t nStr = wcslen(_Str);

			int sub = 0;
			int add = 0;
			int point = 0;

			for (size_t i = 0; i < nStr; ++i)
			{
				bool found = false;
				for (size_t j = 0; j < ARRAYSIZE(CONST_NUMBER_REAL_NUM_W); ++j) {
					if (CONST_NUMBER_REAL_NUM_W[j] == _Str[i])
					{
						if (_Str[i] == L'.')
							++point;
						else if (_Str[i] == L'-')
							++sub;
						else if (_Str[i] == L'+')
							++add;
						found = true;
						break;
					}
				}
				if (!found)
					return false;
			}
			return sub <= 1 && add <= 1 && point <= 1;
		}
		static int ErrorFatal(const char* _FunctionName, const char* _MethodOfDisposition, const char* _ErrorInfo, ...) {
			va_list _vlist = nullptr;
			va_start(_vlist, _ErrorInfo);
			auto nLength = _vscprintf(_ErrorInfo, _vlist) + 1;
			std::vector<char> vBuffer(nLength, '\0');
			auto nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, _ErrorInfo, _vlist);
			va_end(_vlist);
			std::string strMsg("Function: [");
			strMsg.append(_FunctionName).append("]").append("\r\n");
			if (nWritten > 0) {
				vBuffer.pop_back();
				strMsg.append("Message: \"").append(&vBuffer[0], vBuffer.size()).append("\"").append("\r\n");
			}
			strMsg.append("Result: \"").append("The application must be terminated.").append("\"").append("\r\n");
			if (_MethodOfDisposition) {
				strMsg.append("Disposition: \"").append(_MethodOfDisposition).append("\"").append("\r\n");
			}
			else {
				strMsg.append("Disposition: \"").append("Contact the developer or maintainer.").append("\"").append("\r\n");
			}
			::MessageBoxA(NULL, strMsg.c_str(), "sk project ***", MB_OK | MB_TOPMOST);
			std::exit(0);
			return 0;
		}
		//! 数字类型转字符串
		template<typename Type>
		static std::string num2string(const Type& value, int bits = 2) {
			std::string result;
			auto type = typeid(value).name();
			if (!strcmp("long double", type) ||
				!strcmp("double", type) ||
				!strcmp("float", type)) {
				std::stringstream ss;
				ss << std::fixed << std::setprecision(bits) << value;
				result = ss.str();
			}
			else {
				result = std::to_string(value);
			}
			return result;
		}
		/*定长结构体解析*/
		template<typename T>
		static size_t OneUnknownParseStruct(__in const char* buf, __in const size_t& nbuf, __out std::vector<T>& Out) {
			Out.clear();
			if (!buf || nbuf <= 0) return 0;
			size_t pos = 0;
			const size_t nnode = sizeof(T);
			do {
				if (pos >= nbuf) break;
				auto data = (T*)(buf + pos);
				if (!data) break;
				Out.emplace_back(*data);
				pos += nnode;
			} while (1);
			return Out.size();
		}

		template<typename T>
		static size_t OneUnknownParseStruct(__in const char* buf, __in const size_t& nbuf_total, __in const size_t& nbuf_node, __out std::vector<T>& Out) {
			Out.clear();
			if (!buf || nbuf_total <= 0 || nbuf_node <= 0) return 0;
			size_t pos = 0;
			do {
				if (pos >= nbuf_total) break;
				auto data = (T*)(buf + pos);
				if (!data) break;
				Out.emplace_back(*data);
				pos += nbuf_node;
			} while (1);
			return Out.size();
		}

		template<typename T>
		static std::shared_ptr<std::vector<T>> OneUnknownParseStruct(__in const char* buf, __in const size_t& nbuf) {
			std::shared_ptr<std::vector<T>> result;
			if (!buf || nbuf <= 0) return result;
			size_t pos = 0;
			const size_t nnode = sizeof(T);
			do {
				if (!result) {
					result = std::make_shared<std::vector<T>>();
				}
				if (pos >= nbuf) { break; }
				auto data = (T*)(buf + pos);
				if (!data) { break; }
				result->emplace_back(*data);
				pos += nnode;
			} while (1);
			return result;
		}

		/*获取小数点后有效位数 -- */
		static unsigned int DoubleToDecimals(const double& src) {
			if (src == 0.0) return 0;
			auto newSrc = std::to_string(src);
			size_t dot = newSrc.find('.');
			if (dot != std::string::npos) {
				for (auto rit = newSrc.rbegin(); *rit != '.';) {
					if (*rit != '0') break;
					newSrc.erase(std::prev(newSrc.end()));
					++rit;
				}

				unsigned int decimal = 0;
				for (auto rit = newSrc.rbegin(); *rit != '.'; ++rit) {
					++decimal;
				}
				return decimal;
			}
			return 0;
		}
		/**字符串根据分隔符拆分
*	@strContend	原始字符串
*	@nDelimiter	拆分字符
*	@Output	拆分结果
*/
		static void OneUnknownParseA(__in std::string strContend, __in int nDelimiter, __out std::vector<std::string>& Output) {
			Output.clear();
			if (!strContend.length()) {
				return;
			}
			size_t found = 0;
			while ((found = strContend.find(nDelimiter)) != std::string::npos) {
				std::string strSession = "";
				strSession.insert(strSession.length(), strContend.data(), found);
				strContend.erase(0, found + 1);
				Output.push_back(strSession);
			}
			if (strContend.length()) {
				Output.push_back(strContend);
			}
		}
		static void OneUnknownParseW(__in std::wstring strContend, __in int nDelimiter, __out std::vector<std::wstring>& Output) {
			Output.clear();
			if (!strContend.length()) {
				return;
			}
			size_t found = 0;
			while ((found = strContend.find(nDelimiter)) != std::wstring::npos) {
				std::wstring strSession = L"";
				strSession.insert(strSession.length(), strContend.data(), found);
				strContend.erase(0, found + 1);
				Output.push_back(strSession);
			}
			if (strContend.length()) {
				Output.push_back(strContend);
			}
		}
		static void OneUnknownParse(__in sk::string strContend, __in int nDelimiter, __out std::vector<sk::string>& Output) {
			Output.clear();
			if (!strContend.length()) {
				return;
			}
			size_t found = 0;
			while ((found = strContend.find(nDelimiter)) != sk::string::npos) {
				sk::string strSession = _T("");
				strSession.insert(strSession.length(), strContend.data(), found);
				strContend.erase(0, found + 1);
				Output.push_back(strSession);
			}
			if (strContend.length()) {
				Output.push_back(strContend);
			}
		}

		//! 枚举指定目录的文件夹
		static void EnumDir(const std::string& _path, std::map<std::string, std::string>& _out, bool bSleepDirect = false) {
			if (_path.empty()) return;
			std::string path(_path);
			if (path[path.length() - 1] != '\\' && path[path.length() - 1] != '/') {
				path.push_back('\\');
			}
			path.append("*.*");
			_finddata_t  FindData;
			auto FileIndes = _findfirst(path.c_str(), &FindData);
			if (FileIndes < 0) {
				return;
			}
			while (_findnext(FileIndes, &FindData) >= 0) {
				if (strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0) {
					continue;
				}
				if (FindData.attrib & _A_SUBDIR) {
					_out.insert({ {FindData.name,_path + "\\" + FindData.name} });
					if (bSleepDirect) {
						EnumDir((_path + FindData.name).c_str(), _out, true);
					}
					continue;
				}
				//mapFiles.insert(std::make_pair(FindData.name, FindData));
			}
		}

		/**枚举指定目录文件
		*	@filter	过滤条件 *.* 表示所有类型文件
		*	@Path	指定扫描目录		C:\\direct
		*	@mapFiles	存放扫描结果
		*	@bSleepDirect	是否扫描子目录（递归）
		*/
		static void EnumFile(std::string Path, std::map<std::string, _finddata_t>& mapFiles, const char* filter = "*.*", bool bSleepDirect = false) {
			if (Path.empty()) {
				return;
			}
			if (Path[Path.length() - 1] != '\\' && Path[Path.length() - 1] != '/') {
				Path.push_back('\\');
			}
			std::string Direct(Path);
			Direct.append(filter);
			_finddata_t  FindData;
			auto FileIndes = _findfirst(Direct.c_str(), &FindData);
			if (FileIndes < 0) {
				return;
			}
			if (!(FindData.attrib & _A_SUBDIR)) {
				mapFiles.insert(std::make_pair(FindData.name, FindData));
			}
			else if (bSleepDirect) {
				EnumFile((Path + FindData.name).c_str(), mapFiles, filter, true);
			}
			while (_findnext(FileIndes, &FindData) >= 0) {
				if (strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0) {
					continue;
				}
				if (FindData.attrib & _A_SUBDIR) {
					if (bSleepDirect) {
						EnumFile((Path + FindData.name).c_str(), mapFiles, filter, true);
					}
					continue;
				}
				mapFiles.insert(std::make_pair(FindData.name, FindData));
			}
			_findclose(FileIndes);
		}
		static HMODULE GetSelfModuleHandle() {
			MEMORY_BASIC_INFORMATION mbi;
			return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
		}
		//!@ GetCurrentProcessName: Gets the current process name
		static DWORD GetCurrentProcessNameA(std::string& strModuleProcessName, HMODULE hModule = nullptr) {
			strModuleProcessName.clear();
			char szModuleFileFullPath[_MAX_PATH] = { 0 };
			const DWORD dwModuleFileNameLength = ::GetModuleFileNameA(hModule, szModuleFileFullPath, _MAX_PATH);
			if (dwModuleFileNameLength > 0) {
				for (DWORD i = dwModuleFileNameLength; i > 0; --i) {
					if (szModuleFileFullPath[i] == '/' || szModuleFileFullPath[i] == '\\') {
						for (DWORD j = i + sizeof(char), k = 0; j < dwModuleFileNameLength; ++j, ++k) {
#if 0//! 带后缀
							if (szModuleFileFullPath[j] != '.') {
								strModuleProcessName.push_back(szModuleFileFullPath[j]);
							}
							else {
								break;
							}
#else
							strModuleProcessName.push_back(szModuleFileFullPath[j]);
#endif
						}
						break;
					}
				}
			}
			return static_cast<DWORD>(strModuleProcessName.length());
		}
		static std::string GetFileNameByFullPath(const std::string& fullpath)
		{
			std::string result;
			for (auto rit = fullpath.rbegin(); rit != fullpath.rend(); ++rit)
			{
				if (*rit != '/' && *rit != '\\')
					result.append(1, *rit);
				else
					break;
			}
			if (!result.empty())
				std::reverse(result.begin(), result.end());
			return result;
		}
		static std::string GetCurrentProcessNameA(HMODULE hModule = nullptr) {
			std::string result;
			char szModuleFileFullPath[_MAX_PATH] = { 0 };
			const DWORD dwModuleFileNameLength = ::GetModuleFileNameA(hModule, szModuleFileFullPath, _MAX_PATH);
			if (dwModuleFileNameLength > 0) {
				for (DWORD i = dwModuleFileNameLength; i > 0; --i) {
					if (szModuleFileFullPath[i] == '/' || szModuleFileFullPath[i] == '\\') {
						for (DWORD j = i + sizeof(char), k = 0; j < dwModuleFileNameLength; ++j, ++k) {
#if 0//! 带后缀
							if (szModuleFileFullPath[j] != '.') {
								strModuleProcessName.push_back(szModuleFileFullPath[j]);
							}
							else {
								break;
							}
#else
							result.push_back(szModuleFileFullPath[j]);
#endif
						}
						break;
					}
				}
			}
			return result;
		}
		static sk::string GetCurrentProcessName(HMODULE hModule = nullptr) {
			sk::string result;
			TCHAR szModuleFileFullPath[_MAX_PATH] = { 0 };
			const DWORD dwModuleFileNameLength = ::GetModuleFileName(hModule, szModuleFileFullPath, _MAX_PATH);
			if (dwModuleFileNameLength > 0) {
				for (DWORD i = dwModuleFileNameLength; i > 0; --i) {
					if (szModuleFileFullPath[i] == _T('/') || szModuleFileFullPath[i] == _T('\\')) {
						for (DWORD j = i + sizeof(char), k = 0; j < dwModuleFileNameLength; ++j, ++k) {
#if 0//! 带后缀
							if (szModuleFileFullPath[j] != '.') {
								strModuleProcessName.push_back(szModuleFileFullPath[j]);
							}
							else {
								break;
							}
#else
							result.push_back(szModuleFileFullPath[j]);
#endif
						}
						break;
					}
				}
			}
			return result;
		}
		//!@ 应用程序实例标计
		//!@ result true : 实例不存在 false : 实例已经存在
		static bool ProgramInstanceMark(const std::string& ProgramName)
		{
			if (ProgramName.empty())
			{
				return false;
			}
			auto hMutex = ::CreateMutexA(NULL, FALSE, ProgramName.c_str());
			if (hMutex && (::GetLastError() == ERROR_ALREADY_EXISTS))
			{
				::CloseHandle(hMutex);
				hMutex = NULL;
				return false;
			}
			return true;
		}
		//!@ 移除进程名后缀
		static sk::stringa RemoveProcessNameSuffix(const  sk::stringa& processName) {
			sk::stringa result(processName);
			for (auto rit = result.rbegin(); rit != result.rend(); ++rit)
			{
				if (*rit == '.')
				{
					*rit = 0;
					break;
				}
			}
			return result.c_str();
		}
		//!@ 移除进程名后缀
		static sk::stringw RemoveProcessNameSuffix(const sk::stringw& processName) {
			sk::stringw result(processName);
			for (auto rit = result.rbegin(); rit != result.rend(); ++rit)
			{
				if (*rit == '.')
				{
					*rit = 0;
					break;
				}
			}
			return result.c_str();
		}

		//!@ GetCurrentProcessDirectory: Gets the current process directory
		static std::shared_ptr<char> GetCurrentProcessDirectoryA(HMODULE hModule = nullptr) {
#if 0
			std::shared_ptr<char> buffer(new char[_MAX_PATH], [](char* pbuffer) { delete[] pbuffer; });
			_getcwd(buffer.get(), _MAX_PATH);
#else
			char szTemp[_MAX_PATH] = { 0 };
			::GetModuleFileNameA(hModule, szTemp, _MAX_PATH);

			char* pFind = nullptr;

			pFind = strrchr(szTemp, '\\');
			if (!pFind) {
				pFind = strrchr(szTemp, '/');
			}
			if (pFind) {
				szTemp[pFind - szTemp] = 0;
				strcat_s(szTemp, ARRAYSIZE(szTemp), "\\");
			}
			else {
				return nullptr;
			}
			std::shared_ptr<char> buffer(new char[_MAX_PATH], [](char* pbuffer) { delete[] pbuffer; });
			memset(buffer.get(), 0, _MAX_PATH);
			memcpy(buffer.get(), szTemp, strlen(szTemp));
#endif
			return buffer;
		}///GetCurrentProcessDirectoryA
		static std::shared_ptr<wchar_t> GetCurrentProcessDirectoryW() {
#if 0
			std::shared_ptr<wchar_t> buffer(new wchar_t[_MAX_PATH], [](wchar_t* pbuffer) { delete[] pbuffer; });
			_wgetcwd(buffer.get(), _MAX_PATH);
#else
			wchar_t szTemp[_MAX_PATH] = { 0 };
			::GetModuleFileNameW(NULL, szTemp, _MAX_PATH);
			wchar_t* pFind = nullptr;
			pFind = wcsrchr(szTemp, '\\');
			if (!pFind) {
				pFind = wcsrchr(szTemp, '/');
			}
			if (pFind) {
				szTemp[pFind - szTemp] = 0;
				wcscat_s(szTemp, ARRAYSIZE(szTemp), L"\\");
			}
			else {
				return nullptr;
			}
			std::shared_ptr<wchar_t> buffer(new wchar_t[_MAX_PATH], [](wchar_t* pbuffer) { delete[] pbuffer; });
			memset(buffer.get(), 0, _MAX_PATH);
			wcscpy_s(buffer.get(), _MAX_PATH, szTemp);
#endif
			return buffer;
		}///GetCurrentProcessDirectoryW
		//! 检查目录是否存在
		static bool CheckDirectory(const char* path, bool IscCarryFileName = true) {
			return _access(path, 0) == 0;
		}
		//! 返回上一级目录
		static std::string ReturnPrevDirectory(const char* fullpath, int level = 1) {
			if (!fullpath || !level) return fullpath;
			std::string FullPath(fullpath);

			for (auto it = FullPath.begin(); it != FullPath.end();) {
				if (*it == '/' || *it == '\\') {
					*it = '/';
					auto prev = std::prev(it);
					if (prev != FullPath.begin()) {
						if (*prev == '/') {
							it = FullPath.erase(it);
							continue;
						}
					}
				}
				++it;
			}
			if (FullPath.empty()) return FullPath;
#if 0
			if (*std::prev(FullPath.end()) == '/') {
				FullPath.pop_back();
			}
#else
			if (*std::prev(FullPath.end()) != '/') {
				FullPath.push_back('/');
			}
#endif

			int Level = 0;
			for (auto rit = FullPath.rbegin(); rit != FullPath.rend();) {
				if (*rit == '/') {
					if (FullPath.find('/') == FullPath.rfind('/')) {
						break;
					}
					if (Level == level)
						break;
					++Level;
				}
				rit = std::string::reverse_iterator(FullPath.erase((++rit).base()));
			}

			return FullPath;
		}

		/**
		* 检查目录是否存在，如果不存在则创建目录
		*/
		//!@ CreateDirectory: create muti-level directory
		//!@ _Path: example"C:\\TEST1\\TEST2\\ || C://TEST1//TEST2"
		static int CreateDirectoryA(const char* _Path, bool IscCarryFileName = true) {
			if (!_access(_Path, 0)) {
				return 0;
			}
			char Path[_MAX_PATH] = { 0 };
			strncpy_s(Path, _Path, _MAX_PATH);
			if (IscCarryFileName) {
				auto chLast = Path[strlen(Path) - 1];
				if (chLast != 0) {
					if (chLast != '\\' && chLast != '/') {
						auto find1 = strrchr(Path, '\\');
						auto find2 = strrchr(Path, '/');
						if (find1 > find2) {
							find1++;
							Path[find1 - Path] = 0;
						}
						else {
							find2++;
							Path[find2 - Path] = 0;
						}
					}
				}
			}
			int iRet = 0;
			size_t iLen = strlen(Path);
			auto pPath = new char[iLen * 2];
			memset(pPath, 0x00, iLen * 2);
			memcpy(pPath, Path, iLen);
			for (decltype(iLen) i = 0; i < iLen; i++) {
				if (pPath[i] == '\\' || pPath[i] == '/') {
					pPath[i] = '\0';
					iRet = _access(pPath, 0);
					if (iRet != 0) {
						iRet = _mkdir(pPath);
						if (iRet != 0)
							return -1;
					}
					pPath[i] = '/';
				}
			}
			iRet = _mkdir(pPath);
			delete[] pPath;
			pPath = nullptr;
			return iRet;
		}///CreateDirectory
		//!@ Windows 检测指定进程名是否存在
		//! 如果返回值 不为零即存在
		static DWORD GetProcessidFromName(LPCTSTR name) {
			PROCESSENTRY32 pe;
			DWORD id = 0;
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pe.dwSize = sizeof(PROCESSENTRY32);
			if (!Process32First(hSnapshot, &pe))
				return 0;
			while (1) {
				pe.dwSize = sizeof(PROCESSENTRY32);
				if (Process32Next(hSnapshot, &pe) == FALSE)
					break;
				if (_tcscmp(pe.szExeFile, name) == 0) {
					id = pe.th32ProcessID;
					break;
				}
			}
			CloseHandle(hSnapshot);
			return id;
		}
		//! 打开外部程序
		static bool OpenProcess(const sk::stringa& procPath, const sk::stringa& Cmd, PROCESS_INFORMATION& outProcinfo, bool IsShow = true, DWORD dwCreateFlag = CREATE_NEW_CONSOLE) {
			STARTUPINFOA stStartUpInfo;
			::memset(&stStartUpInfo, 0, sizeof(stStartUpInfo));
			stStartUpInfo.cb = sizeof(stStartUpInfo);
			stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
			IsShow ? stStartUpInfo.wShowWindow = SW_SHOW : stStartUpInfo.wShowWindow = SW_HIDE;

			::memset(&outProcinfo, 0, sizeof(outProcinfo));
			sk::stringa cmd(Cmd);
			cmd.insert(0, " ", strlen(" "));
			return ::CreateProcessA(
				procPath.c_str(),
				(char*)cmd.c_str(),
				NULL,
				NULL,
				false,
				dwCreateFlag,
				NULL,
				NULL,
				&stStartUpInfo,
				&outProcinfo);
		}



		//!@ LoadFile: Load the file into memory
		//!@ _Size: Actual load content size	
		static std::shared_ptr<char> LoadFile(const char* _Path, long& _Size, const char* _Mode = "rb", int _ShFlag = _SH_DENYNO) {
			FILE* pFile = _fsopen(_Path, _Mode, _ShFlag);
			if (!pFile) {
				return nullptr;
			}
			fseek(pFile, 0, SEEK_END);
			long lSize = ftell(pFile);
			rewind(pFile);
			std::shared_ptr<char> buffer(new char[lSize], [](char* pbuffer) { delete[] pbuffer; });
			size_t result = fread(buffer.get(), 1, lSize, pFile);
			if (result != lSize) {
				return nullptr;
			}
			fclose(pFile);
			_Size = lSize;
			return buffer;
		}///
		//!@ LoadFile
		//!@ LoadFile 自动跳过BOM头
		//!@ BOM头
		//!@ [FE FF] UTF16BE
		//!@ [FF FE] UTF16LE
		//!@	[EF BB BF] UTF8
		static std::vector<char> LoadFile(const char* _Path, const char* _Mode = "rb", int _ShFlag = _SH_DENYNO)
		{
			std::vector<char> result;
			FILE* pFile = _fsopen(_Path, _Mode, _ShFlag);
			if (!pFile) { return result; }
			fseek(pFile, 0, SEEK_END);
			long lSize = ftell(pFile);
			if (lSize <= 0)
			{
				fclose(pFile);
				return result;
			}
			rewind(pFile);
			result.resize(lSize);
			size_t nread = fread(&result[0], 1, lSize, pFile);
			fclose(pFile);

			if (result.size() >= 2)
			{
				//! UTF16BE
				if (unsigned char(result.at(0)) == 0xFE && unsigned char(result.at(1)) == 0xFF)
				{
					for (int i = 0; i < 2; ++i)
					{
						result.erase(result.begin());
					}
				}
				//! UTF16LE
				else if (unsigned char(result.at(0)) == 0xFF && unsigned char(result.at(1)) == 0xFE)
				{
					for (int i = 0; i < 2; ++i)
					{
						result.erase(result.begin());
					}
				}
				if (result.size() >= 3)
				{
					//! UTF8
					if (unsigned char(result.at(0)) == 0xEF && unsigned char(result.at(1)) == 0xBB && unsigned char(result.at(2)) == 0xBF)
					{
						for (int i = 0; i < 3; ++i)
						{
							result.erase(result.begin());
						}
					}
				}
			}
			return result;
		}///LoadFile


					//! 解析HTTP URL
			//! 键值对的Key要符合函数签名约定
			//! 键值对的Key,Value进行小写转换
		static bool ParseHttpUrl(const sk::stringa& _UrlRequestData, std::map<sk::stringa, sk::stringa>& _Out) {
			std::vector<std::string> vrParameter;
			OneUnknownParseA(_UrlRequestData, '&', vrParameter);
			for (const auto& it : vrParameter) {
				std::vector<std::string> vrParamNode;
				OneUnknownParseA(it, '=', vrParamNode);
				if (vrParamNode.size() == 2) {
					sk::stringa Key(*vrParamNode.begin()), Value(*(vrParamNode.begin() + 1));
					for (auto it = Key.begin(); it != Key.end();) {
						if (!IsFunsignCh(*it)) { it = Key.erase(it); }
						else { ++it; }
					}
					std::transform(Key.begin(), Key.end(), Key.begin(), tolower);
					std::transform(Value.begin(), Value.end(), Value.begin(), tolower);
					_Out.insert({ {Key,Value} });
				}
			}return !_Out.empty();
		}

		static UINT64 GetCpuID()
		{
			UINT64 result = 0UL;
#if defined(_WIN64)//64位编译器
			const BYTE ShellCode[] = "\xB8\x01\x00\x00\x00"/*mov eax 1*/"\x0F\xA2"/*cpuid*/"\xC3"/*ret*/;
			void* pVirtualAlloc = ::VirtualAlloc(NULL, _countof(ShellCode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (!pVirtualAlloc)
				return result;
			memcpy(pVirtualAlloc, ShellCode, _countof(ShellCode));
			typedef UINT64(*fnGet)(void);
			fnGet ExecCode = (fnGet)pVirtualAlloc;
			result = ExecCode();
			::VirtualFree(pVirtualAlloc, _countof(ShellCode), MEM_RELEASE | MEM_COMMIT);
#else
			__asm
			{
				mov eax, 1
				cpuid
				mov result, eax
			}
#endif
			return result;
		}


		static UINT64 GetCpuidEx()
		{
			UINT64 result = 0;
			unsigned int CPUInfo[4] = { 0 };
			unsigned int InfoType = 1;
			__cpuid((int*)(void*)CPUInfo, (int)(InfoType));
			result = CPUInfo[3];
			result <<= 8 * 4;
			result |= CPUInfo[0];
			if (result <= 0)
				return 0;
			return result;
		}

		static BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) {
			TOKEN_PRIVILEGES tp;
			LUID luid;
			if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
				return FALSE;
			}
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			if (bEnablePrivilege) {
				tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			}
			else {
				tp.Privileges[0].Attributes = 0;
			}
			if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
				return FALSE;
			}
			if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
				return FALSE;
			}
			return TRUE;
		}

		//! 解析地址: 127.0.0.1:7001,127.0.0.1:8801 ...
		//sk::stringa host = "127.0.0.1:8800,k34ubll5.oicp.io:8801,k34ubll5.oicp.io:8802,k34ubll5.oicp.io:8803,";
		static void ParseIPAddr(__in const std::string& inAddr, __out std::multimap<sk::stringa, int>& outAddrs) {
			outAddrs.clear();
			if (inAddr.find(':') == std::string::npos) { return; }
			if (inAddr.find(',') != sk::stringa::npos) {
				std::vector<sk::stringa> vrAddresss;
				sk::Helper::OneUnknownParseA(inAddr, ',', vrAddresss);
				for (const auto& addr_node : vrAddresss) {
					std::vector<sk::stringa> vrAddrs;
					sk::Helper::OneUnknownParseA(addr_node, ':', vrAddrs);
					if (vrAddrs.size() == 2) {
						bool exists = false;
						for (const auto& addr : outAddrs) {
							if (addr.first.compare(vrAddrs.at(0)) == 0 && addr.second == atoi(vrAddrs.at(1).c_str())) {
								exists = true;
								break;
							}
						}
						if (!exists) {
							outAddrs.insert({ {vrAddrs.at(0),atoi(vrAddrs.at(1).c_str())} });
						}
					}
				}
			}
			else {
				std::vector<sk::stringa> vrAddrs;
				sk::Helper::OneUnknownParseA(inAddr, ':', vrAddrs);
				if (vrAddrs.size() == 2) {
					outAddrs.insert({ {vrAddrs.at(0),atoi(vrAddrs.at(1).c_str())} });
				}
			}
		}

		//!@ 获取本机正在使用的MAC
		static int GetMacBeingUse(__out std::vector<std::string>& out, bool all = true)
		{
			out.clear();
			PIP_ADAPTER_INFO pAdapterInfo;
			PIP_ADAPTER_INFO pAdapter = NULL;
			DWORD dwRetVal = 0;
			UINT i;

			ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
			pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
			if (pAdapterInfo == NULL)
			{
				/*printf("Error allocating memory needed to call GetAdaptersinfo\n");*/
				return -1;
			}
			// Make an initial call to GetAdaptersInfo to get
			// the necessary size into the ulOutBufLen variable
			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
			{
				free(pAdapterInfo);
				pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
				if (pAdapterInfo == NULL) {
					/*printf("Error allocating memory needed to call GetAdaptersinfo\n");*/
					return -1;
				}
			}
			if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
			{
				pAdapter = pAdapterInfo;
				while (pAdapter) {
					if (pAdapter->DhcpEnabled)
					{
						std::string mac;
						char temp[100] = { 0 };
						int ntemp = 0;

						for (i = 0; i < pAdapter->AddressLength; i++)
						{
							ntemp = sprintf_s(temp, "%.2X", (int)pAdapter->Address[i]);
							if (ntemp > 0)
							{
								mac.append(temp, ntemp);
							}
						}

						if (all && pAdapter->LeaseObtained > 10 && !mac.empty())
						{
							out.emplace_back(mac);
						}
						else if (!all && !mac.empty())
						{
							out.emplace_back(mac);
						}
					}
					pAdapter = pAdapter->Next;
				}
			}
			else
			{
				printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
			}
			if (pAdapterInfo)
			{
				free(pAdapterInfo);
			}
			return 0;
		}

		//!@ 获取本机所有网卡MAC
		static std::string GetLocalMachineAllNetWorkCardMAC(std::set<sk::stringa>& macs) {
			std::string resultMacStr;
			IP_ADAPTER_INFO* oldptr = nullptr;
			auto pIPAdapterInfo = new IP_ADAPTER_INFO;
			auto nIPAdapterInfo = static_cast<ULONG>(sizeof(IP_ADAPTER_INFO));
			auto result = ::GetAdaptersInfo(pIPAdapterInfo, &nIPAdapterInfo);
			if (ERROR_BUFFER_OVERFLOW == result) {
				SK_DELETE_PTR_BUFFER(pIPAdapterInfo);
				pIPAdapterInfo = (PIP_ADAPTER_INFO)new unsigned char[nIPAdapterInfo];
				result = ::GetAdaptersInfo(pIPAdapterInfo, &nIPAdapterInfo);
			}
			oldptr = pIPAdapterInfo;
			if (ERROR_SUCCESS == result) {
				do {
					if (!pIPAdapterInfo) {
						break;
					}
					sk::stringa mac;
					for (decltype(pIPAdapterInfo->AddressLength) i = 0; i < pIPAdapterInfo->AddressLength; ++i) {
						char szTemp[5] = { 0 };
						sprintf_s(szTemp, _countof(szTemp), "%02X", pIPAdapterInfo->Address[i]);
						mac.append(szTemp);
					}
					if (!mac.empty()) {
						macs.insert(mac);
					}
					pIPAdapterInfo = pIPAdapterInfo->Next;
				} while (1);
			}
			SK_DELETE_PTR(oldptr);
			for (const auto& mac : macs)
			{
				resultMacStr.append(mac).append(",");
			}
			if (!resultMacStr.empty())
			{
				resultMacStr.pop_back();
			}
			return resultMacStr;
		}
		//! 运行CMD命令，并获取返回信息
		static std::string ExecCMD(std::string pszCmd) {
			// 创建匿名管道
			SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
			HANDLE hRead, hWrite;
			if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
				return "";
			}
			// 设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWrite
			STARTUPINFOA si = { sizeof(STARTUPINFOA) };
			GetStartupInfoA(&si);
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			si.wShowWindow = SW_HIDE;
			si.hStdError = hWrite;
			si.hStdOutput = hWrite;
			// 启动命令行
			PROCESS_INFORMATION pi;
			if (!CreateProcessA(NULL, (LPSTR)pszCmd.c_str(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
				return "Cannot create process";
			}
			::CloseHandle(hWrite);
			// 读取命令行返回值
			std::string strRetTmp;
			char buff[1024] = { 0 };
			DWORD dwRead = 0;
			strRetTmp = buff;
			while (ReadFile(hRead, buff, 1024, &dwRead, NULL)) {
				strRetTmp.append(buff, dwRead);
			}
			::CloseHandle(hRead);
			return strRetTmp;
		}
#pragma pack(push,1)
		//!@ Machine Hardware Information
		typedef struct tagMachineHardwareInformation {
			char ProcessorID[16];	//! CPU ID  BFEBFBFF000706E5
			//char BIOSSerialNumber[256];//! BIOS SN ACE4_2E00_9578_AB39
			//char DiskSerialNumber[256];//! DISK SN
			tagMachineHardwareInformation() { memset(this, 0x00, sizeof(*this)); }
			void operator=(const tagMachineHardwareInformation& obj) { memcpy(this, &obj, sizeof(*this)); }
		}MachineHardwareInformation;
#pragma pack(pop)
		//!@ Window 获取本地计算机硬盘信息 主板(Manufacturer)、硬盘(SerialNumber)、CPU(ProcessorID)
		static void GetLocalMachineHardwareInformation(MachineHardwareInformation& result) {

			memset(&result, 0x00, sizeof(result));
			/*
			"cmd: wmic CPU get ProcessorID"
			"cmd: wmic diskdrive get SerialNumber"
			*/
			const char* pid = "ProcessorId";
			const char* snb = "SerialNumber";

			sk::stringa ProcessorId = ExecCMD("wmic CPU get ProcessorId");
			/*sk::stringa MACAddress = ExecCMD("wmic nicconfig get macaddress");*/
			/*sk::stringa DiskSerialNumber = ExecCMD("wmic diskdrive get SerialNumber");*/
			/*sk::stringa BIOSSerialNumber = ExecCMD("wmic bios get serialnumber");*/

			if (ProcessorId.empty() /*&& DiskSerialNumber.empty() && BIOSSerialNumber.empty()*/)
			{
				return;
			}

			if (!ProcessorId.empty())
			{
				for (auto it = ProcessorId.begin(); it != ProcessorId.end();)
				{
					if (*it == '\r' || *it == '\n' || *it == '.' || *it == ' ')
					{
						it = ProcessorId.erase(it);
						continue;
					}
					++it;
				}
				auto pos = ProcessorId.find(pid);
				if (pos != sk::stringa::npos)
				{
					ProcessorId.erase(pos, strlen(pid));
				}

				SafeCopyA(result.ProcessorID, ProcessorId.c_str(), _countof(result.ProcessorID));
			}
#if 0
			if (!DiskSerialNumber.empty())
			{
				for (auto it = DiskSerialNumber.begin(); it != DiskSerialNumber.end();)
				{
					if (*it == '\r' || *it == '\n' || *it == '.' || *it == ' ')
					{
						it = DiskSerialNumber.erase(it);
						continue;
					}
					++it;
				}
				auto pos = DiskSerialNumber.find(snb);
				if (pos != sk::stringa::npos)
				{
					DiskSerialNumber.erase(pos, strlen(snb));
				}
				SafeCopyA(result.DiskSerialNumber, DiskSerialNumber.c_str(), _countof(result.DiskSerialNumber));
			}

			if (!BIOSSerialNumber.empty())
			{
				for (auto it = BIOSSerialNumber.begin(); it != BIOSSerialNumber.end();)
				{
					if (*it == '\r' || *it == '\n' || *it == '.' || *it == ' ')
					{
						it = BIOSSerialNumber.erase(it);
						continue;
					}
					++it;
				}
				auto pos = BIOSSerialNumber.find(snb);
				if (pos != sk::stringa::npos)
				{
					BIOSSerialNumber.erase(pos, strlen(snb));
				}
				SafeCopyA(result.BIOSSerialNumber, BIOSSerialNumber.c_str(), _countof(result.BIOSSerialNumber));
			}
#endif
		}



		//! 获取文件版本号

		static sk::string GetProgramVersion(const sk::string& filePath) {
			sk::string result;
			VS_FIXEDFILEINFO* pVsInfo;
			unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);

			int iVerInfoSize = GetFileVersionInfoSize(filePath.c_str(), NULL);
			if (iVerInfoSize != 0) {
				TCHAR* pBuf = NULL;
				while (!pBuf) {
					pBuf = new TCHAR[iVerInfoSize];
				}
				if (GetFileVersionInfo(filePath.c_str(), 0, iVerInfoSize, pBuf)) {
					if (VerQueryValue(pBuf, _T("\\"), (void**)&pVsInfo, &iFileInfoSize)) {
						auto len = _stprintf_s(pBuf, iVerInfoSize, _T("%d.%d.%d.%d"), HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
						result.insert(0, pBuf, len);
					}
				}
				SK_DELETE_PTR_BUFFER(pBuf);
			}
			return result;
		}
		static sk::stringa GetProgramVersionA(const sk::stringa& filePath) {
			sk::stringa result;
			VS_FIXEDFILEINFO* pVsInfo;
			unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);

			int iVerInfoSize = GetFileVersionInfoSizeA(filePath.c_str(), NULL);
			if (iVerInfoSize != 0) {
				char* pBuf = NULL;
				while (!pBuf) {
					pBuf = new char[iVerInfoSize];
				}
				if (GetFileVersionInfoA(filePath.c_str(), 0, iVerInfoSize, pBuf)) {
					if (VerQueryValueA(pBuf, "\\", (void**)&pVsInfo, &iFileInfoSize)) {
						auto len = sprintf_s(pBuf, iVerInfoSize, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
						result.insert(0, pBuf, len);
					}
				}
				SK_DELETE_PTR_BUFFER(pBuf);
			}
			return result;
		}
		static sk::stringa GetProgramVersion(const char* pFilePath, sk::uint64& Major, sk::uint64& Minor, sk::uint64& Build, sk::uint64& Revision) {
			sk::stringa result;
			VS_FIXEDFILEINFO* pVsInfo;
			unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
			int iVerInfoSize = GetFileVersionInfoSizeA(pFilePath, NULL);
			if (iVerInfoSize != 0) {
				char* pBuf = NULL;
				while (!pBuf) {
					pBuf = new char[iVerInfoSize];
				}
				if (GetFileVersionInfoA(pFilePath, 0, iVerInfoSize, pBuf)) {
					if (VerQueryValueA(pBuf, "\\", (void**)&pVsInfo, &iFileInfoSize)) {
						Major = HIWORD(pVsInfo->dwFileVersionMS);
						Minor = LOWORD(pVsInfo->dwFileVersionMS);
						Build = HIWORD(pVsInfo->dwFileVersionLS);
						Revision = LOWORD(pVsInfo->dwFileVersionLS);
						auto len = sprintf_s(pBuf, iVerInfoSize, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
						result.insert(0, pBuf, len);
					}
				}
				SK_DELETE_PTR_BUFFER(pBuf);
			}
			return result;
		}

		static int MachineHardwareInformationGet(sk::MachineHardwareInformation& out)
		{
			out = sk::MachineHardwareInformation();
			out.CPUID = sk::Helper::GetCpuidEx();
			std::vector<std::string> MACs;
			if (sk::Helper::GetMacBeingUse(MACs, false) || MACs.empty() || !out.CPUID)
			{
				return -1;
			}
			size_t loop = __min(MACs.size(), _countof(out.MACS));
			for (size_t i = 0; i < loop; ++i)
			{
				sk::SafeCopyA(out.MACS[i], MACs[i].c_str(), _countof(out.MACS[i]));
			}
			return 0;
		}


		static HMODULE FindRemoteMod(HANDLE hProcess, const wchar_t* pszModShortName)
		{
			HMODULE _hModules[0x200] = { 0 };
			int _cbNeeded = 0;
			if (::EnumProcessModules(hProcess, _hModules, sizeof(_hModules), (unsigned long*)&_cbNeeded))
			{
				for (int _i = 0; _i < (_cbNeeded >> 2); _i++)
				{
					wchar_t _szModuleName[0x200] = { 0 };
					::GetModuleBaseNameW(hProcess, _hModules[_i], _szModuleName, sizeof(_szModuleName) / sizeof(_szModuleName[0]));
					if (!::_wcsicmp(_szModuleName, pszModShortName))
					{
						return HMODULE{ _hModules[_i] };
					}
				}
			}
			return nullptr;
		}

		static BOOL LoadDll(HANDLE hProcess, const wchar_t* dllpath)
		{
			// 计算路径长度，分配远程进程空间内虚拟内存
			SIZE_T dwRemoteBufLength = (wcslen(dllpath) + 1) * sizeof(wchar_t);

			// 在目标进程中分配一点空间来存放LoadLibraryW的参数     
			wchar_t* _pBuf = (wchar_t*)VirtualAllocEx(hProcess
				, NULL
				, dwRemoteBufLength
				, MEM_COMMIT
				, PAGE_READWRITE);
			if (!_pBuf)
				return FALSE;

			// 把dll的全路径写入到远程进程地址空间
			if (!(WriteProcessMemory(hProcess
				, _pBuf
				, (PVOID)dllpath
				, dwRemoteBufLength
				, NULL)))
				return FALSE;

			// 以LoadLibraryW为线程体来创建远程线程
			HANDLE _hThread = CreateRemoteThread(hProcess
				, NULL
				, 0
				, (PTHREAD_START_ROUTINE)(LoadLibraryW)
				, (PVOID)_pBuf
				, 0
				, NULL);
			// 内存泄漏?! 
			return (_hThread != INVALID_HANDLE_VALUE);
		}

		static void ViewProcessLoadDll(const sk::string& processName, std::multimap<HMODULE, sk::string>& out)
		{
			sk::string tmpstr = _TEXT("");
			PROCESSENTRY32 pinfo;
			pinfo.dwSize = sizeof(pinfo);
			DWORD dw_pid = 0;
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot == NULL)
			{
				return;
			}
			bool flag = Process32First(snapshot, &pinfo);
			while (flag)
			{
				tmpstr = sk::string(pinfo.szExeFile);
				if (tmpstr == processName)
				{
					dw_pid = pinfo.th32ProcessID;
					break;
				}
				flag = Process32Next(snapshot, &pinfo);
			}
			::CloseHandle(snapshot);
			if (dw_pid == 0) { return; }
			// 打开进程，具备写入权限和创建线程权限 
			HANDLE hProcess = ::OpenProcess(
				PROCESS_VM_READ |
				PROCESS_VM_WRITE |
				PROCESS_VM_OPERATION |
				PROCESS_SUSPEND_RESUME |
				PROCESS_CREATE_THREAD |
				PROCESS_QUERY_INFORMATION
				, FALSE,
				dw_pid);
			if (NULL == hProcess)
				return;
			// 检查是否是同一个用户会话中的进程
			HANDLE _hToken = INVALID_HANDLE_VALUE;
			if (!::OpenProcessToken(hProcess, TOKEN_READ, &_hToken))
			{
				::CloseHandle(hProcess);
				return;
			}
			::CloseHandle(_hToken);

			HMODULE _hModules[0x200] = { 0 };
			int _cbNeeded = 0;
			if (::EnumProcessModules(hProcess, _hModules, sizeof(_hModules), (unsigned long*)&_cbNeeded))
			{
				for (int _i = 0; _i < (_cbNeeded >> 2); _i++)
				{
					if (!_hModules[_i])
						continue;
					wchar_t _szModuleName[0x200] = { 0 };
					::GetModuleBaseNameW(hProcess, _hModules[_i], _szModuleName, sizeof(_szModuleName) / sizeof(_szModuleName[0]));
					wchar_t _szModuleFileName[0x200] = { 0 };
					auto nFileName = ::GetModuleFileName(_hModules[_i], _szModuleFileName, 0x200);
					if (nFileName > 0)
					{
						out.insert(std::make_pair(_hModules[_i], sk::string(_szModuleFileName, nFileName)));
					}
				}
			}
			::CloseHandle(hProcess);
		}

		static BOOL UnLoadDllAt(const sk::string& processName, const sk::string& moduleName)
		{
			sk::string tmpstr = _TEXT("");
			PROCESSENTRY32 pinfo;
			pinfo.dwSize = sizeof(pinfo);
			DWORD dw_pid = 0;
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot == NULL)
			{
				return FALSE;
			}
			bool flag = Process32First(snapshot, &pinfo);
			while (flag)
			{
				tmpstr = sk::string(pinfo.szExeFile);
				if (tmpstr == processName)
				{
					dw_pid = pinfo.th32ProcessID;
					break;
				}
				flag = Process32Next(snapshot, &pinfo);
			}
			::CloseHandle(snapshot);
			if (dw_pid == 0) { return FALSE; }
			// 打开进程，具备写入权限和创建线程权限 
			HANDLE hProcess = ::OpenProcess(
				PROCESS_VM_READ |
				PROCESS_VM_WRITE |
				PROCESS_VM_OPERATION |
				PROCESS_SUSPEND_RESUME |
				PROCESS_CREATE_THREAD |
				PROCESS_QUERY_INFORMATION
				, FALSE,
				dw_pid);
			if (NULL == hProcess)
				return FALSE;
			// 检查是否是同一个用户会话中的进程
			HANDLE _hToken = INVALID_HANDLE_VALUE;
			if (!::OpenProcessToken(hProcess, TOKEN_READ, &_hToken))
			{
				::CloseHandle(hProcess);
				return FALSE;
			}
			::CloseHandle(_hToken);

			HMODULE hFound = FindRemoteMod(hProcess, moduleName.c_str());
			if (!hFound)
			{
				::CloseHandle(hProcess);
				return FALSE;
			}

			unsigned long _tid = 0;
			HANDLE hResult = ::CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, hFound, 0, &_tid);
			if (!hResult)
			{
				::CloseHandle(hProcess);
				return FALSE;
			}

			::WaitForSingleObject(hResult, INFINITE);
			::CloseHandle(hResult);
			::CloseHandle(hProcess);
			return TRUE;
		}


	};
}///namespace sk


//#endif ///WIN32


/// /*新生联创（上海）**/
/// /*2019年07月30日 13:15:27|213**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_HELPER_H__F85DA_A25E5068_A25CCC88E211448_EB9C__TAIL__