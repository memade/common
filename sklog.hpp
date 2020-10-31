#if !defined(AFX_SKSTU_SKLOG_H__E2A75_3A34F8BD_79AA765B09957FD_328E__HEAD__)
#define AFX_SKSTU_SKLOG_H__E2A75_3A34F8BD_79AA765B09957FD_328E__HEAD__

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/fmt/ostr.h>

namespace sk {

	class Log final {
	public:
		Log(const char* path) : m_LogPath(sk::stringa(path) + "sk.log\\") {
			if (Init()) { Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }
		}
		~Log() {
			UnInit();
		}
		const sk::stringa LogPath() const { return m_LogPath + "\\"; }
	private:
		sk::stringa m_LogPath;
	private:
		int UnInit()
		{
			return 0;
		}
		int Init()
		{
#if 0
			try {
#if 1	//!输出到控制台
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPath + "//" + pModule + ".log").c_str(), 0, 0));
				m_pLoggerPtr = std::make_shared<spdlog::logger>(pModule, begin(sinks), end(sinks));
#else
				result = spdlog::daily_logger_mt(_pModuleName, (m_strLogDirectory + _pModuleName + ".log").c_str(), 0, 0);
#endif

#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				//spLogger->set_pattern("[%Y%m%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);

				m_LoggerQ.push(pModule, result);
			}
			catch (const spdlog::spdlog_ex & ex) {
				std::cout << "Log initialization failed: " << ex.what() << std::endl;
			}
#endif
			return 0;
		}
	private:
		std::shared_ptr<spdlog::logger> m_pLoggerPtr;


#if 0
		int Init() {

			//spdlog::set_level(spdlog::level::info);
			//spdlog::set_pattern("[%Y%m%d %H:%M:%S.%e][%t][%L] %^%v%$");
			//spdlog::set_pattern("[%Y/%m/%d %H:%M:%S.%e][%n] %^%v%$");
			//spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("{}", global_sk_team_tenet); });

			sk::Helper::CreateDirectoryA(m_LogPath.c_str());
			//auto pSystemLog = Append("sk.system");
			//pSystemLog->info("{}", global_sk_team_tenet);
			return 0;
		}
		int UnInit() {
			//spdlog::set_pattern("[%Y/%m/%d %H:%M:%S.%e][%n] %^%v%$");
			//spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("{}", global_sk_team_sign); });
			//log("sk.system", "info", "{}", global_sk_team_sign);

			//m_LoggerQ.iterate(
			//	[&](const auto&,auto& _obj_log) {
			//		_obj_log->
			//	});


			m_LoggerQ.clear();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			spdlog::flush_every(std::chrono::seconds(30));
			spdlog::shutdown();
			spdlog::drop_all();
			return 0;
		}
#if 0
	public:
		void Remove(const char* pModule) {
			m_LoggerQ.pop(pModule, [](const auto&, auto&) {});
		}
		std::shared_ptr<spdlog::logger> Append(const char* pModule, const char* pSubPath = nullptr) {
			std::shared_ptr<spdlog::logger> result;
			try {
#if 1	//!输出到控制台
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				if (!pSubPath) {
					sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPath + "//" + pModule + ".log").c_str(), 0, 0));
				}
				else {
					sk::Helper::CreateDirectoryA((m_LogPath + "//" + pSubPath + "//").c_str());
					sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPath + "//" + pSubPath + "//" + pModule + ".log").c_str(), 0, 0));
				}
				result = std::make_shared<spdlog::logger>(pModule, begin(sinks), end(sinks));
#else
				result = spdlog::daily_logger_mt(_pModuleName, (m_strLogDirectory + _pModuleName + ".log").c_str(), 0, 0);
#endif

#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				//spLogger->set_pattern("[%Y%m%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);

				m_LoggerQ.push(pModule, result);
			}
			catch (const spdlog::spdlog_ex & ex) {
				std::cout << "Log initialization failed: " << ex.what() << std::endl;
			}
			return result;
		}

		template <typename String, typename... Args>
		static __inline auto Format(const String& _Format, const Args& ... _Args) {
			fmt::internal::check_format_string<Args...>(_Format);
			typedef typename fmt::internal::format_string_traits<String>::char_type char_t;
			typedef typename fmt::buffer_context<char_t>::type context_t;
			fmt::format_arg_store<context_t, Args...> as{ _Args... };
			return fmt::internal::vformat(fmt::basic_string_view<char_t>(_Format), fmt::basic_format_args<context_t>(as));
		}

		template <typename String, typename... Args>
		static __inline void log(void* _SpdlogPtr, const char* _pLogType, const String& _Format, const Args& ... _Args) {
			auto pSpdlog = reinterpret_cast<Log*>(_SpdlogPtr);
			if (!pSpdlog) { return; }

			pSpdlog->log("sk.system", _pLogType, _Format, _Args ...);
		}///function end

		template <typename String, typename... Args>
		__inline void log(const char* _pModuleName, const char* _pLogType, const String& _Format, const Args& ... _Args) {
			fmt::internal::check_format_string<Args...>(_Format);
			typedef typename fmt::internal::format_string_traits<String>::char_type char_t;
			typedef typename fmt::buffer_context<char_t>::type context_t;
			fmt::format_arg_store<context_t, Args...> as{ _Args... };
			auto log_data = fmt::internal::vformat(fmt::basic_string_view<char_t>(_Format), fmt::basic_format_args<context_t>(as));
			if (log_data.empty()) {
				return;
			}
			if (!m_LoggerQ.search(_pModuleName,
				[&](const auto& _module_name, auto& _spdlogger) {
					if (0 == memcmp(_pLogType, "error", strlen("error"))) {
						_spdlogger->error(log_data);
					}
					else if (0 == memcmp(_pLogType, "warn", strlen("warn"))) {
						_spdlogger->warn(log_data);
					}
					else if (0 == memcmp(_pLogType, "trace", strlen("trace"))) {
						_spdlogger->trace(log_data);
					}
					else if (0 == memcmp(_pLogType, "critical", strlen("critical"))) {
						_spdlogger->critical(log_data);
					}
					else {
						_spdlogger->info(log_data);
					}
				})) {
				auto pNewModuleLogger = Append(_pModuleName);
				if (pNewModuleLogger) {
					if (0 == memcmp(_pLogType, "error", strlen("error"))) {
						pNewModuleLogger->error(log_data);
					}
					else if (0 == memcmp(_pLogType, "warn", strlen("warn"))) {
						pNewModuleLogger->warn(log_data);
					}
					else if (0 == memcmp(_pLogType, "trace", strlen("trace"))) {
						pNewModuleLogger->trace(log_data);
					}
					else if (0 == memcmp(_pLogType, "critical", strlen("critical"))) {
						pNewModuleLogger->critical(log_data);
					}
					else {
						pNewModuleLogger->info(log_data);
					}
				}
			}
		}///function end
#endif
	private:
		sk::stringa m_LogPath;
		sk::container::queue<sk::stringa> m_LogDataQ;
		sk::container::map<sk::stringa, std::shared_ptr<spdlog::logger>> m_LoggerQ;
#endif
	};
}///namespace sk





/// /*新生联创（上海）**/
/// /*2020年02月14日 10:43:27|100**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_SKLOG_H__E2A75_3A34F8BD_79AA765B09957FD_328E__TAIL__