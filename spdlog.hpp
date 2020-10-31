#if !defined(AFX_SKSTU_spdlog_H__C7AEF_AF5B3BBA_DA348498B276049_5F45__HEAD__)
#define AFX_SKSTU_spdlog_H__C7AEF_AF5B3BBA_DA348498B276049_5F45__HEAD__

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/fmt/ostr.h>

using namespace spdlog;

namespace sk {
#define LOGF sk::Log::Format

	enum struct EnLogType : unsigned long long
	{
		EN_LOG_TYPE_BEGIN = 1,
		EN_LOG_TYPE_TRACE = EN_LOG_TYPE_BEGIN << 1,
		EN_LOG_TYPE_ERROR = EN_LOG_TYPE_BEGIN << 2,
		EN_LOG_TYPE_WARN = EN_LOG_TYPE_BEGIN << 3,
		EN_LOG_TYPE_FAILED = EN_LOG_TYPE_BEGIN << 4,
		EN_LOG_TYPE_SUCCESS = EN_LOG_TYPE_BEGIN << 5,
		EN_LOG_TYPE_SIGNAL = EN_LOG_TYPE_BEGIN << 6,
		EN_LOG_TYPE_FOLLOW = EN_LOG_TYPE_BEGIN << 7,
		EN_LOG_TYPE_SYSTEM = EN_LOG_TYPE_BEGIN << 8,

		EN_LOG_TYPE_ALL = EN_LOG_TYPE_TRACE | EN_LOG_TYPE_ERROR | EN_LOG_TYPE_WARN | \
		EN_LOG_TYPE_FAILED | EN_LOG_TYPE_SUCCESS | EN_LOG_TYPE_SIGNAL | \
		EN_LOG_TYPE_FOLLOW | EN_LOG_TYPE_SYSTEM,

		EN_LOG_TYPE_SIMPLE = EN_LOG_TYPE_TRACE | EN_LOG_TYPE_ERROR | EN_LOG_TYPE_WARN | \
		EN_LOG_TYPE_FAILED | EN_LOG_TYPE_SUCCESS | EN_LOG_TYPE_SYSTEM,
	};


	inline EnLogType operator|(const EnLogType& obj1, const unsigned long long& obj2)
	{
		return EnLogType(unsigned long long(obj1) | obj2);
	}
	inline EnLogType operator|(const EnLogType& obj1, const EnLogType& obj2)
	{
		return EnLogType(unsigned long long(obj1) | unsigned long long(obj2));
	}
	inline EnLogType operator|(const unsigned long long& obj1, const EnLogType& obj2)
	{
		return EnLogType(obj1 | unsigned long long(obj2));
	}
	inline bool operator&(const EnLogType& obj1, const EnLogType& obj2)
	{
		return unsigned long long(obj1) & unsigned long long(obj2);
	}

	class Log final
	{
	public:
		Log(const sk::stringa& path, spdlog::tf_msg_cb msgcb = nullptr) : m_LogPathBase(path + "/sk.logs/"), m_msgcb(msgcb) {}
		~Log() { Free(); }

		std::shared_ptr<spdlog::logger>
			log(const sk::stringa& process, EnLogType logType = EnLogType::EN_LOG_TYPE_SYSTEM)
		{
			return m_pSpdlogPtrQ[process][logType];
		}

	public:
		void MsgCb(const spdlog::tf_msg_cb& msgcb) 
		{
			m_msgcb = msgcb;
			m_pSpdlogPtrQ.iterate(
				[&](const auto& process, auto& loggers)
				{
					loggers.iterate(
						[&](const auto&,auto& logger) 
						{
							logger->msgcb(m_msgcb);
						});
				});
		}
		const sk::stringa Alloc(const sk::stringa& process_name, const EnLogType& LogTypes)
		{
			auto process = sk::Helper::RemoveProcessNameSuffix(process_name);
			sk::stringa module_name = "system";
			sk::container::map<EnLogType, std::shared_ptr<spdlog::logger>> processLoggers;

			{
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				/*spLogger->set_pattern("[%Y%m%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");*/
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_SYSTEM, result);
			}

