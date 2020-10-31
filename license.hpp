#if !defined(AFX_SKSTU_LICENSE_H__14DB0_60510085_6E346F55C123621_51BC__HEAD__)
#define AFX_SKSTU_LICENSE_H__14DB0_60510085_6E346F55C123621_51BC__HEAD__

#include "shlobj.h"
#include "encrypt.hpp"
#include "time.hpp"

namespace sk {

	//!@ 授权文件名
	CONST CHAR LICENSE_FILENAME[] = "LICENSE.SK";
	//!@ 预授权文件名
	CONST CHAR LICENSE_FILENAME_READY[] = "LICENSE.READY";
	//!@ 授权文件安装目录
	CONST CHAR LICENSE_ROOT[] = "\\SK\\License\\";
	CONST CHAR LICENSE_BRIDGE[] = "\\SK\\License\\BRIGDE\\";
	CONST CHAR LICENSE_FOLLOWSA[] = "\\SK\\License\\FOLLOWSA\\";
	CONST CHAR LICENSE_FOLLOWMA[] = "\\SK\\License\\FOLLOWMA\\";

#pragma pack(push,1)
	typedef struct tagLicenseInfo
	{
		enum struct EnProductType : long long
		{
			EN_PRODUCT_TYPE_UNDEFINED = 0,
			EN_PRODUCT_TYPE_FOLLOWOBJ_MA = 1,
			EN_PRODUCT_TYPE_FOLLOWOBJ_SA = EN_PRODUCT_TYPE_FOLLOWOBJ_MA << 1,
			EN_PRODUCT_TYPE_BRIGDE_PROXY = EN_PRODUCT_TYPE_FOLLOWOBJ_MA << 2,

			EN_PRODUCT_TYPE_COMMON = EN_PRODUCT_TYPE_FOLLOWOBJ_MA | EN_PRODUCT_TYPE_FOLLOWOBJ_SA | EN_PRODUCT_TYPE_BRIGDE_PROXY,
		};

		enum struct EnProductVersionType : long long
		{
			EN_PRODUCT_VERSION_TYPE_INVALID = -1,
			EN_PRODUCT_VERSION_TYPE_BETA = 0,
			EN_PRODUCT_VERSION_TYPE_DEMO = 1,
			EN_PRODUCT_VERSION_TYPE_TRIAL = 2,
			EN_PRODUCT_VERSION_TYPE_OFFICIAL = 3,
			EN_PRODUCT_VERSION_TYPE_FINAL = 4,
		};

		enum struct EnProductLicenseType : long long
		{
			EN_PRODUCT_LICENSE_TYPE_WEEK_1 = 0,//! 一周
			EN_PRODUCT_LICENSE_TYPE_MONTH_1 = 1,//! 一个月
			EN_PRODUCT_LICENSE_TYPE_MONTH_3 = 2,//! 三个月
			EN_PRODUCT_LICENSE_TYPE_MONTH_6 = 3,//! 半年
			EN_PRODUCT_LICENSE_TYPE_YEAR_1 = 4,//! 一年
			EN_PRODUCT_LICENSE_TYPE_YEAR_3 = 5,//! 三年
			EN_PRODUCT_LICENSE_TYPE_FINAL = 6,//! 终身授权
		};

		__int64 Head;
		CHAR ClientLogo[64];
		EnProductType ProductType;
		EnProductVersionType ProductVersionType;
		EnProductLicenseType ProductLicenseType;
		MachineHardwareInformation Machine;
		__int64 DateEnd; //! 结束日期
		__int64 DateStart;//! 起始日期
		__int64 Tail;

		tagLicenseInfo()
		{
			memset(this, 0x00, sizeof(*this));
			Head = 0xFAC9C2D0;
			Tail = 0xB4B4AAC1;

			ProductVersionType = EnProductVersionType::EN_PRODUCT_VERSION_TYPE_INVALID;
		}
		bool Verify() const { return Head == 0xFAC9C2D0 && Tail == 0xB4B4AAC1; }
		void operator=(const tagLicenseInfo& obj) { memcpy(this, &obj, sizeof(*this)); }

		const char* ProductTypeStrA(bool cn = true) const
		{
			switch (ProductType)
			{
			case EnProductType::EN_PRODUCT_TYPE_FOLLOWOBJ_SA:
			{
				return cn ? "MT5跟单插件(SA)" : "Followobj for matetrader5(SA)";
			}break;
			case EnProductType::EN_PRODUCT_TYPE_FOLLOWOBJ_MA:
			{
				return cn ? "MT5跟单组件(MA)" : "Followobj for matetrader5(MA)";
			}break;
			case EnProductType::EN_PRODUCT_TYPE_BRIGDE_PROXY:
			{
				return cn ? "MT5清算桥系统组件(ASSY)" : "Bridgeobj for matetrader5(ASSY)";
			}break;
			}
			return cn ? "未知产品" : "UNKNOWN";
		}

		const char* ProductVersionTypeStrA(bool cn = true) const
		{
			switch (ProductVersionType)
			{
			case EnProductVersionType::EN_PRODUCT_VERSION_TYPE_BETA:
			{
				return cn ? "测试版" : "BETA";
			}break;
			case EnProductVersionType::EN_PRODUCT_VERSION_TYPE_DEMO:
			{
				return cn ? "演示版" : "DEMO";
			}break;
			case EnProductVersionType::EN_PRODUCT_VERSION_TYPE_FINAL:
			{
				return cn ? "最终版" : "FINAL";
			}break;
			case EnProductVersionType::EN_PRODUCT_VERSION_TYPE_OFFICIAL:
			{
				return cn ? "正式版" : "OFFICIAL";
			}break;
			case EnProductVersionType::EN_PRODUCT_VERSION_TYPE_TRIAL:
			{
				return cn ? "试用版" : "TRIAL";
			}break;
			}
			return cn ? "未知版本" : "UNKNOWN";
		}

		const sk::stringa ProductLicenseToStrA(bool cn = true) const
		{
			if (DateEnd > 0 && DateEnd < MAXINT64)
			{
				auto licenseTo = sk::time::Time::TimestampToTimeStr(DateEnd);
				cn ? licenseTo.insert(0, "产品授权截至: ") : licenseTo.insert(0, "Product license period: ");
				return licenseTo;
			}
			return cn ? "待定" : "undetermined";
		}

		const sk::stringa ProductServiceToStrA(bool cn = true) const
		{
			if (DateEnd > 0 && DateEnd < MAXINT64)
			{
				auto serviceTo = sk::time::Time::TimestampToTimeStr(DateEnd);
				cn ? serviceTo.insert(0, "产品服务截至: ") : serviceTo.insert(0, "Product service period: ");
				return serviceTo;
			}
			return cn ? "待定" : "undetermined";
		}

	}LicenseInfo, LicenseParam;
#pragma pack(pop)


