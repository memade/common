#if !defined(AFX_SKSTU_timer_H__35ECC_D92C9616_E3D86945FE42845_9153__HEAD__)
#define AFX_SKSTU_timer_H__35ECC_D92C9616_E3D86945FE42845_9153__HEAD__

#include "time.hpp"
#include "container.hpp"

namespace sk {
	const time_t ms_millisecond = 1;
	const time_t ms_second = 1000 * ms_millisecond;
	const time_t ms_minute = 60 * ms_second;
	const time_t ms_hour = 60 * ms_minute;
	const time_t ms_hour_half = ms_hour / 2;
	const time_t ms_day = 24 * ms_hour;

	const time_t s_second = 1;
	const time_t s_minute = 60 * s_second;
	const time_t s_hour = 60 * s_minute;
	const time_t s_hour_half = s_hour / 2;
	const time_t s_day = 24 * s_hour;

	class Timer final {
	public:
		Timer() {
		}
		~Timer() {
		}
		int Open() {
			if (m_IsOpen.load()) { return 0; }
			m_Threads.emplace_back(
				[&]() {
					do {
						m_TimerQ.iterate(
							[&](auto& key, auto& _map_business_cb) {
								m_TimestampBakQ.iterate(
									[&](const auto& _time_interval, auto& _timestamp_bak) {						
										if (sk::time::Time::GetTimestamp(true) - _timestamp_bak >= _time_interval) {//! 允许10毫秒偏差
											for (const auto& node : _map_business_cb) {
												node.second(_time_interval, node.first, sk::time::Time::GetSystime());
											}
											_timestamp_bak = sk::time::Time::GetTimestamp(true);
										}
									});
							});
						if (!m_IsOpenT.load()) {
							break;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
					} while (1);		
				});
			m_IsOpen.store(true);
			return 0;
		}
		int Close() {
			if (!m_IsOpen.load()) { return 0; }
			sk::container::queue<sk::int64> _removes;
			m_TimerQ.iterate(
				[&](const auto& key, auto& val) {
					_removes.push(key);
				});
			if (!_removes.empty()) {
				auto node = _removes.pop();
				if (node) {
					m_TimerQ.pop(*node, [](const auto&, auto&) {});
					m_TimestampBakQ.pop(*node, [&](const auto&, auto&) {});
				}
			}
			m_IsOpenT.store(false);
			for (auto it = m_Threads.begin(); it != m_Threads.end(); ++it) {
				it->join();
			}
			m_Threads.clear();
			m_IsOpen.store(false);
			return 0;
		}
	public:
		void TimerAppend(const sk::int64& _time_interval, void* _host_ptr, const std::function<void(const sk::int64&,const sk::intptr&, const sk::int64&)>& _cb_timer) {
			if (!m_TimerQ.search(_time_interval,
				[&](const auto& key, auto& val) {
					val.insert({ { (sk::intptr)_host_ptr,_cb_timer } });
				})) {		
				std::map<sk::intptr, std::function<void(const sk::int64&, const sk::intptr&, const sk::int64&)>> __map({ {(sk::intptr)_host_ptr,_cb_timer } });
				m_TimerQ.push(_time_interval, __map);
				m_TimestampBakQ.push(_time_interval, sk::time::Time::GetTimestamp(true));
			}
		}
		void TimerRemove(const sk::int64& _time_interval, void* _host_ptr) {
			sk::container::queue<sk::int64> _removes;
			m_TimerQ.iterate(
				[&](const auto& key, auto& val) {
					if (key == _time_interval) {
						auto ifind = val.find(reinterpret_cast<sk::intptr>(_host_ptr));
						if (ifind != val.end()) {
							val.erase(ifind);
						}
					}
					if (val.empty()) {
						_removes.push(key);
					}
				});
			if (!_removes.empty()) {
				auto node = _removes.pop();
				if (node) {
					m_TimerQ.pop(*node, [](const auto&,auto&) {});
					m_TimestampBakQ.pop(*node, [&](const auto&,auto&) {});
				}
			}
		}
	private:
		std::vector<std::thread> m_Threads;
		std::atomic<bool> m_IsOpen = false;
		std::atomic<bool> m_IsOpenT = true;
		sk::container::map<sk::int64, sk::int64> m_TimestampBakQ;
		sk::container::map<sk::int64, std::map<sk::intptr, std::function<void(const sk::int64&, const sk::intptr&, const sk::int64&)>>> m_TimerQ;
	};
}///namespace sk




/// /*新生联创（上海）**/
/// /*2019年06月30日 13:35:39|493**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_timer_H__35ECC_D92C9616_E3D86945FE42845_9153__TAIL__
