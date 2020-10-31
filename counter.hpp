#if !defined(AFX_SKSTU_Counter_H__F3B33_365F6757_C6D7E0FF2136421_3E1C__HEAD__)
#define AFX_SKSTU_Counter_H__F3B33_365F6757_C6D7E0FF2136421_3E1C__HEAD__


namespace sk {

	class Counter final {
	public:
		Counter() { m_mutex = std::make_shared<std::mutex>(); }
		~Counter() {}
	public:
		long Increment()
		{
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Counter++;
			return m_Counter.load();
		}
		long Decrement()
		{
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Counter--;
			return m_Counter.load();
		}
		long operator=(const long& value)
		{
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Counter.store(value);
			return m_Counter.load();
		}
		void Reset()
		{
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Counter.store(0);
		}
		void Assign(const long& value)
		{
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Counter.store(value);
		}
		long Get()
		{
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Counter.load();
		}
	private:
		std::shared_ptr<std::mutex> m_mutex;
		std::atomic_long m_Counter = 0;
	};

	typedef class Switch final {
	public:
		Switch() { m_mutex = std::make_shared<std::mutex>(); }
		Switch(const bool& status) { m_mutex = std::make_shared<std::mutex>(); m_Status = status; }
		~Switch() {}
	public:
		bool Get() {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Status.load();
		}
		void Set(const bool& status) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Status.store(status);
		}
		void operator=(Switch& obj) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Status.store(obj.Get());
		}
		void operator=(const bool& status) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Status.store(status);
		}
	private:
		std::shared_ptr<std::mutex> m_mutex;
		std::atomic_bool m_Status = true;
	}Ready,Open;
#if 0
	class Status final {
	public:
		Status() { m_mutex = std::make_shared<std::mutex>(); }
		Status(const sk::uint32& status) { m_mutex = std::make_shared<std::mutex>(); m_Status = status; }
		~Status() {}
	public:
		sk::uint32 GetStatus() {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Status.load();
		}
		void SetStatus(const sk::uint32& status) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Status.store(status);
		}
		void operator=(Status& obj) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Status.store(obj.GetStatus());
		}
		void operator=(const sk::uint32& status) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			m_Status.store(status);
		}
		bool operator==(Status& obj) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Status.load() == obj.GetStatus();
		}
		bool operator==(const sk::uint32& status) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Status.load() == status;
		}
		bool operator!=(const sk::uint32& status) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Status.load() != status;
		}
		bool operator!=(Status& obj) {
			std::lock_guard<std::mutex> lock{ *m_mutex };
			return m_Status.load() != obj.GetStatus();
		}
	private:
		std::shared_ptr<std::mutex> m_mutex;
		std::atomic_uint32_t m_Status = true;
	};
#endif
	class Flag final {//! 三种状态 0: 关  1: 开  -1: 开关中...
	public:
		Flag(const long& val) { m_flag = val; }
		~Flag(){}
	public:
		long Get() noexcept { return ::InterlockedExchangeAdd(&m_flag, 0); }
		long Set(const long& val) noexcept { return ::InterlockedExchange(&m_flag, val); }
		void operator=(const long& val) noexcept  { ::InterlockedExchange(&m_flag,val); }
		bool operator!=(const long& val) noexcept { return ::InterlockedExchangeAdd(&m_flag, 0l) != val; }
		bool operator==(const long& val) noexcept { return ::InterlockedExchangeAdd(&m_flag, 0l) == val; }
	private:
		volatile long m_flag = 0;
	};

	class ID final {
	public:
		ID(const sk::uint64& val) { m_id = val; }
		~ID(){}
	public:
		void operator=(const sk::uint64& val) noexcept { ::InterlockedExchange(&m_id, val); }
		bool operator!=(const sk::uint64& val) noexcept { return ::InterlockedExchangeAdd(&m_id, 0l) != val; }
		bool operator==(const sk::uint64& val) noexcept { return ::InterlockedExchangeAdd(&m_id, 0l) == val; }
		sk::uint64 operator+=(const sk::uint64& val) noexcept { ::InterlockedExchangeAdd(&m_id, val); }
		sk::uint64 operator++(int) noexcept {return ::InterlockedExchangeAdd(&m_id, 1);}
		sk::uint64 Get() noexcept { return ::InterlockedExchangeAdd(&m_id, 0); }
	private:
		sk::uint64 m_id = 0;
	};

	class Timestamp final {
	public:
		Timestamp() {}
		Timestamp(const unsigned long long& timestamp) {}
		~Timestamp() {}
	public:
		unsigned long long Get() {
			return ::InterlockedExchangeAdd(&m_Timestamp, 0);
		}
		void Set(const unsigned long long& timestamp) {
			::InterlockedExchange(&m_Timestamp, timestamp);
		}
		void Set(Timestamp& obj) {
			Set(obj.Get());
		}
		bool operator==(const unsigned long long& timestamp) {
			return Get() == timestamp;
		}
		bool operator!=(const unsigned long long& timestamp) {
			return Get() != timestamp;
		}
		bool operator==(Timestamp& obj) {
			return Get() == obj.Get();
		}
		bool operator!=(Timestamp& obj) {
			return Get() != obj.Get();
		}
	private:
		unsigned long long m_Timestamp = 0;
	};

	class Status final {
	public:
		enum class EnStatus : int {
			EN_STATUS_INVALID = 0,
			EN_STATUS_OPENING = 1,
			EN_STATUS_CLOSEING = 2,
			EN_STATUS_OPENED = 3,
			EN_STATUS_CLOSED = 4,
			EN_STATUS_CLOSE_FIRE = 5,/*点燃关闭*/
		};
	public:
		Status(const EnStatus& status) {
			Set(status);
		}
		~Status() {}
	public:
		void Set(const EnStatus& status) {
			::InterlockedExchange(&m_Src, static_cast<int>(status));
		}
		EnStatus Get() {
			return static_cast<EnStatus>(::InterlockedExchangeAdd(&m_Src, 0));
		}
		bool operator==(Status& obj) {
			return obj.Get() == Get();
		}
		bool operator!=(Status& obj) {
			return obj.Get() != Get();
		}
		void operator=(Status& obj) {
			Set(obj.Get());
		}

		bool operator==(const EnStatus& obj) {
			return obj == Get();
		}
		bool operator!=(const EnStatus& obj) {
			return obj != Get();
		}
		void operator=(const EnStatus& obj) {
			Set(obj);
		}
	private:
		volatile long m_Src = 0;
	};

}///namespace sk



/// /*新生联创（上海）**/
/// /*2019年08月31日 2:51:19|640**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_Counter_H__F3B33_365F6757_C6D7E0FF2136421_3E1C__TAIL__