#if !defined(AFX_SKSTU_interface_H__61039_B98A806E_349A41846199E5A_184E__HEAD__)
#define AFX_SKSTU_interface_H__61039_B98A806E_349A41846199E5A_184E__HEAD__

#pragma pack(push,1)
namespace sk {
	namespace network {
		//! 通用封包包头信息 -- 可变长封包头部定义 *! 最大CONST_NET_PACKET_HEADER_MAX
		const int CONST_NET_PACKET_HEADER_MAX = 100;
		typedef struct tagNetPacketHeader {
			unsigned long long		sign_head;	/*packet head signature*/
			long long command;	//	封包类型
			long total_length;	//	总长度（包含包头）
			long total_data_count;	//	封包个数	最大支持 CONST_NET_PACKET_HEADER_MAX 个封包
			long total_data_length;	 // 数据总大小（不包含本头）
			long long single_data_mark[CONST_NET_PACKET_HEADER_MAX];
			long single_data_length[CONST_NET_PACKET_HEADER_MAX];	// 单个数据大小
			long single_data_offset[CONST_NET_PACKET_HEADER_MAX];	//	单个数据起始位置偏移量相对于data
			unsigned long long		sign_tail;	/*packet tail signature*/
			char data[1];	// 封包数据起始
			tagNetPacketHeader() {
				memset(this, 0x00, sizeof(*this));
				sign_head = 0xFAC9C2D0;
				sign_tail = 0xB4B4AAC1;
			}
			__inline bool Verify() const {
				return sign_head == 0xFAC9C2D0 && sign_tail == 0xB4B4AAC1;
			}
		}NetPacketHeader;
		//! 通用封包结构定义
		typedef struct tagNetPacket {
			unsigned long long		sign_head;	/*packet head signature*/
			unsigned long long command;
			unsigned int size;
			unsigned int size_pak_total;
			unsigned long long		sign_tail;	/*packet tail signature*/
			char data[1];
			tagNetPacket() {
				memset(this, 0x00, sizeof(*this));
				sign_head = 0xFAC9C2D0;
				sign_tail = 0xB4B4AAC1;
				size_pak_total = sizeof(*this);
			}
			__inline bool Verify() const {
				return sign_head == 0xFAC9C2D0 && sign_tail == 0xB4B4AAC1;
			}
		}tagNetUdpPacket;
		/** Communication interface - Packet header definition
		* This protocol is suitable for use in long connections. */
		typedef struct HEADBIN {
			enum EnTypeDataFormat {
				TYPE_DATA_FORMAT_TEXT = 0,
				TYPE_DATA_FORMAT_JSON = 1,
				TYPE_DATA_FORMAT_XML = 2,
				TYPE_DATA_FORMAT_FIRST = TYPE_DATA_FORMAT_TEXT,
				TYPE_DATA_FORMAT_LAST = TYPE_DATA_FORMAT_XML,
			};
			enum EnTypeDataWrapped {
				TYPE_DATA_WRAPPED_BIN = 0,
				TYPE_DATA_WRAPPED_ENCRYPTION = 1,		/*仅加密*/
				TYPE_DATA_WRAPPED_COMPRESS = 2,		/*仅压缩*/
				TYPE_DATA_WRAPPED_ENCRYPTION_COMPRESS = 3,	/*先加密后压缩*/
				TYPE_DATA_WRAPPED_COMPRESS_ENCRYPTION = 4,	/*先压缩后加密*/
				TYPE_DATA_WRAPPED_FIRST = TYPE_DATA_WRAPPED_BIN,
				TYPE_DATA_WRAPPED_LAST = TYPE_DATA_WRAPPED_COMPRESS_ENCRYPTION,
			};
			enum EnTypeCompress {
				TYPE_DATA_UNCOMPRESS = 0,
				TYPE_DATA_COMPRESS_ZIP = 1,
				TYPE_DATA_COMPRESS_GZIP = 2,
				TYPE_DATA_COMPRESS_7Z = 3,
				TYPE_DATA_COMPRESS_FIRST = TYPE_DATA_COMPRESS_ZIP,
				TYPE_DATA_COMPRESS_LAST = TYPE_DATA_COMPRESS_7Z,
			};
			enum EnTypeEncryption {
				TYPE_DATA_UNENCRYPTION = 0,
				TYPE_DATA_ENCRYPTION_BASE64 = 1,
				TYPE_DATA_ENCRYPTION_WEMADE = 2,
				TYPE_DATA_ENCRYPTION_FIRST = TYPE_DATA_ENCRYPTION_BASE64,
				TYPE_DATA_ENCRYPTION_LAST = TYPE_DATA_ENCRYPTION_WEMADE,
			};
			unsigned long long		sign_head;	/*packet head signature*/
			long long						command;
			unsigned char				type_wrapped;
			unsigned char				type_compress;
			unsigned char				type_encryption;
			unsigned int					length_raw;		/*raw data length*/ //! 原始数据的长度
			unsigned int					length;				/*physical length*/ //! 实际长度，处理后的长度
			unsigned int					length_total;		/*the total length of this packet*/ //! 包含当前包头整套数据的长度
			unsigned int					length_compress;	/*compressed data length*/ //! 如果启用了压缩 那么这是数据区'data'  压缩处理后的长度
			unsigned int					length_encryption;	/*encryptoin data length*/ //! 如果启用了加密 那么这是数据区'data'  加密后的长度
			char								reserve[8];
			unsigned long long		serial_number;			/*serial number*/ //! 如果启用了分包 这个是分包的序号
			unsigned long long		sign_tail;	/*packet tail signature*/
			char								data[1];		/*first data address*/
			HEADBIN() {
				memset(this, 0, sizeof(*this));
				sign_head = 0xFAC9C2D0;
				sign_tail = 0xB4B4AAC1;
				length_total = sizeof(*this);
			}
			__inline bool Verify() const {
				return sign_head == 0xFAC9C2D0 && sign_tail == 0xB4B4AAC1;
			}
			//! @GetLength: run time use
			__inline unsigned int GetLength() {
				return (unsigned int)sizeof(*this) + length - (unsigned int)sizeof(char);
			}
		}*LPHEADBIN;
		const unsigned int LENHEADBIN = sizeof(HEADBIN);
	}///namespace network
}///namespace xslc
#pragma pack(pop)



/// /*新生联创（上海）**/
/// /*2019年01月20日 10:8:21|926**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_interface_H__61039_B98A806E_349A41846199E5A_184E__TAIL__
