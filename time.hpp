#if !defined(AFX_SKSTU_time_H__9B395_8D0501E1_E1746A695F328FB_AFBB__HEAD__)
#define AFX_SKSTU_time_H__9B395_8D0501E1_E1746A695F328FB_AFBB__HEAD__

namespace sk {
	namespace time {


		class TimeObject final {
		public:
			sk::uint32 GetMillisecond() const { return m_SysTime.wMilliseconds; }
			sk::uint32 GetSecond() const { return m_SysTime.wSecond; }
			sk::uint32 GetMinute() const { return m_SysTime.wMinute; }
			sk::uint32 GetHour() const { return m_SysTime.wHour; }
			sk::uint32 GetDay() const { return m_SysTime.wDay; }
			sk::uint32 GetMonth() const { return m_SysTime.wMonth; }
			sk::uint32 GetYear() const { return m_SysTime.wYear; }
			static TimeObject	Now() { TimeObject obj; obj.SetCurrentTime(); return obj; }
			void SetCurrentTime() { ::GetSystemTime(&m_SysTime); }
		private:
			SYSTEMTIME m_SysTime = { 0 };
		};


		class Time final {
		public:
			Time() {}
			~Time() = delete;
		public:
			//! @获取系统当前时间 如 201903121434568(ms)
			static long long GetSystime(bool IsMilliseconds = false) {
				SYSTEMTIME _sysTime;
				::GetLocalTime(&_sysTime);
				char szTime[100] = { 0 };
				if (IsMilliseconds) {
					sprintf_s(szTime, "%04d%02d%02d%02d%02d%02d%03d",
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay,
						_sysTime.wHour,
						_sysTime.wMinute,
						_sysTime.wSecond,
						_sysTime.wMilliseconds);
				}
				else {
					sprintf_s(szTime, "%04d%02d%02d%02d%02d%02d",
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay,
						_sysTime.wHour,
						_sysTime.wMinute,
						_sysTime.wSecond
					);
				}
				return ::strtoll(szTime, nullptr, 10);
			}
			//! 
			static std::string GetSystimeStr(bool IsMilliseconds = false) {
				SYSTEMTIME _sysTime;
				::GetLocalTime(&_sysTime);
				char szTime[100] = { 0 };
				if (IsMilliseconds) {
					sprintf_s(szTime, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay,
						_sysTime.wHour,
						_sysTime.wMinute,
						_sysTime.wSecond,
						_sysTime.wMilliseconds);
				}
				else {
					sprintf_s(szTime, "%04d/%02d/%02d %02d:%02d:%02d",
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay,
						_sysTime.wHour,
						_sysTime.wMinute,
						_sysTime.wSecond);
				}
				return szTime;
			}
			static std::string GetSysdateStr(__out time_t& date,const char* pFormat = nullptr) {
				SYSTEMTIME _sysTime;
				::GetLocalTime(&_sysTime);
				char szDate[100] = { 0 };
				if (!pFormat) {
					sprintf_s(szDate, _countof(szDate),
						"%04d-%02d-%02d",
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay);
				}
				else {
					sprintf_s(szDate, _countof(szDate),
						pFormat,
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay);
				}

				char szTemp[100] = { 0 };
				sprintf_s(szTemp, _countof(szTemp), "%04d%02d%02d",
					_sysTime.wYear,
					_sysTime.wMonth,
					_sysTime.wDay);
				date = strtoll(szTemp, nullptr, 10);

				return szDate;
			}
			static std::string GetSystimeStrEx(const char* pFormat,bool IsMilliseconds = false) {
				SYSTEMTIME _sysTime;
				::GetLocalTime(&_sysTime);
				char szTime[100] = { 0 };
				if (IsMilliseconds) {
					sprintf_s(szTime, pFormat,
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay,
						_sysTime.wHour,
						_sysTime.wMinute,
						_sysTime.wSecond,
						_sysTime.wMilliseconds);
				}
				else {
					sprintf_s(szTime, pFormat,
						_sysTime.wYear,
						_sysTime.wMonth,
						_sysTime.wDay,
						_sysTime.wHour,
						_sysTime.wMinute,
						_sysTime.wSecond);
				}
				return szTime;
			}

			//! @时间戳转换成长整型时间 如 201903121434568(ms)
			struct Times {
				int Year;
				int Mon;
				int Day;
				int Hour;
				int Min;
				int Second;
				int Milliseconds;
			};
			static long long TimestampToTime(long long _Timestamp, bool IsMilliseconds = false) {
				int ms = 0;
				time_t tick;
				if (IsMilliseconds) {//! 
					ms = _Timestamp % 1000;
					tick = (time_t)((_Timestamp - ms) / 1000);
				}
				else {
					tick = (time_t)_Timestamp;
				}
				struct tm tm;
				char s[100];
				Times standard;

				localtime_s(&tm, &tick);
				strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);

