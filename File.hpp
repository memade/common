#if !defined(AFX_SKSTU_FILE_H__FEF97_AB485DCF_416C07B8D403283_4E59__HEAD__)
#define AFX_SKSTU_FILE_H__FEF97_AB485DCF_416C07B8D403283_4E59__HEAD__

#include "time.hpp"

namespace sk {
	namespace file {
#pragma pack(push,1)
		//! Memory file standard header
		typedef struct tagDiskFileHeader {
			long long			IdentifyHead;			//	0xFAC9C2D0
			char					comment[64];			//	�ļ�ע��
			long long			length;						//	���ݳ��ȣ�������ͷ���ȣ�
			long long			length_total;				//	�ܳ��ȣ�����ͷ���ȣ�
			long long			create_timestamp;	// ����ʱ���
			long long			update_timestamp;	// ����ʱ���
			long long			reserve1[7];
			char					reserve2[32];
			long long			IdentifyTail;				//	0xB4B4AAC1
			tagDiskFileHeader() {
				memset(this, 0x00, sizeof(*this));
				IdentifyHead = 0xFAC9C2D0;
				IdentifyTail = 0xB4B4AAC1;
			}
			__inline bool Verify() const {
				return IdentifyHead == 0xFAC9C2D0 && IdentifyTail == 0xB4B4AAC1;
			}
		}DiskFileHeader;
		const int DISKFILEHEADERLEN = sizeof(DiskFileHeader);
#pragma pack(pop)
		class DiskFile final {
		public:
			DiskFile(const char* pFilePath,bool IsHead = true) :
				m_IsHead(IsHead),
				m_pFile(nullptr),
				m_mutex(),
				m_condition_variable() {
				memset(&m_Head, 0x00, sizeof(m_Head));
				memcpy(m_szFilePath, pFilePath, strlen(pFilePath));
			}
			~DiskFile() {
				Close();
			}
		public:
			__inline bool Open(const char* _Mode = "ab+", int _ShFlag = _SH_DENYNO) {//"rb+"
				if (!strcmp(_Mode, "r+") ||//!��ֻ���ļ������ļ�������ڡ�
					!strcmp(_Mode, "r+") ||//!�򿪿ɶ�д���ļ������ļ�������ڡ�
					!strcmp(_Mode, "rb+") ||//!��д��һ���������ļ���ֻ�����д���ݡ�
					!strcmp(_Mode, "rt+") ||//!��д��һ���ı��ļ����������д��
					!strcmp(_Mode, "w") ||//!��ֻд�ļ������ļ��������ļ�������Ϊ0�������ļ����ݻ���ʧ�����ļ��������������ļ���
					!strcmp(_Mode, "w+") ||//!�򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������������ļ�
					!strcmp(_Mode, "a") ||//!�Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�����ļ�ԭ�ȵ����ݻᱻ��������EOF��������
					!strcmp(_Mode, "a+") ||//!�Ը��ӷ�ʽ�򿪿ɶ�д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������ ��ԭ����EOF����������
					!strcmp(_Mode, "wb") ||//!ֻд�򿪻��½�һ���������ļ���ֻ����д���ݡ�
					!strcmp(_Mode, "wb+") ||//!��д�򿪻���һ���������ļ����������д��
					!strcmp(_Mode, "wt+") ||//!��д�򿪻��Ž���һ���ı��ļ��������д��
					!strcmp(_Mode, "at+") ||//!��д��һ���ı��ļ�������������ı�ĩ׷�����ݡ�
					!strcmp(_Mode, "ab+")) {//!��д��һ���������ļ�������������ļ�ĩ׷�����ݡ�
					m_ShFlag = _ShFlag;
					//! ����ļ��Ƿ����
					if (::_access(m_szFilePath, 0) != -1) {//! ����
						m_pFile = ::_fsopen(m_szFilePath, "rb+", m_ShFlag);
						::fseek(m_pFile, 0, SEEK_END);

						if (m_IsHead) {
							auto filelen = ::ftell(m_pFile);
							if (filelen < DISKFILEHEADERLEN) {//! ����ļ�����С�ڱ�׼ͷ���ȣ���˵�����ļ����Ϸ� ���ǻὫ���ļ���ղ��������ɾ���ͷ����Ϣ���ļ�
								fclose(m_pFile);
								m_pFile = ::_fsopen(m_szFilePath, "w+", m_ShFlag);//!����ļ���


								//!д���ʼͷ����Ϣ
								tagDiskFileHeader _NewHead;
								_NewHead.create_timestamp = sk::time::Time::GetTimestamp();
								_NewHead.update_timestamp = _NewHead.create_timestamp;
								_NewHead.length_total = DISKFILEHEADERLEN;
								memcpy(&m_Head, &_NewHead, DISKFILEHEADERLEN);
								::fseek(m_pFile, 0, SEEK_END);
								if (::fwrite((const void*)& m_Head, DISKFILEHEADERLEN, 1, m_pFile) != 1) {
									return false;
								}
								::fseek(m_pFile, 0, SEEK_END);
							}
							else {
								::fseek(m_pFile, 0, SEEK_SET);
								std::shared_ptr<char> buffer(new char[DISKFILEHEADERLEN], [](char* pbuffer) { delete[] pbuffer; });
								if (1 != ::fread(buffer.get(), DISKFILEHEADERLEN, 1, m_pFile)) {
									return false;
								}
								memcpy(&m_Head, buffer.get(), DISKFILEHEADERLEN);
								::fseek(m_pFile, 0, SEEK_END);
							}
						}///if (m_IsHead)				
					}
					else {
						m_pFile = ::_fsopen(m_szFilePath, "ab+", m_ShFlag);
						if (m_IsHead) {
							//!д���ʼͷ����Ϣ
							tagDiskFileHeader _NewHead;
							_NewHead.create_timestamp = sk::time::Time::GetTimestamp();
							_NewHead.update_timestamp = _NewHead.create_timestamp;
							_NewHead.length_total = DISKFILEHEADERLEN;
							memcpy(&m_Head, &_NewHead, DISKFILEHEADERLEN);
							::fseek(m_pFile, 0, SEEK_END);
							if (::fwrite((const void*)& m_Head, DISKFILEHEADERLEN, 1, m_pFile) != 1) {
								return false;
							}
							::fseek(m_pFile, 0, SEEK_END);
						}
					}
					if (m_pFile) {
						return true;
					}
				}///if (!strcmp . . .
				return false;
			}///Open
			__inline void Close() {
				if (m_pFile) {
					::fclose(m_pFile);
					m_pFile = nullptr;
				}
			}///Close
			__inline long GetSize() {//! �������ݴ�С������ͷ��С
				if (!m_pFile) {
					return 0;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				::fseek(m_pFile, 0, SEEK_END);
				if (m_IsHead) {
					return ::ftell(m_pFile) - DISKFILEHEADERLEN;
				}
				return ::ftell(m_pFile);
			}///GetSize
			__inline long GetDataSize() {
				if (!m_pFile) {
					return 0;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				::fseek(m_pFile, 0, SEEK_END);
				return ::ftell(m_pFile);
			}
			//! @_Offset!=0 && _ElementCount==0 : Read all the content after the selected location *!*@_Offset: An offset from the start of the file
			//! @_Offset==0 && _ElementCount==0 : Read the full content
			__inline std::shared_ptr<char> Read(long _Offset = 0, size_t _ElementCount = 0, size_t _ElementSize = sizeof(char), int _Origin = SEEK_SET) {
				if (!m_pFile) {
					return nullptr;
				}
				std::lock_guard<std::mutex> lock(m_mutex);

				::fseek(m_pFile, 0, SEEK_END);
				size_t lenFile = ::ftell(m_pFile);
				if (lenFile <= 0) {
					return nullptr;
				}

				//! Read the full content
				if (_Offset == 0 && _ElementCount == 0) {
					::fseek(m_pFile, _Offset + DISKFILEHEADERLEN, SEEK_SET);
					std::shared_ptr<char> buffer(new char[lenFile - DISKFILEHEADERLEN], [](char* pbuffer) { delete[] pbuffer; });
					auto result = ::fread(buffer.get(), _ElementSize, lenFile - DISKFILEHEADERLEN, m_pFile);
					if (result < lenFile - DISKFILEHEADERLEN) {
						return nullptr;
					}
					return buffer;
				}
				else if (_Offset != 0 && _ElementCount == 0) {
					if (lenFile <= (size_t)_Offset) {
						return nullptr;
					}
					::fseek(m_pFile, _Offset, SEEK_SET);
					std::shared_ptr<char> buffer(new char[lenFile - _Offset], [](char* pbuffer) { delete[] pbuffer; });
					auto result = ::fread(buffer.get(), _ElementSize, lenFile - _Offset, m_pFile);
					if (result < (lenFile - _Offset)) {
						return nullptr;
					}
					return buffer;
				}
				//! Read the selection
				if (lenFile < _ElementSize || lenFile < _ElementCount) {
					return nullptr;
				}
				::fseek(m_pFile, _Offset, _Origin);
				lenFile = ftell(m_pFile);
				if (lenFile < _ElementSize || lenFile < _ElementCount) {
					return nullptr;
				}
				std::shared_ptr<char> buffer(new char[_ElementCount], [](char* pbuffer) { delete[] pbuffer; });
				auto result = ::fread(buffer.get(), _ElementSize, _ElementCount, m_pFile);
				if (result < _ElementCount) {
					return nullptr;
				}
				return buffer;
			}
			//@ default: Start at the end of the file
			long Write(const char* pData, long DataLen, long _Offset = 0, int _Origin = SEEK_END) {
				if (!m_pFile) {
					return 0ull;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				if (_Origin == SEEK_END) {
					_Offset = 0;
				}
				::fseek(m_pFile, _Offset, _Origin);
				if (::fwrite((const void*)pData, DataLen, 1, m_pFile) != 1) {
					return 0ull;
				}
				::fseek(m_pFile, 0, SEEK_END);
				if (m_IsHead) {
					//! �����ļ�ͷ
					m_Head.update_timestamp = ::time(0);
					m_Head.length_total = ftell(m_pFile);
					m_Head.length = m_Head.length_total - DISKFILEHEADERLEN;
					::fseek(m_pFile, 0, SEEK_SET);
					::fwrite((const void*)& m_Head, DISKFILEHEADERLEN, 1, m_pFile);
				}
				::fseek(m_pFile, 0, SEEK_END);
				return ::ftell(m_pFile);
			}///Write
			void Clear() {
				if (!m_pFile) {
					return;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				fclose(m_pFile);
				m_pFile = ::_fsopen(m_szFilePath, "w+", m_ShFlag);//!����ļ���
				if (m_IsHead) {
					//!д���ʼͷ����Ϣ
					memcpy(&m_Head, &tagDiskFileHeader(), DISKFILEHEADERLEN);
					::fseek(m_pFile, 0, SEEK_END);
					if (::fwrite((const void*)& m_Head, DISKFILEHEADERLEN, 1, m_pFile) != 1) {
						return;
					}
				}
				::fseek(m_pFile, 0, SEEK_END);
			}
		private:
			bool m_IsHead;
			int m_ShFlag;
			tagDiskFileHeader m_Head;
			char m_szFilePath[260];
			FILE* m_pFile;
			std::mutex m_mutex;
			std::condition_variable m_condition_variable;
		};
#pragma pack(push,1)
		//! Memory file standard header
		typedef struct tagMapFileHeader {
			std::uint64_t		IdentifyHead;			//	0xFAC9C2D0
			std::int8_t			sharedname[64];		//	Shared memory name
			//std::int8_t			mutexname[64];		// Metux name
			std::int64_t		length;						//	The length of the data(Inclusion head structure)
			std::int64_t		length_total;				//	File total size
			std::int64_t		create_timestamp;
			std::int64_t		update_timestamp;
			std::int64_t		reserve1[7];
			std::int8_t			reserve2[32];
			std::uint64_t		IdentifyTail;				//	0xB4B4AAC1
			tagMapFileHeader() {
				memset(this, 0x00, sizeof(*this));
				IdentifyHead = 0xFAC9C2D0;
				IdentifyTail = 0xB4B4AAC1;
				create_timestamp = ::time(0);
				update_timestamp = create_timestamp;
			}
			__inline bool Verify() const {
				return IdentifyHead == 0xFAC9C2D0 && IdentifyTail == 0xB4B4AAC1;
			}
		}MapFileHeader;
		const size_t MAPFILEHEADERLEN = sizeof(MapFileHeader);
#pragma pack(pop)
		class MapFile final {
		public:
			MapFile(const char* _FilePath, const char* _SharedName, bool _IsClearOpen = false) :
				m_IsOpen(false),
				m_hObjectFile(nullptr),
				m_IsClearOpen(_IsClearOpen),
				m_strFilePath(_FilePath) {
				auto lenSharedName = strlen(_SharedName);
				if (lenSharedName > ARRAYSIZE(m_FileInfo.sharedname)) {
					lenSharedName = ARRAYSIZE(m_FileInfo.sharedname);
				}
				memcpy(m_FileInfo.sharedname, _SharedName, lenSharedName);
			}
			~MapFile() {
			}
		public:
			void Clear() {
				if (!m_IsOpen) {
					return;
				}
				Close();
				m_IsClearOpen = true;
				Open();
			}
			//! ��λ/�޸�
			void Reset(size_t DataLength = 0) {
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!DataLength) {
					m_qwFileOffset = MAPFILEHEADERLEN;
					//! �����ļ�ͷ
					auto thisHeader = (MapFileHeader*)m_lpMappingBaseAddress;
					thisHeader->update_timestamp = ::time(0);
					thisHeader->length = 0;
					thisHeader->length_total = m_dwMappingFileSize;
					memcpy(&m_FileInfo, thisHeader, MAPFILEHEADERLEN);
					::memcpy((BYTE*)m_lpMappingBaseAddress, &m_FileInfo, sizeof(m_FileInfo));
					::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset);
				}
				else {
					m_qwFileOffset = MAPFILEHEADERLEN + DataLength;
					//! �����ļ�ͷ
					auto thisHeader = (MapFileHeader*)m_lpMappingBaseAddress;
					thisHeader->update_timestamp = ::time(0);
					thisHeader->length = DataLength;
					thisHeader->length_total = m_dwMappingFileSize;
					memcpy(&m_FileInfo, thisHeader, MAPFILEHEADERLEN);
					::memcpy((BYTE*)m_lpMappingBaseAddress, &m_FileInfo, sizeof(m_FileInfo));
					::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset);
				}
			}
			bool Open() {
				if (m_IsOpen) {
					return false;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				DWORD dwOpenType = OPEN_ALWAYS;
				if (m_IsClearOpen) {
					if (_access(m_strFilePath.c_str(), 0) != -1) {//! ������ļ�����
						dwOpenType = OPEN_ALWAYS | TRUNCATE_EXISTING;
					}
				}
				m_hObjectFile = ::CreateFileA(
					m_strFilePath.c_str(),
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					dwOpenType,
					FILE_FLAG_SEQUENTIAL_SCAN,
					NULL);
				if (INVALID_HANDLE_VALUE == m_hObjectFile || NULL == m_hObjectFile) {
					return false;
				}

				bool bProcessStageResult = false;
				//! �ļ���С
				auto CurrentFileLen = ::GetFileSize(m_hObjectFile, NULL);

				if (m_IsClearOpen) {
					m_FileInfo.length = 0;
					m_FileInfo.length_total = MAPFILEHEADERLEN;
					m_qwFileOffset = MAPFILEHEADERLEN;
					bProcessStageResult = true;
				}
				else {
					//! ����ļ���С�����׼�ļ�ͷ������ո��ļ���������
					if (CurrentFileLen < MAPFILEHEADERLEN) {
						m_FileInfo.length = 0;
						m_FileInfo.length_total = MAPFILEHEADERLEN;
						m_qwFileOffset = MAPFILEHEADERLEN;
						bProcessStageResult = true;
					}
					else {
						if (::SetFilePointer(m_hObjectFile, 0, NULL, FILE_BEGIN) != HFILE_ERROR) {
							DWORD dwRead = 0;
							MapFileHeader Head;
							if (::ReadFile(m_hObjectFile, &Head, MAPFILEHEADERLEN, &dwRead, NULL)) {
								if (Head.Verify()) {
									::memcpy(&m_FileInfo, &Head, MAPFILEHEADERLEN);
									::SetFilePointer(m_hObjectFile, 0, NULL, FILE_END);
									::SetEndOfFile(m_hObjectFile);
									m_qwFileOffset = MAPFILEHEADERLEN + Head.length;
									bProcessStageResult = true;
								}
								else {
									auto headerror = 0;
								}
							}///if (ReadFile(m_hObjectFile, &Head, MAPFILEHEADERLEN, &dwRead, NULL))
						}///if (::SetFilePointer(m_hObjectFile, 0, NULL, FILE_BEGIN) != HFILE_ERROR)
					}
				}

				if (!bProcessStageResult) {
					if (m_hObjectFile) {
						::CloseHandle(m_hObjectFile);
						m_hObjectFile = nullptr;
					}
					return false;
				}///if (!bProcessStageResult) 
#if 0
				 //! ȡ��ϵͳĬ�ϵĵ�ҳ���С
				SYSTEM_INFO sysInfo;
				GetSystemInfo(&sysInfo);
				if (CurrentFileLen <= sysInfo.dwAllocationGranularity) {
					m_dwMappingFileSize = sysInfo.dwAllocationGranularity;
				}
				else {
					m_dwMappingFileSize = ((CurrentFileLen / sysInfo.dwAllocationGranularity) + 1) * sysInfo.dwAllocationGranularity;
				}
#else
				m_dwMappingFileSize = CurrentFileLen;

				SYSTEM_INFO sysInfo;
				GetSystemInfo(&sysInfo);

				if (m_dwMappingFileSize < sysInfo.dwAllocationGranularity) {
					m_dwMappingFileSize = sysInfo.dwAllocationGranularity;
				}
#endif
				//! Ԥӳ���ļ����ڴ�
				m_hObjectFileMapping = ::CreateFileMappingA(
					m_hObjectFile,
					NULL,
					PAGE_READWRITE,
					0,
					m_dwMappingFileSize,
					(LPCSTR)m_FileInfo.sharedname);

				if (INVALID_HANDLE_VALUE == m_hObjectFileMapping || NULL == m_hObjectFileMapping) {

					auto errorno = GetLastError();
					if (m_hObjectFile) {
						::CloseHandle(m_hObjectFile);
						m_hObjectFile = nullptr;
					}
					return false;
				}///if (INVALID_HANDLE_VALUE == m_hObjectFileMapping || NULL == m_hObjectFileMapping)

				//! ӳ���ļ����ڴ�
				m_lpMappingBaseAddress = ::MapViewOfFile(
					m_hObjectFileMapping,
					FILE_MAP_ALL_ACCESS,
					(DWORD)(m_qwFileOffset >> 32),
					0/*(DWORD)(m_qwFileOffset & 0xFFFFFFFF)*/,
					m_dwMappingFileSize);
				if (NULL == m_lpMappingBaseAddress) {
					if (m_hObjectFileMapping) {
						::CloseHandle(m_hObjectFileMapping);
						m_hObjectFileMapping = nullptr;
					}
					if (m_hObjectFile) {
						::CloseHandle(m_hObjectFile);
						m_hObjectFile = nullptr;
					}
					return false;
				}
				::memcpy((BYTE*)m_lpMappingBaseAddress, &m_FileInfo, sizeof(m_FileInfo));
				if (!::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset)) {
					auto sk = 0;
				}
				m_IsOpen = true;
				return true;
			}///Open()
			void Close() {
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!m_IsOpen) {
					return;
				}
				if (!::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset)) {

				}

