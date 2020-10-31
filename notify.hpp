#if !defined(AFX_SKSTU_notify_H__5D44F_CE9B7FEE_E5F22FA416CFF72_3E88__HEAD__)
#define AFX_SKSTU_notify_H__5D44F_CE9B7FEE_E5F22FA416CFF72_3E88__HEAD__

/**通知回调，消息回调
*/

namespace sk {
	typedef struct tagNotifyPacket {
		int type;//! @_FrequencyType 频率类型 0:低频 1:高频
		long long cmd;
		char* data;
		size_t len;
		tagNotifyPacket() { memset(this, 0x00, sizeof(*this)); }
		tagNotifyPacket(const long long& _cmd, const char* _lpData = nullptr, const size_t& _nData = 0 , int _Type = 0) {
			memset(this, 0x00, sizeof(*this));
			if (_lpData && _nData) {
				type = _Type;
				cmd = _cmd;
				len = _nData;
				data = new char[len];
				memcpy(data, _lpData, len);
			}
			else {
				type = _Type;
				cmd = _cmd;
			}
		}
	}NotifyPacket;

	class NotifyImpl {
	public:
		NotifyImpl() {

		}
		virtual ~NotifyImpl() {
		}
	public:
		bool Verify() {
			return sign_head == 0xFAC9C2D0 && sign_tail == 0xB4B4AAC1;
		}

		virtual void OnNotifyLow(const NotifyPacket*) = 0;
		virtual void OnNotifyHigh(const NotifyPacket*) = 0;
	private:
		const long long sign_tail = 0xB4B4AAC1;
		const long long sign_head = 0xFAC9C2D0;
	};

	class NotifyObject final {
	public:
		NotifyObject() {
			Open();
		}
		~NotifyObject() {
			Close();
		}
	public:
		void Append(NotifyImpl* target, const std::string& mark) {
			m_NotifyTargetQ.push(target, mark);
		}
		void Remove(NotifyImpl* target) {
			m_NotifyTargetQ.pop(target, [](const auto&,auto&) {});
		}
		void Push(const NotifyPacket& pak) {
			switch (pak.type) {
			case 0: {
				m_MsgPakLowQ.push(pak);
			}break;
			case 1: {
				m_MsgPakHighQ.push(pak);
			}break;
			}///switch
		}
		void Push(NotifyPacket* pak) {
			switch (pak->type) {
			case 0: {
				m_MsgPakLowQ.push(*pak);
			}break;
			case 1: {
				m_MsgPakHighQ.push(*pak);
			}break;
			}///switch
		}
	private:
		sk::container::map<NotifyImpl*, std::string> m_NotifyTargetQ;
		sk::container::queue<NotifyPacket> m_MsgPakLowQ;
		sk::container::queue<NotifyPacket> m_MsgPakHighQ;

		std::atomic<bool> m_IsOpen = false;
		std::vector<std::thread> m_vt_thread;
	/*public:*/
		int Open() {
			if (m_IsOpen.load()) { return 0; }
			m_IsOpen.store(true);
			//! 高频轮询
			m_vt_thread.emplace_back([this]() {
				do {
					auto pMsg = m_MsgPakHighQ.pop();
					if (pMsg) {
						m_NotifyTargetQ.iterate([&](NotifyImpl* pNotify, std::string& msg) {
							if (pNotify->Verify()) {
								pNotify->OnNotifyHigh(pMsg.get());
							}				
						});
						SK_DELETE_PTR_BUFFER(pMsg->data);

						std::this_thread::sleep_for(std::chrono::milliseconds(5));
					}
					else {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					if (!m_IsOpen.load()) {
						break;
					}

				} while (1);
			});
			//! 其它轮询
			m_vt_thread.emplace_back([this]() {
				do {
					auto pMsg = m_MsgPakLowQ.pop();
					if (pMsg) {
						m_NotifyTargetQ.iterate([&](NotifyImpl* pNotify, std::string& msg) {
							if (pNotify->Verify()) {
								pNotify->OnNotifyLow(pMsg.get());
							}
						});
						SK_DELETE_PTR_BUFFER(pMsg->data);

						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
					else {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					if (!m_IsOpen.load()) {
						break;
					}

				} while (1);
			});
			return 0;
		}
		void Close() {
			if (!m_IsOpen.load()) { return; }
			m_IsOpen.store(false);
			for (auto it = m_vt_thread.begin(); it != m_vt_thread.end(); ++it) {
				it->join();
			}
			m_vt_thread.clear();

			do {
				auto data = m_MsgPakLowQ.pop();
				if (!data) {
					break;
				}
				SK_DELETE_PTR_BUFFER(data->data);
			} while (1);

			do {
				auto data = m_MsgPakHighQ.pop();
				if (!data) {
					break;
				}
				SK_DELETE_PTR_BUFFER(data->data);
			} while (1);
		}
	};
}


/// /*新生联创（上海）**/
/// /*2019年03月30日 8:46:06|175**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_notify_H__5D44F_CE9B7FEE_E5F22FA416CFF72_3E88__TAIL__