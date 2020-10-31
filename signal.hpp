#if !defined(AFX_SKSTU_signal_H__DA1E4_15AE0E2E_1651FA7CD166436_D5D0__HEAD__)
#define AFX_SKSTU_signal_H__DA1E4_15AE0E2E_1651FA7CD166436_D5D0__HEAD__

#include <Windows.h>

namespace sk {
	///////////////////////////////////////////////////////////////////////////////////
	class CEvent
	{
	public:
		CEvent();
		virtual ~CEvent();
	protected:
		HANDLE m_hEvent;
	public:
		__inline void Signal();
		__inline void Notify();
		__inline bool Wait(unsigned long lMillisec = 0);
		__inline void Pulse();
		__inline void Reset();
	};
	__inline CEvent::CEvent()
	{
		m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	__inline CEvent::~CEvent()
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
	__inline void CEvent::Notify()
	{
		::SetEvent(m_hEvent);
	}
	__inline void CEvent::Signal()
	{
		::SetEvent(m_hEvent);
	}
	__inline bool CEvent::Wait(unsigned long lMillisec /*= 0*/)
	{
		unsigned long __StartTime(0), __StopTime(0), __DiffTime(0);
		__StartTime = ::GetTickCount();
		if (0 == lMillisec)
		{
			::WaitForSingleObject(m_hEvent, INFINITE);
		}
		else
		{
			::WaitForSingleObject(m_hEvent, lMillisec);
		}
		__StopTime = ::GetTickCount();
		__DiffTime = __StopTime - __StartTime;
		if (lMillisec >(__DiffTime + 10))
		{
			return true;
		}
		return false;
	}
	__inline void CEvent::Pulse()
	{
		::Sleep(0);
		::PulseEvent(m_hEvent);
	}
	__inline void CEvent::Reset()
	{
		::ResetEvent(m_hEvent);
	}
	///////////////////////////////////////////////////////////////////////////////////
	template <typename Mutex, typename CondVar>
	class basic_semaphore {
	public:
		using native_handle_type = typename CondVar::native_handle_type;
		explicit basic_semaphore(size_t count = 0);
		basic_semaphore(const basic_semaphore&) = delete;
		basic_semaphore(basic_semaphore&&) = delete;
		basic_semaphore& operator=(const basic_semaphore&) = delete;
		basic_semaphore& operator=(basic_semaphore&&) = delete;
		void notify();
		void wait();
		bool try_wait();
		bool waiting();
		template<class Rep, class Period>
		bool wait_for(const std::chrono::duration<Rep, Period>& d);
		template<class Clock, class Duration>
		bool wait_until(const std::chrono::time_point<Clock, Duration>& t);
		native_handle_type native_handle();
	private:
		Mutex   mMutex;
		CondVar mCv;
		size_t  mCount;
	};
	using semaphore = basic_semaphore<std::mutex, std::condition_variable>;
	template <typename Mutex, typename CondVar>
	basic_semaphore<Mutex, CondVar>::basic_semaphore(size_t count)
		: mCount{ count }
	{}
	template <typename Mutex, typename CondVar>
	void basic_semaphore<Mutex, CondVar>::notify() {
		std::lock_guard<Mutex> lock{ mMutex };
		++mCount;
		mCv.notify_one();
	}
	template <typename Mutex, typename CondVar>
	void basic_semaphore<Mutex, CondVar>::wait() {
		std::unique_lock<Mutex> lock{ mMutex };
		mCv.wait(lock, [&] { return mCount > 0; });
		--mCount;
	}
	template <typename Mutex, typename CondVar>
	bool basic_semaphore<Mutex, CondVar>::try_wait() {
		std::lock_guard<Mutex> lock{ mMutex };
		if (mCount > 0) {
			--mCount;
			return true;
		}
		return false;
	}
	template <typename Mutex, typename CondVar>
	bool basic_semaphore<Mutex, CondVar>::waiting() {
		std::lock_guard<Mutex> lock{ mMutex };
		return mCount > 0;
	}
	template <typename Mutex, typename CondVar>
	template<class Rep, class Period>
	bool basic_semaphore<Mutex, CondVar>::wait_for(const std::chrono::duration<Rep, Period>& d) {
		std::unique_lock<Mutex> lock{ mMutex };
		auto finished = mCv.wait_for(lock, d, [&] { return mCount > 0; });
		if (finished)
			--mCount;
		return finished;
	}
	template <typename Mutex, typename CondVar>
	template<class Clock, class Duration>
	bool basic_semaphore<Mutex, CondVar>::wait_until(const std::chrono::time_point<Clock, Duration>& t) {
		std::unique_lock<Mutex> lock{ mMutex };
		auto finished = mCv.wait_until(lock, t, [&] { return mCount > 0; });
		if (finished)
			--mCount;
		return finished;
	}
	template <typename Mutex, typename CondVar>
	typename basic_semaphore<Mutex, CondVar>::native_handle_type basic_semaphore<Mutex, CondVar>::native_handle() {
		return mCv.native_handle();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	class Semaphore final {
	public:
		Semaphore() {}
		void wait() {
			std::unique_lock<std::mutex> lock{ mutex };
			if (--count <= 0) {
				condition.wait(lock);
			}
		}
		bool waiting() {
			std::unique_lock<std::mutex> lock{ mutex };
			return count <= 0;
		}
		void signal() {
			std::unique_lock<std::mutex> lock{ mutex };
			if (++count <= 0) {
				condition.notify_one();
			}
		}
	private:
		int count = 1;
		std::mutex mutex;
		std::condition_variable condition;
	};
}///namespace xslc




/// /*新生联创（上海）**/
/// /*2019年01月30日 9:32:53|018**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_signal_H__DA1E4_15AE0E2E_1651FA7CD166436_D5D0__TAIL__