				if (!::UnmapViewOfFile(m_lpMappingBaseAddress)) {

				}
				m_IsOpen = false;
				if (m_hObjectFileMapping) {
					::CloseHandle(m_hObjectFileMapping);
					m_hObjectFileMapping = nullptr;
				}
				if (m_hObjectFile) {
					::CloseHandle(m_hObjectFile);
					m_hObjectFile = nullptr;
				}
				auto hTempObjectFile = ::CreateFileA(
					m_strFilePath.c_str(),
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_ALWAYS,
					FILE_FLAG_SEQUENTIAL_SCAN,
					NULL);
				if (INVALID_HANDLE_VALUE == hTempObjectFile || NULL == hTempObjectFile) {
					return;
				}
				auto hTempObjectFileMapping = ::CreateFileMappingA(
					hTempObjectFile,
					NULL,
					PAGE_READWRITE,
					0,
					(DWORD)(m_qwFileOffset & 0xFFFFFFFF),
					NULL);
				if (INVALID_HANDLE_VALUE == hTempObjectFileMapping || NULL == hTempObjectFileMapping) {
					if (hTempObjectFile) {
						::CloseHandle(hTempObjectFile);
						hTempObjectFile = NULL;
					}
					return;
				}
				if (hTempObjectFile) {
					::CloseHandle(hTempObjectFile);
					hTempObjectFile = nullptr;
				}
				auto pTempMappingBaseAddress = ::MapViewOfFile(
					hTempObjectFileMapping,
					FILE_MAP_ALL_ACCESS,
					0,
					0,
					(UINT)m_qwFileOffset);

