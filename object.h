#if !defined(AFX_SKSTU_OBJECT_H__8EFA4_F1220BA9_CB4850B6C7B92C1_7C7F__HEAD__)
#define AFX_SKSTU_OBJECT_H__8EFA4_F1220BA9_CB4850B6C7B92C1_7C7F__HEAD__

#include "license.hpp"

namespace sk {

	enum struct EnStatus : LONG
	{
		EN_STATUS_CLOSED = 0,
		EN_STATUS_OPENED = 1,
		EN_STATUS_CLOSING = 2,
		EN_STATUS_OPENING = 3,
	};

	namespace object {

		class ISetup {
		public:
			virtual ~ISetup() {};
		protected:
			virtual int Init() = 0;
			virtual int UnInit() = 0;

			virtual int Set() = 0;
			virtual int Get() = 0;

			virtual int Sync() = 0;
		protected:
			sk::stringa m_path;
		};

		class IObject {
		public:
			virtual ~IObject() { for (int i = 0; i < m_Argc; ++i) { SK_DELETE_PTR_BUFFER(m_Argv[i]);}SK_DELETE_PTR(m_Argv);}
			int Argc() const { return m_Argc; }
			TCHAR** Argv() const { return m_Argv; }
		protected:
			virtual int Init() = 0;
			virtual int UnInit() = 0;
		public:
			virtual int Reset() 
			{
				Close();
				UnInit();
				Init();
				Open();
				return 0;
			};
			virtual int Open() { return 0; };
			virtual int Close() { return 0; }
			virtual int Open(int, TCHAR**) { return 0; }
			virtual int Open(const sk::cchar* IPAddr, const sk::uint32& StreamType, const sk::nptr& Host) { return 0; };
		protected:
			std::vector<std::thread> m_Threads;
			std::atomic_bool m_IsOpen = false;
			std::atomic_bool m_IsOpenT = false;
			TCHAR** m_Argv = nullptr;
			int m_Argc = 0;
			void Remember(int argc, TCHAR** argv)
			{
				if (argc <= 0) return;
				m_Argv = new TCHAR*[argc];
				for (int i = 0; i < argc; ++i)
				{
					if (!argv[i]) { break; }
					m_Argv[i] = new TCHAR[_MAX_PATH];
					_tcscpy_s(m_Argv[i], _MAX_PATH, argv[i]);
					++m_Argc;
				}
			}
		};


		class IGlobal {
			HMODULE m_hModule;
		public:
			IGlobal(HMODULE hModule = nullptr) :
				m_hModule(hModule) {
				Ready();
			}
			virtual ~IGlobal() {

			}
		public:
			HMODULE GetInstance() const { return m_hModule; }
			long LicenseResult() { return InterlockedExchangeAdd(&license, 0); }
			const sk::LicenseInfo& LicenseInfo() const { return m_LicenseInfo; }
		protected:
			virtual int Init() = 0;
			virtual int UnInit() = 0;
			virtual int License(__int64 time = 0) { return 0; };
		protected:
			sk::UUID m_uuid;
			long license = 0;
			sk::LicenseInfo m_LicenseInfo;
			void LicenseSet(const long& at) { InterlockedExchange(&license, at); }
			char m_szCurrentProcessName[_MAX_PATH] = { 0 };
			char m_szCurrentProcessDirectory[_MAX_PATH] = { 0 };

			char m_szCurrentHostProcessName[_MAX_PATH] = { 0 };
			char m_szCurrentHostProcessDirectory[_MAX_PATH] = { 0 };
		public:
			__inline sk::stringa uuid(bool IsUpper = true) { if (!IsUpper) { return m_uuid.uuid(); } else { return sk::StringConvert::ToUpperA(m_uuid.uuid()); } }

			__inline sk::stringa GetCurrentProcessName() { return &m_szCurrentProcessName[0]; }
			__inline sk::stringa GetCurrentProcessDirectory() { return &m_szCurrentProcessDirectory[0]; }

			__inline sk::stringa GetCurrentHostProcessName() { return &m_szCurrentHostProcessName[0]; }
			__inline sk::stringa GetCurrentHostProcessDirectory() { return &m_szCurrentHostProcessDirectory[0]; }
		protected:
			__inline int Ready() {
				std::string strCurrentHostProcessName;
				sk::Helper::GetCurrentProcessNameA(strCurrentHostProcessName, NULL);
				strcpy_s(m_szCurrentHostProcessName, strCurrentHostProcessName.c_str());

				std::string strCurrentHostProcessDirectory = sk::Helper::GetCurrentProcessDirectoryA(NULL).get();
				strcpy_s(m_szCurrentHostProcessDirectory, strCurrentHostProcessDirectory.c_str());

				auto hModule = sk::Helper::GetSelfModuleHandle();
				if (hModule) {
					std::string strCurrentProcessName;
					sk::Helper::GetCurrentProcessNameA(strCurrentProcessName, hModule);
					strcpy_s(m_szCurrentProcessName, strCurrentProcessName.c_str());

					std::string strCurrentProcessDirectory = sk::Helper::GetCurrentProcessDirectoryA(hModule).get();
					strcpy_s(m_szCurrentProcessDirectory, strCurrentProcessDirectory.c_str());
				}
				return 0;
			}
		};



	}///namesoace object
}///namespace sk




/// /*新生联创（上海）**/
/// /*2019年07月30日 14:35:58|602**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_OBJECT_H__8EFA4_F1220BA9_CB4850B6C7B92C1_7C7F__TAIL__