				standard.Year = atoi(s);
				standard.Mon = atoi(s + 5);
				standard.Day = atoi(s + 8);
				standard.Hour = atoi(s + 11);
				standard.Min = atoi(s + 14);
				standard.Second = atoi(s + 17);
				if (ms) {
					sprintf_s(s, 100, "%04d%02d%02d%02d%02d%02d%03d", standard.Year, standard.Mon, standard.Day, standard.Hour, standard.Min, standard.Second, ms);
				}
				else {
					sprintf_s(s, 100, "%04d%02d%02d%02d%02d%02d", standard.Year, standard.Mon, standard.Day, standard.Hour, standard.Min, standard.Second);
				}
				return ::strtoll(s, nullptr, 10);
			}
			static sk::stringa TimestampToTimeStr(const long long& _Timestamp, bool IsMilliseconds = false) {
				int ms = 0;
				time_t tick;
				if (IsMilliseconds) {//! 
					ms = _Timestamp % 1000;
					tick = (time_t)((_Timestamp - ms) / 1000);
				}
				else {
					tick = (time_t)_Timestamp;
				}
				struct tm tm;
				char s[200];
				Times standard;
				localtime_s(&tm, &tick);
				strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
				standard.Year = atoi(s);
				standard.Mon = atoi(s + 5);
				standard.Day = atoi(s + 8);
				standard.Hour = atoi(s + 11);
				standard.Min = atoi(s + 14);
				standard.Second = atoi(s + 17);
				int nret = 0;
				if (ms) {
					nret = sprintf_s(s, _countof(s), "%04d/%02d/%02d %02d:%02d:%02d.%03d", standard.Year, standard.Mon, standard.Day, standard.Hour, standard.Min, standard.Second, ms);
				}
				else {
					nret = sprintf_s(s, _countof(s), "%04d/%02d/%02d %02d:%02d:%02d", standard.Year, standard.Mon, standard.Day, standard.Hour, standard.Min, standard.Second);
				}
				return sk::stringa(s, nret);
			}

			static sk::stringa TimestampToTimeStrNoDate(const long long& _Timestamp, bool IsMilliseconds = false) {
				int ms = 0;
				time_t tick;
				if (IsMilliseconds) {//! 
					ms = _Timestamp % 1000;
					tick = (time_t)((_Timestamp - ms) / 1000);
				}
				else {
					tick = (time_t)_Timestamp;
				}

				struct tm tm = { 0 };
				char s[100] = { 0 };
				Times standard = { 0 };

				localtime_s(&tm, &tick);
				strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);

				standard.Year = atoi(s);
				standard.Mon = atoi(s + 5);
				standard.Day = atoi(s + 8);
				standard.Hour = atoi(s + 11);
				standard.Min = atoi(s + 14);
				standard.Second = atoi(s + 17);
				if (ms) {
					sprintf_s(s, 100, "%02d:%02d:%02d:%03d", standard.Hour, standard.Min, standard.Second, standard.Milliseconds);
				}
				else {
					sprintf_s(s, 100, "%02d:%02d:%02d", standard.Hour, standard.Min, standard.Second);
				}
				return sk::stringa(s);
			}
			static long long GetTimestamp(bool IsMilliseconds = false) {
				if (IsMilliseconds) {
					SYSTEMTIME _sysTime;
					::GetLocalTime(&_sysTime);
					char szTimeMs[100] = { 0 };
					sprintf_s(szTimeMs, "%03d", _sysTime.wMilliseconds);
					return ::strtoll(std::to_string(::time(0)).append(szTimeMs).c_str(), nullptr, 10);
				}
				return ::time(0);
			}
			//!@ 将数据库的Date字符串转换为整型时间 
			//!@ 数据库例如 2013-12-06 00:00:00.000
			//!@ 原则上通用到其它时间格式，原理上是析取数据 打包转换成整型~
			static long long Timestr2Integer(std::string _Timestr) {
				if (_Timestr.empty()) {
					return 0;
				}
				char szTime[18] = { 0 };
				int index = 0;
				for (auto it = _Timestr.begin(); it != _Timestr.end(); ++it) {
					if (*it >= '0' && *it <= '9') {
						szTime[index] = *it;
						++index;
						if (index > ARRAYSIZE(szTime) - 1) {
							break;
						}
					}
				}
				return ::strtoll(szTime, nullptr, 10);
			}


			//! 23:59:59 = 86399 | 00:00:00 = 90060
			static UINT TimePeriodMade(const sk::stringa strTimePeriod, bool end = false)
			{
				UINT result = 0;
				std::vector<sk::stringa> vrParse;
				sk::Helper::OneUnknownParseA(strTimePeriod, ':', vrParse);
				if (vrParse.size() != 3)
				{
					return result;
				}
				result = stoi(vrParse[0]) * 60 * 60 + stoi(vrParse[1]) * 60 + stoi(vrParse[2]);
				if (end && !result)
				{
					return 90060;
				}
				return result;
			}
			static UINT TimePeriodMade(const UINT& Hour, const UINT& Minute, const UINT& Second, bool end = false)
			{
				UINT result = 0;
				result = Hour * 60 * 60 + Minute * 60 + Second;
				if (end && !result)
				{
					result = 90060;
				}
				return result;
			}
			static sk::stringa TimePeriodUnMade(const UINT& TimePeriod)
			{
				sk::stringa result = "00:00:00";
				if (TimePeriod < 90060 && TimePeriod>0)
				{
					sk::int64 sec = TimePeriod % 60;
					sk::int64 min = (TimePeriod - sec) % 3600 / 60;
					sk::int64 hour = (TimePeriod - sec - min * 60) / 3600;
					if (sec >= 0 && sec <= 59 && min >= 0 && min <= 59 && hour >= 0 && hour <= 23)
					{
						result.clear();
						char stime[64] = { 0 };
						auto len = sprintf_s(stime, "%02I64d:%02I64d:%02I64d", hour, min, sec);
						result.append(stime, len);
					}
				}
				return result;
			}

		};



	}///namespace time 
}///namespace sk




/// /*新生联创（上海）**/
/// /*2019年05月18日 7:41:01|954**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_time_H__9B395_8D0501E1_E1746A695F328FB_AFBB__TAIL__