			if (LogTypes & EnLogType::EN_LOG_TYPE_ERROR)
			{
				module_name = "error";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_ERROR, result);
			}
			if (LogTypes & EnLogType::EN_LOG_TYPE_WARN)
			{
				module_name = "warn";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_WARN, result);
			}
			if (LogTypes & EnLogType::EN_LOG_TYPE_TRACE)
			{
				module_name = "trace";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_TRACE, result);
			}
			if (LogTypes & EnLogType::EN_LOG_TYPE_SUCCESS)
			{
				module_name = "success";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_SUCCESS, result);
			}
			if (LogTypes & EnLogType::EN_LOG_TYPE_FAILED)
			{
				module_name = "failed";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_FAILED, result);
			}
			if (LogTypes & EnLogType::EN_LOG_TYPE_SIGNAL)
			{
				module_name = "signal";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_SIGNAL, result);
			}
			if (LogTypes & EnLogType::EN_LOG_TYPE_FOLLOW)
			{
				module_name = "follow";
				std::vector<spdlog::sink_ptr> sinks;
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>((m_LogPathBase + process + "//" + module_name + ".log").c_str(), 0, 0));
				auto result = std::make_shared<spdlog::logger>(module_name, begin(sinks), end(sinks));
#ifdef _DEBUG //! 设置日志记录级别
				result->set_level(spdlog::level::trace);
#else
				result->set_level(spdlog::level::info);
#endif
				result->set_pattern("[%Y/%m/%d %H:%M:%S.%e][%t][%L][%n] %^%v%$");
				result->flush_on(spdlog::level::trace);
				processLoggers.push(EnLogType::EN_LOG_TYPE_FOLLOW, result);
			}

			if (processLoggers.empty())
			{
				return "";
			}
			m_pSpdlogPtrQ.push(process, processLoggers);
			log(process)->msgcb(m_msgcb);
			log(process)->info("{}", global_sk_team_tenet);
			return process;
		}
		void FreeAt(const sk::stringa& process)
		{
			m_pSpdlogPtrQ.pop(
				process,
				[&](const auto&, auto& modules)
				{
					modules[EnLogType::EN_LOG_TYPE_SYSTEM]->info(global_sk_team_sign);
				});
		}
		void Free()
		{
			m_pSpdlogPtrQ.iterate_clear(
				[&](const auto& process, auto& modules, auto& itbreak, auto& itclear)
				{
					modules[EnLogType::EN_LOG_TYPE_SYSTEM]->info(global_sk_team_sign);
					itclear = true;
				});
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			spdlog::flush_every(std::chrono::seconds(30));
			spdlog::shutdown();
			spdlog::drop_all();
		}
	public:
		const sk::stringa LogPath() const { return m_LogPathBase; }

		template<typename... Args>
		inline static sk::stringw Format(spdlog::wstring_view_t fmt, const Args&... args);
		template<typename... Args>
		inline static sk::stringa Format(spdlog::string_view_t fmt, const Args&... args);
	private:
		sk::stringa m_LogPathBase;
		spdlog::tf_msg_cb m_msgcb = nullptr;
		sk::container::map<sk::stringa, sk::container::map<EnLogType, std::shared_ptr<spdlog::logger>>> m_pSpdlogPtrQ;
	};





	template<typename... Args>
	inline static sk::stringw Log::Format(spdlog::wstring_view_t fmt, const Args&... args)
	{
		try
		{
			fmt::wmemory_buffer wbuf;
			fmt::format_to(wbuf, fmt, args...);
			return wbuf.data();
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
			return L"";
		}
	}
	template<typename... Args>
	inline static sk::stringa Log::Format(spdlog::string_view_t fmt, const Args&... args)
	{
		try
		{
			fmt::memory_buffer buf;
			fmt::format_to(buf, fmt, args...);
			return buf.data();
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
			return "";
		}
	}




}///namespace sk



/// /*新生联创（上海）**/
/// /*2019年04月15日 11:46:23|499**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_spdlog_H__C7AEF_AF5B3BBA_DA348498B276049_5F45__TAIL__