	class License final {
	public:
		License() {}
		~License() {}
	public:
		static int Made
		(
			__in const sk::stringa& ClientLogo,/*客户Logo*/
			__in const sk::stringa& ReadyLicenseFileBuffer,/*预授权文件缓存*/
			__inout LicenseInfo& licenseInfo/*授权信息*/
		)
		{
			const __int64 oneDay = 60 * 60 * 24;

			sk::stringa targetBuffer = ReadyLicenseFileBuffer;
			//! 客户机器硬件信息
			auto destr = sk::encrypt::wemade::Decode6Bit(targetBuffer);
			if (destr.empty())
			{
				return -1;
			}
			memcpy(&licenseInfo.Machine, destr.data(), LENMACHINEHARDWAREINFORMATION);
			//! 授权当日8:0:0(AM)开始计时
			licenseInfo.DateStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			licenseInfo.DateStart -= licenseInfo.DateStart % oneDay;
			licenseInfo.DateEnd = licenseInfo.DateStart;
			switch (licenseInfo.ProductLicenseType)
			{
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_MONTH_1:
			{
				licenseInfo.DateEnd += oneDay * 31;
			}break;
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_MONTH_3:
			{
				licenseInfo.DateEnd += oneDay * 31 * 3;
			}break;
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_MONTH_6:
			{
				licenseInfo.DateEnd += oneDay * 31 * 6;
			}break;
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_WEEK_1:
			{
				licenseInfo.DateEnd += oneDay * 7;
			}break;
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_YEAR_1:
			{
				licenseInfo.DateEnd += oneDay * 365;
			}break;
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_YEAR_3:
			{
				licenseInfo.DateEnd += oneDay * 365 * 3;
			}break;
			case sk::LicenseInfo::EnProductLicenseType::EN_PRODUCT_LICENSE_TYPE_FINAL:
			{
				licenseInfo.DateEnd = MAXINT64;
			}break;
			default:
			{
				return -1;
			}
			}
			return 0;
		}