				if (NULL == pTempMappingBaseAddress) {
					if (hTempObjectFileMapping) {
						::CloseHandle(hTempObjectFileMapping);
						hTempObjectFileMapping = nullptr;
					}
					return;
				}
				if (!::UnmapViewOfFile(pTempMappingBaseAddress)) {

				}
				if (hTempObjectFileMapping) {
					::CloseHandle(hTempObjectFileMapping);
					hTempObjectFileMapping = nullptr;
				}
			}///Close()
			//! ��ͷ����ȡ���������ļ���Ϣͷ
			const char* Read() {
				if (!m_IsOpen) {
					return nullptr;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!m_hObjectFileMapping) {
					return nullptr;
				}
				return (const char*)m_lpMappingBaseAddress + MAPFILEHEADERLEN;
			}
			long long GetDataLength() {
				if (!m_IsOpen) {
					return 0;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				auto thisHeader = (tagMapFileHeader*)m_lpMappingBaseAddress;
				return thisHeader->length;
			}
			//! ��ָ����ƫ������ȡ������ͷ����ƫ����|�����ݿ�ʼ�����ƫ����
			const char* Read(unsigned long _Offset) {
				if (!m_IsOpen) {
					return nullptr;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!m_hObjectFileMapping) {
					return nullptr;
				}
				if (m_qwFileOffset < _Offset) {
					return nullptr;
				}
				return (const char*)m_lpMappingBaseAddress + MAPFILEHEADERLEN + _Offset;
			}
			//! *׷��д��
			//! �����ҳ �򷵻�һ���ڴ�ӳ���µĻ�ַ��
			//! ���δ��ҳ���򷵻�nullptr
			//! ������-1
			bool Write(const char* _pData, unsigned long _nData) {
				if (!m_IsOpen) {
					return false;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				//! ���д���µ��������ݽ�������ǰӳ���ļ��Ĵ�С��60%
				//! ��ô��ӳ���ļ�����
				bool IsNewPage = false;	//! ��ҳ���
				if ((m_dwMappingFileSize * 5 / 10) <= (m_qwFileOffset + _nData))
				{
					::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset);
					if (m_hObjectFileMapping) {
						::CloseHandle(m_hObjectFileMapping);
						m_hObjectFileMapping = nullptr;
					}
					::UnmapViewOfFile(m_lpMappingBaseAddress);
					m_lpMappingBaseAddress = nullptr;
					if (m_hObjectFile) {
						::CloseHandle(m_hObjectFile);
						m_hObjectFile = nullptr;
					}
					m_hObjectFile = ::CreateFileA(
						m_strFilePath.c_str(),
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_ALWAYS,
						FILE_FLAG_SEQUENTIAL_SCAN,
						NULL);
					if (INVALID_HANDLE_VALUE == m_hObjectFile || NULL == m_hObjectFile) {
						return false;
					}
					::SetFilePointer(m_hObjectFile, 0, NULL, FILE_END);
					::SetEndOfFile(m_hObjectFile);

					//! ȡ��ϵͳĬ�ϵĵ�ҳ���С
					SYSTEM_INFO sysInfo;
					GetSystemInfo(&sysInfo);

					auto diff = (m_dwMappingFileSize * 5 / 10) - (m_qwFileOffset + _nData);
					if (diff < 0) {
						diff = 0 - diff;
					}
					m_dwMappingFileSize += sysInfo.dwAllocationGranularity;
					m_dwMappingFileSize += static_cast<DWORD>(diff);

					m_hObjectFileMapping = ::CreateFileMappingA(
						m_hObjectFile,
						NULL,
						PAGE_READWRITE,
						0,
						m_dwMappingFileSize,
						(LPCSTR)m_FileInfo.sharedname);

					if (INVALID_HANDLE_VALUE == m_hObjectFileMapping || NULL == m_hObjectFileMapping) {
						if (m_hObjectFile) {
							::CloseHandle(m_hObjectFile);
							m_hObjectFile = NULL;
							return false;
						}
					}

					m_lpMappingBaseAddress = ::MapViewOfFile(
						m_hObjectFileMapping,
						FILE_MAP_ALL_ACCESS,
						(DWORD)(m_qwFileOffset >> 32),
						0/*(DWORD)(m_qwFileOffset & 0xFFFFFFFF)*/,
						m_dwMappingFileSize
					);

					if (NULL == m_lpMappingBaseAddress) {
						if (m_hObjectFileMapping) {
							::CloseHandle(m_hObjectFileMapping);
							m_hObjectFileMapping = NULL;
						}
						if (m_hObjectFile) {
							::CloseHandle(m_hObjectFile);
							m_hObjectFile = NULL;
						}
						return false;
					}///if (NULL == m_lpMappingBaseAddress)
					IsNewPage = true;
				}///if ((m_dwMappingFileSize * 6 / 10) < (m_qwFileOffset + _nData))
				memcpy((char*)m_lpMappingBaseAddress + m_qwFileOffset, _pData, _nData);
				m_qwFileOffset += _nData;
				auto thisHeader = (tagMapFileHeader*)m_lpMappingBaseAddress;
				thisHeader->length = m_qwFileOffset - MAPFILEHEADERLEN;
				thisHeader->length_total = m_dwMappingFileSize;
				thisHeader->update_timestamp = ::time(0);
				::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset);
				if (IsNewPage) {
					/*return reinterpret_cast<long long>(m_lpMappingBaseAddress);*/
				}
				memcpy(&m_FileInfo, thisHeader, sizeof(m_FileInfo));
				return true;
			}///Write()
			//! *ƫ��д��
			//! *�÷���ֻ��������д���ļ��������ݴ�С�ķ�Χ����
			//! *ʹ�ø÷������ļ���Ч���ݴ�С������ı�
			bool Write(unsigned long _Offset, const char* _pData, unsigned long _nData) {
				if (!m_IsOpen) {
					return false;
				}
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!m_hObjectFileMapping) {
					return false;
				}
				//! ָ����ƫ��λ�ò�����Ч���ݷ�Χ��
				auto PosBegin = _Offset + MAPFILEHEADERLEN;
				auto PosEnd = _Offset + MAPFILEHEADERLEN + _nData;

