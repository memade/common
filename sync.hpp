#if !defined(AFX_SKSTU_SYNC_H__52035_1D519CB7_46FF3DFAF35DB8F_3E60__HEAD__)
#define AFX_SKSTU_SYNC_H__52035_1D519CB7_46FF3DFAF35DB8F_3E60__HEAD__

namespace sk {

	class Sync final
	{
	public:
		Sync() { ::memset(&m_cs, 0x00, sizeof(m_cs)); ::InitializeCriticalSection(&m_cs); }
		~Sync() { ::DeleteCriticalSection(&m_cs); }
	private:
		CRITICAL_SECTION  m_cs;
	public:
		__inline VOID Lock(void) { ::EnterCriticalSection(&m_cs); }
		__inline VOID UnLock(void) { ::LeaveCriticalSection(&m_cs); }
		__inline BOOL TryLock(void) { return(::TryEnterCriticalSection(&m_cs) != FALSE); }
	};

}///namespace sk




/// /*新生联创（上海）**/
/// /*2020年05月23日 9:43:29|336**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_SYNC_H__52035_1D519CB7_46FF3DFAF35DB8F_3E60__TAIL__
