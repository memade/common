#if !defined(AFX_SKSTU_encrypt_H__5BBB0_9815715E_CE3AD2CCEA17B87_3421__HEAD__)
#define AFX_SKSTU_encrypt_H__5BBB0_9815715E_CE3AD2CCEA17B87_3421__HEAD__

namespace sk {
	namespace encrypt {
		class wemade final {
		public:
			~wemade() = delete;
			static std::string Encode6Bit(const std::string& strSrc) {
				std::string result;
				auto nSrcLen = strSrc.length();
				auto pszSrc = (unsigned char*)strSrc.c_str();
				int				nDestPos = 0;
				int				nRestCount = 0;
				unsigned char	chMade = 0, chRest = 0;
				for (decltype(nSrcLen) i = 0; i < nSrcLen; i++)
				{
					chMade = ((chRest | (pszSrc[i] >> (2 + nRestCount))) & 0x3f);
					chRest = (((pszSrc[i] << (8 - (2 + nRestCount))) >> 2) & 0x3f);
					nRestCount += 2;
					if (nRestCount < 6) {
						result.push_back(chMade + 0x3c);
					}
					else {
						result.push_back(chMade + 0x3c);
						result.push_back(chRest + 0x3c);

						nRestCount = 0;
						chRest = 0;
					}
				}
				if (nRestCount > 0) {
					result.push_back(chRest + 0x3c);
				}
				if (!result.empty()) {
					result.insert(0, "#", strlen("#"));
					result.push_back('!');
				}
				return result;
			}///Encode6Bit
			static std::string Decode6Bit(std::string& strSrc) {
				std::string result;
				if (strSrc.empty()) {
					return result;
				}
				auto endPos = strSrc.rfind(0x21);
				if (endPos == std::string::npos)
				{
					return result;
				}
				else
				{
					strSrc.erase(endPos, strSrc.size() - endPos);
					strSrc.push_back(0x21);
				}
				if (0x23 != strSrc.front() || 0x21 != strSrc.back()) {
					return result;
				}
				strSrc.erase(strSrc.begin());
				strSrc.pop_back();
				const unsigned char Decode6BitMask[5] = { 0xfc, 0xf8, 0xf0, 0xe0, 0xc0 };
				int				nDestPos = 0, nBitPos = 2;
				int				nMadeBit = 0;
				unsigned char	ch, chCode, tmp;
				for (auto it = strSrc.begin(); it != strSrc.end(); ++it) {
					if ((*it - 0x3c) >= 0) {
						ch = *it - 0x3c;
					}
					else {
						nDestPos = 0;
						break;
					}
					if ((nMadeBit + 6) >= 8) {
						chCode = (tmp | ((ch & 0x3f) >> (6 - nBitPos)));
						result.push_back(chCode);
						nMadeBit = 0;
						if (nBitPos < 6)
							nBitPos += 2;
						else {
							nBitPos = 2;
							continue;
						}
					}
					tmp = ((ch << nBitPos) & Decode6BitMask[nBitPos - 2]);
					nMadeBit += (8 - nBitPos);
				}
				return result;
			}///Decode6Bit
		};


		static const std::string base64_chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";
		static inline bool is_base64(unsigned char c) {
			return (isalnum(c) || (c == '+') || (c == '/'));
		}
		class base64 final
		{
		public:
			base64() {};
			~base64() = delete;
		public:
			static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
				std::string ret;
				int i = 0;
				int j = 0;
				unsigned char char_array_3[3];
				unsigned char char_array_4[4];
				while (in_len--) {
					char_array_3[i++] = *(bytes_to_encode++);
					if (i == 3) {
						char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
						char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
						char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
						char_array_4[3] = char_array_3[2] & 0x3f;
						for (i = 0; (i < 4); i++)
							ret += base64_chars[char_array_4[i]];
						i = 0;
					}
				}
				if (i)
				{
					for (j = i; j < 3; j++)
						char_array_3[j] = '\0';
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;
					for (j = 0; (j < i + 1); j++)
						ret += base64_chars[char_array_4[j]];
					while ((i++ < 3))
						ret += '=';
				}
				return ret;
			}
			static std::string base64_decode(std::string const& encoded_string) {
				int in_len = static_cast<int>(encoded_string.size());
				int i = 0;
				int j = 0;
				int in_ = 0;
				unsigned char char_array_4[4], char_array_3[3];
				std::string ret;
				while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
					char_array_4[i++] = encoded_string[in_]; in_++;
					if (i == 4) {
						for (i = 0; i < 4; i++)
							char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
						char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
						char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
						char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
						for (i = 0; (i < 3); i++)
							ret += char_array_3[i];
						i = 0;
					}
				}
				if (i) {
					for (j = i; j < 4; j++)
						char_array_4[j] = 0;
					for (j = 0; j < 4; j++)
						char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
					for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
				}
				return ret;
			}
		};


		// define