				if (PosBegin < MAPFILEHEADERLEN || PosEnd > m_dwMappingFileSize) {
					return false;
				}
				memcpy((char*)m_lpMappingBaseAddress + MAPFILEHEADERLEN + _Offset, _pData, _nData);
				//! �����ļ�ͷ
				auto thisHeader = (tagMapFileHeader*)m_lpMappingBaseAddress;
				thisHeader->update_timestamp = ::time(0);
				memcpy(&m_FileInfo, thisHeader, MAPFILEHEADERLEN);
				::FlushViewOfFile(m_lpMappingBaseAddress, (DWORD)m_qwFileOffset);
				return true;
			}///Write()
		private:
			std::string m_strFilePath;
			tagMapFileHeader m_FileInfo;
			volatile bool m_IsClearOpen = false;
			volatile bool m_IsOpen = false;

			INT64		m_qwFileOffset = 0;						//	�ڴ�ӳ���ļ�ƫ�ƣ�β��λ�ã���С����Լ��ʼ������ΪEOF
			HANDLE	m_hObjectFile = nullptr;						//	�����ļ����
			HANDLE	m_hObjectFileMapping = nullptr;			//	ӳ���ļ����
			LPVOID	m_lpMappingBaseAddress = nullptr;		//	ӳ���ļ���ַ
			DWORD	m_dwMappingFileSize = 0;			//	��ǰ�ڴ��ļ�ӳ���С AllocationGranularity ��������
			DWORD	m_dwContentFileSize = 0;				//	ʵ���ļ����ݴ�С