		static SKAPIRES Verify
		(
			__in const LicenseInfo::EnProductType& productType,
			__in const MachineHardwareInformation& local,
			__in const __int64& targetTradeSystemTimestamp,
			__in const sk::stringa& licenseInfoBuffer,
			__out LicenseInfo& licenseInfo
		)
		{
			memset(&licenseInfo, 0x00, sizeof(licenseInfo));
			if (licenseInfoBuffer.empty())
			{
				return SKAPIRES::LICENSE_RET_ERROR_BUFFER_EMPTY;
			}
			sk::stringa targetBuffer = licenseInfoBuffer;
			auto destr = sk::encrypt::wemade::Decode6Bit(targetBuffer);
			if (destr.empty())
			{
				return SKAPIRES::LICENSE_RET_ERROR_DECODE_FAILED;
			}
			const __int64 oneDay = 60 * 60 * 24;
			memcpy(&licenseInfo, destr.data(), sizeof(licenseInfo));

			//!@ 授权文件的完整性
			if (!licenseInfo.Verify())
			{
				return SKAPIRES::LICENSE_RET_ERROR_DATA_VERIFY_FAILED;
			}
			//!@ 授权文件指定的产品类型
			if (licenseInfo.ProductType != productType)
			{
				return SKAPIRES::LICENSE_RET_ERROR_PRODUCT_TYPE;
			}
			//!@ 硬件信息校验
			if (!licenseInfo.Machine.Verify() || licenseInfo.Machine != local)
			{
				return SKAPIRES::LICENSE_RET_ERROR_HARDWAREINFO;
			}
			//!@ 授权期限检查
			if (licenseInfo.DateEnd < targetTradeSystemTimestamp)
			{
				return SKAPIRES::LICENSE_RET_ERROR_DATE;
			}
			return SKAPIRES::LICENSE_RET_SUCCESS;
		}

		static int Parse
		(
			__in const sk::stringa& licenseInfoBuffer,
			__out LicenseInfo& licenseInfo
		)
		{
			memset(&licenseInfo, 0x00, sizeof(licenseInfo));
			if (licenseInfoBuffer.empty()) { return -1; }
			sk::stringa targetBuffer = licenseInfoBuffer;
			auto destr = sk::encrypt::wemade::Decode6Bit(targetBuffer);
			if (destr.empty()) { return -1; }
			memcpy(&licenseInfo, destr.data(), sizeof(licenseInfo));
			if (!licenseInfo.Verify()) { return -1; }
			return 0;
		}