#define UInt32 unsigned int
#define BIT_OF_BYTE  8
#define BIT_OF_GROUP 512
#define SRC_DATA_LEN 64

// 四个非线性函数宏定义
#define DEF_F(X, Y, Z ) ((( (X) & (Y) )|((~X)&(Z))))
#define DEF_G(X, Y, Z)  (((X)&(Z))|((Y)&(~Z)))
#define DEF_H(X, Y, Z)  ((X)^(Y)^(Z))
#define DEF_I(X, Y, Z)  ((Y)^((X)|(~Z)))

// 求链接数函数宏定义
#define FF(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + DEF_F(b,c,d) + Mj + ti),s));
#define GG(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + DEF_G(b,c,d) + Mj + ti),s));
#define HH(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + DEF_H(b,c,d) + Mj + ti),s));
#define II(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + DEF_I(b,c,d) + Mj + ti),s));

		class md5 final {
		public:
			// 4轮循环算法
			struct ParamDynamic {
				UInt32 ua_;
				UInt32 ub_;
				UInt32 uc_;
				UInt32 ud_;
			};
		public:
			md5() {}
			~md5() {};

			std::string Encode(std::string src_info) {
				ParamDynamic param;
				param.ua_ = kA;
				param.ub_ = kB;
				param.uc_ = kC;
				param.ud_ = kD;
				std::string result;
				const char* src_data = src_info.c_str();
				char* out_data_ptr = NULL;
				int total_byte = FillData(src_data, (int)strlen(src_data), &out_data_ptr);
				char* data_BIT_OF_GROUP = out_data_ptr;
				for (int i = 0; i < total_byte / (BIT_OF_GROUP / BIT_OF_BYTE); ++i) {
					data_BIT_OF_GROUP += i * (BIT_OF_GROUP / BIT_OF_BYTE);
					RotationCalculate(data_BIT_OF_GROUP, param);
				}
				if (NULL != out_data_ptr) {
					delete[] out_data_ptr, out_data_ptr = NULL;
				}
				result.append(GetHexStr(param.ua_));
				result.append(GetHexStr(param.ub_));
				result.append(GetHexStr(param.uc_));
				result.append(GetHexStr(param.ud_));
				return result;
			}
		protected:
			UInt32 CycleMoveLeft(UInt32 src_num, int bit_num_to_move) {
				if (0 >= bit_num_to_move) {
					return src_num;
				}
				return ((src_num << bit_num_to_move) \
					| (src_num >> (32 - bit_num_to_move)));
			}
			UInt32 FillData(const char* in_data_ptr, int data_byte_len, char** out_data_ptr) {
				int bit_num = data_byte_len * BIT_OF_BYTE;
				int grop_num = bit_num / BIT_OF_GROUP;
				int mod_bit_num = bit_num % BIT_OF_GROUP;
				int bit_need_fill = 0;
				if (mod_bit_num > (BIT_OF_GROUP - SRC_DATA_LEN)) {
					bit_need_fill = (BIT_OF_GROUP - mod_bit_num);
					bit_need_fill += (BIT_OF_GROUP - SRC_DATA_LEN);
				}
				else {
					bit_need_fill = (BIT_OF_GROUP - SRC_DATA_LEN) - mod_bit_num; //  这里多加了一个BIT_OF_GROUP，避免bit_need_fill正好等于0,暂时不加
				}
				int all_bit = bit_num + bit_need_fill;
				if (0 < bit_need_fill) {
					*out_data_ptr = new char[all_bit / BIT_OF_BYTE + SRC_DATA_LEN / BIT_OF_BYTE];
					memset(*out_data_ptr, 0, all_bit / BIT_OF_BYTE + SRC_DATA_LEN / BIT_OF_BYTE);
					// copy data
					memcpy(*out_data_ptr, in_data_ptr, data_byte_len);
					// fill rest data
					unsigned char* tmp = reinterpret_cast<unsigned char*>(*out_data_ptr);
					tmp += data_byte_len;
					// fill 1 and 0
					*tmp = 0x80;
					// fill origin data len
					unsigned long long* origin_num = (unsigned long long*)((*out_data_ptr) + ((all_bit / BIT_OF_BYTE)));
					*origin_num = data_byte_len * BIT_OF_BYTE;
				}
				return (all_bit / BIT_OF_BYTE + SRC_DATA_LEN / BIT_OF_BYTE);
			}
			void RoundF(char* data_BIT_OF_GROUP_ptr, ParamDynamic& param) {
				UInt32* M = reinterpret_cast<UInt32*>(data_BIT_OF_GROUP_ptr);
				int s[] = { 7, 12, 17, 22 };
				for (int i = 0; i < 16; ++i) {
					UInt32 ti = (UInt32)(k_ti_num_integer * abs(sin(i + 1)));
					if (i % 4 == 0) {
						FF(param.ua_, param.ub_, param.uc_, param.ud_, M[i], s[i % 4], ti);
					}
					else if (i % 4 == 1) {
						FF(param.ud_, param.ua_, param.ub_, param.uc_, M[i], s[i % 4], ti);
					}
					else if (i % 4 == 2) {
						FF(param.uc_, param.ud_, param.ua_, param.ub_, M[i], s[i % 4], ti);
					}
					else if (i % 4 == 3) {
						FF(param.ub_, param.uc_, param.ud_, param.ua_, M[i], s[i % 4], ti);
					}
				}
			}
			void RoundG(char* data_BIT_OF_GROUP_ptr, ParamDynamic& param) {
				UInt32* M = reinterpret_cast<UInt32*>(data_BIT_OF_GROUP_ptr);
				int s[] = { 5, 9, 14, 20 };
				for (int i = 0; i < 16; ++i) {
					UInt32 ti = (UInt32)(k_ti_num_integer * abs(sin(i + 1 + 16)));
					int index = (i * 5 + 1) % 16;
					if (i % 4 == 0) {
						GG(param.ua_, param.ub_, param.uc_, param.ud_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 1) {
						GG(param.ud_, param.ua_, param.ub_, param.uc_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 2) {
						GG(param.uc_, param.ud_, param.ua_, param.ub_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 3) {
						GG(param.ub_, param.uc_, param.ud_, param.ua_, M[index], s[i % 4], ti);
					}
				}
			}
			void RoundH(char* data_BIT_OF_GROUP_ptr, ParamDynamic& param) {
				UInt32* M = reinterpret_cast<UInt32*>(data_BIT_OF_GROUP_ptr);
				int s[] = { 4, 11, 16, 23 };
				for (int i = 0; i < 16; ++i) {
					UInt32 ti = (UInt32)(k_ti_num_integer * abs(sin(i + 1 + 32)));
					int index = (i * 3 + 5) % 16;
					if (i % 4 == 0) {
						HH(param.ua_, param.ub_, param.uc_, param.ud_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 1) {
						HH(param.ud_, param.ua_, param.ub_, param.uc_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 2) {
						HH(param.uc_, param.ud_, param.ua_, param.ub_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 3) {
						HH(param.ub_, param.uc_, param.ud_, param.ua_, M[index], s[i % 4], ti);
					}
				}
			}
			void RoundI(char* data_BIT_OF_GROUP_ptr, ParamDynamic& param) {
				UInt32* M = reinterpret_cast<UInt32*>(data_BIT_OF_GROUP_ptr);
				int s[] = { 6, 10, 15, 21 };
				for (int i = 0; i < 16; ++i) {
					UInt32 ti = (UInt32)(k_ti_num_integer * abs(sin(i + 1 + 48)));
					int index = (i * 7 + 0) % 16;
					if (i % 4 == 0) {
						II(param.ua_, param.ub_, param.uc_, param.ud_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 1) {
						II(param.ud_, param.ua_, param.ub_, param.uc_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 2) {
						II(param.uc_, param.ud_, param.ua_, param.ub_, M[index], s[i % 4], ti);
					}
					else if (i % 4 == 3) {
						II(param.ub_, param.uc_, param.ud_, param.ua_, M[index], s[i % 4], ti);
					}
				}
			}
			void RotationCalculate(char* data_512_ptr, ParamDynamic& param) {
				if (NULL == data_512_ptr) {
					return;
				}
				RoundF(data_512_ptr, param);
				RoundG(data_512_ptr, param);
				RoundH(data_512_ptr, param);
				RoundI(data_512_ptr, param);
				param.ua_ = kA + param.ua_;
				param.ub_ = kB + param.ub_;
				param.uc_ = kC + param.uc_;
				param.ud_ = kD + param.ud_;
			}
			std::string GetHexStr(unsigned int num_str) {
				std::string hexstr = "";
				char szch[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
				unsigned char* tmptr = (unsigned char*)&num_str;
				int len = sizeof(num_str);
				// 小端字节序，逆序打印
				for (int i = 0; i < len; i++) {
					unsigned char ch = tmptr[i] & 0xF0;
					ch = ch >> 4;
					hexstr.append(1, szch[ch]);
					ch = tmptr[i] & 0x0F;
					hexstr.append(1, szch[ch]);
				}
				return hexstr;
			}
		private:
			// 幻数定义
			static const int kA = 0x67452301;
			static const int kB = 0xefcdab89;
			static const int kC = 0x98badcfe;
			static const int kD = 0x10325476;
			static const unsigned long long k_ti_num_integer = 4294967296;
		};

	}///namespace encrypt
}///namespace sk




/// /*新生联创（上海）**/
/// /*2019年05月30日 11:50:06|617**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_encrypt_H__5BBB0_9815715E_CE3AD2CCEA17B87_3421__TAIL__