			std::mutex m_mutex;
			std::condition_variable m_condition_variable;
		};




		class IniStream final {
		public:
			IniStream() { m_osm.str(""); }
			~IniStream() { m_osm.str(""); }
		public:
			IniStream& Append(const sk::stringa& app) { m_osm << "[" << app << "]" << std::endl; return *this; }
			IniStream& Append(const sk::stringa& key, const sk::stringa& val) { m_osm << key << "=" << val << std::endl; return *this; }
			void Clear() { m_osm.str(""); }
			sk::stringa Get() const { return m_osm.str(); }
			void Set(const sk::stringa& src) { m_osm.str(""); m_osm << src; }
		private:
			std::ostringstream m_osm;
		};

		class IniFile final {
		public:
			IniFile() {}
			~IniFile() {}
		public:
			bool Write(const std::string& AppName, const std::string& KeyName, const std::string& Value) {
				if (AppName.empty()) {
					return false;
				}
				bool exists = false;
				auto findAppName = m_IniMapping.find(AppName);
				if (findAppName != m_IniMapping.end()) {
					if (!KeyName.empty()) {
						auto findKeyName = findAppName->second.find(KeyName);
						if (findKeyName != findAppName->second.end()) {
							findAppName->second.erase(findKeyName);
						}
						findAppName->second.insert({ {KeyName,Value} });
						exists = true;
					}
				}
				else {
					if (!KeyName.empty()) {
						m_IniMapping.insert({ {AppName,std::map<std::string,std::string>{ {KeyName,Value}}} });
					}

				}
				return exists;
			}
			const std::string Read(const std::string& AppName, const std::string& KeyName) {
				auto findApp = m_IniMapping.find(AppName);
				if (findApp != m_IniMapping.end()) {
					auto findKey = findApp->second.find(KeyName);
					if (findKey != findApp->second.end()) {
						return findKey->second;
					}
				}
				return "";
			}
			std::string Output() const {
				std::string out;
				for (const auto& app_name : m_IniMapping) {
					out.append("[").append(app_name.first).append("]").append("\n");
					for (const auto& session : app_name.second) {
						out.append(session.first).append("=").append(session.second).append("\n");
					}
				}
				return out;
			}
			std::string OutputUtf8() const {
				std::string out;
				for (const auto& app_name : m_IniMapping) {
					out.append("[").append(app_name.first).append("]").append("\n");
					for (const auto& session : app_name.second) {
						out.append(session.first).append("=").append(session.second).append("\n");
					}
				}
				return sk::StringConvert::MBytesToUTF8(out);
			}
			//! ֻ֧�ֽ��������װ�ĺϷ���ini��ʽ��
			int Input(const std::string& src) {
				m_IniMapping.clear();

				std::vector<sk::stringa> vrLines;
				sk::Helper::OneUnknownParseA(src, '\n', vrLines);

				char App[_MAX_PATH] = { 0 };
				//! KO ��ע���� ��������
				for (auto& val : vrLines) {
					sk::stringa Key, Val;
					if (val[0] == '[') {
						int pos = 0;
						for (auto it = val.begin(); it != val.end(); ++it) {
							if (*it != '0' && *it != '[' && *it != ']') {
								App[pos] = *it;
								++pos;
							}
							else {
								App[pos] = 0;
							}
						}

						if (App[0]) {
							//m_IniMapping.insert({ {App,std::map<std::string,std::string>{ {Key,Val}}} });
							m_IniMapping.insert({ {App,std::map<std::string,std::string>{ }} });
						}

					}
					else {
						std::vector<sk::stringa> vrSession;
						sk::Helper::OneUnknownParseA(val, '=', vrSession);
						if (vrSession.size() >= 2) {
							Key = vrSession[0];
							Val = vrSession[1];
						}


						if (App[0] && !Key.empty() && !Val.empty()) {
							auto pApp = m_IniMapping.find(App);
							if (pApp == m_IniMapping.end()) {
								m_IniMapping.insert({ {App,std::map<std::string,std::string>{ {Key,Val}}} });
							}
							else {
								auto findKey = pApp->second.find(Key);
								if (findKey != pApp->second.end()) {
									pApp->second.erase(findKey);
								}
								pApp->second.insert({ {Key,Val} });
							}
						}
					}
				}

				return 0;
			}
		private:
			std::map<std::string, std::map<std::string, std::string>> m_IniMapping;
		};

	}///namespace file
}///namespace sk




/// /*�����������Ϻ���**/
/// /*2019��06��29�� 16:13:38|138**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_FILE_H__FEF97_AB485DCF_416C07B8D403283_4E59__TAIL__