		static int UnInstall()
		{
			char LocalAppData[_MAX_PATH] = { 0 };
			char TempPath[_MAX_PATH] = { 0 };
			LPITEMIDLIST pidl = NULL;
			auto ret = SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_APPDATA, &pidl);
			if (pidl && SHGetPathFromIDListA(pidl, TempPath))
			{
				GetShortPathNameA(TempPath, LocalAppData, _MAX_PATH);
			}
			sk::stringa strLocalAppData(LocalAppData);
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_ROOT).c_str());
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_BRIDGE).c_str());
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_FOLLOWSA).c_str());
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_FOLLOWMA).c_str());
			sk::stringa targetFile;
			targetFile = strLocalAppData + sk::LICENSE_BRIDGE + sk::LICENSE_FILENAME;
			::DeleteFileA(targetFile.c_str());
			targetFile = strLocalAppData + sk::LICENSE_FOLLOWMA + sk::LICENSE_FILENAME;
			::DeleteFileA(targetFile.c_str());
			targetFile = strLocalAppData + sk::LICENSE_FOLLOWSA + sk::LICENSE_FILENAME;
			::DeleteFileA(targetFile.c_str());
			return 0;
		}
		static int Install
		(
			__in const LicenseInfo::EnProductType& typeProduct,
			__in const sk::packet& licenseBuf
		)
		{
			char LocalAppData[_MAX_PATH] = { 0 };
			char TempPath[_MAX_PATH] = { 0 };
			LPITEMIDLIST pidl = NULL;
			auto ret = SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_APPDATA, &pidl);
			if (pidl && SHGetPathFromIDListA(pidl, TempPath))
			{
				GetShortPathNameA(TempPath, LocalAppData, _MAX_PATH);
			}
			sk::stringa strLocalAppData(LocalAppData);
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_ROOT).c_str());
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_BRIDGE).c_str());
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_FOLLOWSA).c_str());
			sk::Helper::CreateDirectoryA((strLocalAppData + sk::LICENSE_FOLLOWMA).c_str());
			sk::stringa targetFile;
			switch (typeProduct)
			{
			case LicenseInfo::EnProductType::EN_PRODUCT_TYPE_BRIGDE_PROXY:
			{
				targetFile = strLocalAppData + sk::LICENSE_BRIDGE + sk::LICENSE_FILENAME;
			}break;
			case LicenseInfo::EnProductType::EN_PRODUCT_TYPE_FOLLOWOBJ_MA:
			{
				targetFile = strLocalAppData + sk::LICENSE_FOLLOWMA + sk::LICENSE_FILENAME;
			}break;
			case LicenseInfo::EnProductType::EN_PRODUCT_TYPE_FOLLOWOBJ_SA:
			{
				targetFile = strLocalAppData + sk::LICENSE_FOLLOWSA + sk::LICENSE_FILENAME;
			}break;
			}
			std::ofstream ofs(targetFile, std::ios::out | std::ios::binary | std::ios::trunc);
			if (ofs.is_open())
			{
				ofs.write(licenseBuf.data(), licenseBuf.size());
				ofs.flush();
				ofs.close();
				return 0;
			}
			return -1;
		}

		static sk::packet Load
		(
			const LicenseInfo::EnProductType& typeProduct
		)
		{
			char LocalAppData[_MAX_PATH] = { 0 };
			IMalloc* pShellMalloc = NULL;        // A pointer to the shell's IMalloc interface
			LPITEMIDLIST pidlItem = NULL;         // The item's PIDL.
			if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
			{
				if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_APPDATA, &pidlItem)))
				{
					SHGetPathFromIDListA(pidlItem, LocalAppData);
					pShellMalloc->Free(pidlItem);
				}
				pShellMalloc->Release();
			}

			sk::packet result;

			if (LocalAppData[0] == 0)
			{
				return result;
			}

			sk::stringa strLocalAppData(LocalAppData);


			switch (typeProduct)
			{
			case LicenseInfo::EnProductType::EN_PRODUCT_TYPE_BRIGDE_PROXY:
			{
				auto licenseBuF = sk::Helper::LoadFile((strLocalAppData + sk::LICENSE_BRIDGE + +sk::LICENSE_FILENAME).c_str());

				if (!licenseBuF.empty())
				{
					result.append(&licenseBuF[0], licenseBuF.size());
				}
			}break;
			case LicenseInfo::EnProductType::EN_PRODUCT_TYPE_FOLLOWOBJ_MA:
			{
				auto licenseBuF = sk::Helper::LoadFile((strLocalAppData + sk::LICENSE_FOLLOWMA + +sk::LICENSE_FILENAME).c_str());

				if (!licenseBuF.empty())
				{
					result.append(&licenseBuF[0], licenseBuF.size());
				}
			}break;
			case LicenseInfo::EnProductType::EN_PRODUCT_TYPE_FOLLOWOBJ_SA:
			{
				auto licenseBuF = sk::Helper::LoadFile((strLocalAppData + sk::LICENSE_FOLLOWSA + +sk::LICENSE_FILENAME).c_str());

				if (!licenseBuF.empty())
				{
					result.append(&licenseBuF[0], licenseBuF.size());
				}
			}break;
			}
			return result;
		}

	};




}///namespace sk




/// /*新生联创（上海）**/
/// /*2020年02月13日 19:15:26|137**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_LICENSE_H__14DB0_60510085_6E346F55C123621_51BC__TAIL__