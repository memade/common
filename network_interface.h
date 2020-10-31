#if !defined(AFX_SKSTU_interface_H__61039_B98A806E_349A41846199E5A_184E__HEAD__)
#define AFX_SKSTU_interface_H__61039_B98A806E_349A41846199E5A_184E__HEAD__

#pragma pack(push,1)
namespace sk {
	namespace network {
		//! ͨ�÷����ͷ��Ϣ -- �ɱ䳤���ͷ������ *! ���CONST_NET_PACKET_HEADER_MAX
		const int CONST_NET_PACKET_HEADER_MAX = 100;
		typedef struct tagNetPacketHeader {
			unsigned long long		sign_head;	/*packet head signature*/
			long long command;	//	�������
			long total_length;	//	�ܳ��ȣ�������ͷ��
			long total_data_count;	//	�������	���֧�� CONST_NET_PACKET_HEADER_MAX �����
			long total_data_length;	 // �����ܴ�С����������ͷ��
			long long single_data_mark[CONST_NET_PACKET_HEADER_MAX];
			long single_data_length[CONST_NET_PACKET_HEADER_MAX];	// �������ݴ�С
			long single_data_offset[CONST_NET_PACKET_HEADER_MAX];	//	����������ʼλ��ƫ���������data
			unsigned long long		sign_tail;	/*packet tail signature*/
			char data[1];	// ���������ʼ
			tagNetPacketHeader() {
				memset(this, 0x00, sizeof(*this));
				sign_head = 0xFAC9C2D0;
				sign_tail = 0xB4B4AAC1;
			}
			__inline bool Verify() const {
				return sign_head == 0xFAC9C2D0 && sign_tail == 0xB4B4AAC1;
			}
		}NetPacketHeader;
		//! ͨ�÷���ṹ����
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
				TYPE_DATA_WRAPPED_ENCRYPTION = 1,		/*������*/
				TYPE_DATA_WRAPPED_COMPRESS = 2,		/*��ѹ��*/
				TYPE_DATA_WRAPPED_ENCRYPTION_COMPRESS = 3,	/*�ȼ��ܺ�ѹ��*/
				TYPE_DATA_WRAPPED_COMPRESS_ENCRYPTION = 4,	/*��ѹ�������*/
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
			unsigned int					length_raw;		/*raw data length*/ //! ԭʼ���ݵĳ���
			unsigned int					length;				/*physical length*/ //! ʵ�ʳ��ȣ������ĳ���
			unsigned int					length_total;		/*the total length of this packet*/ //! ������ǰ��ͷ�������ݵĳ���
			unsigned int					length_compress;	/*compressed data length*/ //! ���������ѹ�� ��ô����������'data'  ѹ�������ĳ���
			unsigned int					length_encryption;	/*encryptoin data length*/ //! ��������˼��� ��ô����������'data'  ���ܺ�ĳ���
			char								reserve[8];
			unsigned long long		serial_number;			/*serial number*/ //! ��������˷ְ� ����Ƿְ������
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



/// /*�����������Ϻ���**/
/// /*2019��01��20�� 10:8:21|926**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_interface_H__61039_B98A806E_349A41846199E5A_184E__TAIL__
