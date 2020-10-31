#if !defined(AFX_SKSTU_UUID_H__B947A_DFC5D27B_D44041B7B6E454D_E03F__HEAD__)
#define AFX_SKSTU_UUID_H__B947A_DFC5D27B_D44041B7B6E454D_E03F__HEAD__

#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif

namespace sk {
	static const int global_uuid_length = 37;

	class UUID final {
	public:
		enum { UUID4_ESUCCESS = 0, UUID4_EFAILURE = -1 };

		UUID() {
			init();
		}
		~UUID() {

		}
	private:
		int init() {
#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
			int res;
			FILE* fp = fopen("/dev/urandom", "rb");
			if (!fp) {
				return UUID4_EFAILURE;
			}
			res = fread(seed, 1, sizeof(seed), fp);
			fclose(fp);
			if (res != sizeof(seed)) {
				return UUID4_EFAILURE;
			}

#elif defined(_WIN32)
			int res;
			HCRYPTPROV hCryptProv;
			res = CryptAcquireContext(
				&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
			if (!res) {
				return UUID4_EFAILURE;
			}
			res = CryptGenRandom(hCryptProv, (DWORD) sizeof(seed_), (PBYTE)seed_);
			CryptReleaseContext(hCryptProv, 0);
			if (!res) {
				return UUID4_EFAILURE;
			}

#else
#error "unsupported platform"
#endif
			return UUID4_ESUCCESS;
		}

		std::uint64_t xorshift128plus(std::uint64_t* s) {
			std::uint64_t s1 = s[0];
			const uint64_t s0 = s[1];
			s[0] = s0;
			s1 ^= s1 << 23;
			s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
			return s[1] + s0;
		}
		void generate(char* dst) {
			static const char* temp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
			static const char* chars = "0123456789abcdef";
			union { unsigned char b[16]; std::uint64_t word[2]; } s;
			const char* p;
			int i, n;
			/* get random */
			s.word[0] = xorshift128plus(seed_);
			s.word[1] = xorshift128plus(seed_);
			/* build string */
			p = temp;
			i = 0;
			while (*p) {
				n = s.b[i >> 1];
				n = (i & 1) ? (n >> 4) : (n & 0xf);
				switch (*p) {
				case 'x': *dst = chars[n];              i++;  break;
				case 'y': *dst = chars[(n & 0x3) + 8];  i++;  break;
				default: *dst = *p;
				}
				dst++, p++;
			}
			*dst = '\0';
		}
	public:
		sk::stringa uuid() {
			char buf[global_uuid_length] = { 0 };
			generate(buf);
			return sk::stringa(buf, global_uuid_length);
		}
	private:
		std::uint64_t seed_[2];
	};



}




/// /*新生联创（上海）**/
/// /*2019年07月30日 14:36:43|346**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_UUID_H__B947A_DFC5D27B_D44041B7B6E454D_E03F__TAIL__