#if !defined(AFX_SKSTU_zlib_H__A9DF7_76E3BA44_93C19DF11500421_C233__HEAD__)
#define AFX_SKSTU_zlib_H__A9DF7_76E3BA44_93C19DF11500421_C233__HEAD__

#define _zlibver_	  "1.2.11"

#define CHUNK 16384
#define windowBits 15
#define GZIP_ENCODING 16
#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1
#define Z_SYNC_FLUSH    2
#define Z_FULL_FLUSH    3
#define Z_FINISH        4
#define Z_BLOCK         5
#define Z_TREES         6
/* Allowed flush values; see deflate() and inflate() below for details */

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)
		/* Return codes for the compression/decompression functions. Negative values
		* are errors, positive values are used for special but normal events.
		*/
#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)
		/* compression levels */
#define Z_FILTERED            1
#define Z_HUFFMAN_ONLY        2
#define Z_RLE                 3
#define Z_FIXED               4
#define Z_DEFAULT_STRATEGY    0
		/* compression strategy; see deflateInit2() below for details */
#define Z_BINARY   0
#define Z_TEXT     1
#define Z_ASCII    Z_TEXT   /* for compatibility with 1.2.2 and earlier */
#define Z_UNKNOWN  2
		/* Possible values of the data_type field for deflate() */
#define Z_DEFLATED   8
		/* The deflate compression method (the only one supported in this version) */
#define Z_NULL  0  /* for initializing zalloc, zfree, opaque */
#ifndef MAX_WBITS
#  define MAX_WBITS   15 /* 32K LZ77 window */
#endif

#include <string>
#include <iostream>
#include <tuple>
#include <Windows.h>

namespace sk {
	namespace compress {
		class Zlib {
			typedef void*(*alloc_func)(void* opaque, unsigned int items, unsigned int size);
			typedef void(*free_func)(void* opaque, void* address);
			struct internal_state;
			typedef struct z_stream_s
			{
				const BYTE *next_in;     /* next input byte */
				unsigned int     avail_in;  /* number of bytes available at next_in */
				ULONG    total_in;  /* total number of input bytes read so far */
				BYTE    *next_out; /* next output byte should be put there */
				unsigned int     avail_out; /* remaining free space at next_out */
				ULONG    total_out; /* total number of bytes output so far */
				const char *msg;  /* last error message, NULL if no error */
				struct internal_state FAR *state; /* not visible by applications */
				alloc_func zalloc;  /* used to allocate the internal state */
				free_func  zfree;   /* used to free the internal state */
				void*     opaque;  /* private data object passed to zalloc and zfree */
				int     data_type;  /* best guess about the data type: binary or text */
				ULONG   adler;      /* adler32 value of the uncompressed data */
				ULONG   reserved;   /* reserved for future use */
			} z_stream;
			typedef z_stream FAR *z_streamp;

			typedef ULONG(__stdcall* lpfnCompressBound)(ULONG);
			typedef int(__stdcall* lpfnCompress)(BYTE*, ULONG*, const BYTE*, ULONG);
			typedef int(__stdcall *lpfnUncompress)(BYTE*, ULONG*, const BYTE*, ULONG);
			lpfnCompressBound m_zipCompressBound;
			lpfnCompress m_zipCompress;
			lpfnUncompress m_zipUncompress;
			typedef int(__stdcall* lpfnDeflateBound)(z_streamp, ULONG);
			typedef int(__stdcall* lpfnInflate)(z_streamp, int);
			typedef int(__stdcall* lpfnInflateInit2)(z_streamp, int, const char*, int);
			typedef int(__stdcall* lpfnInflateEnd)(z_streamp);
			typedef int(__stdcall* lpfnDeflate)(z_streamp, int);
			typedef int(__stdcall* lpfnDeflateInit2)(z_streamp, int, int, int, int, int, const char*, int);
			typedef int(__stdcall* lpfnDeflateEnd)(z_streamp);
			lpfnDeflate m_gzipDeflate;
			lpfnDeflateInit2 m_gzipDeflateInit2;
			lpfnDeflateEnd m_gzipDeflateEnd;
			lpfnInflate m_gzipInflate;
			lpfnInflateInit2 m_gzipInflateInit2;
			lpfnInflateEnd m_gzipInflateEnd;
			lpfnDeflateBound m_gzipDeflateBound;
		public:
			Zlib();
			~Zlib();
		private:
			bool LoadZLib();
			void UnLoadZLib();
			HMODULE m_dllZiplib;
		public://!@ Zip
			//!@ _pSrc : 待压缩的数据指针
			//!@ _nSrc : 待压缩的数据长度
			//!@ return 'empty' failed
			static 
				std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> 
				ZipCompress(const char* _pSrc, unsigned long _nSrc);
			//!@ _pSrc : 待解压的数据指针
			//!@ _nSrc : 待解压的数据长度
			//!@ _nSrcRaw : 未被压缩原来的长度
			//!@ return 'empty' failed
			static 
				std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> 
				ZipUnCompress(const char* _pSrc, unsigned long _nSrc, unsigned long _nSrcRaw);

			static 
				std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>>
				GZipUnCompress(const char* _pSrc, unsigned long _nSrc, unsigned long _nSrcRaw);
			static
				std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>>
				GZipCompress(const char* _pSrc, unsigned long _nSrc);

			static
				bool
				GZipUnCompress(__in const std::string& _strCompressData,__out std::string& _strUnCompressData);
			static
				bool
				GZipCompress(__in const std::string& _strSrc,__out std::string& _strDest);
		private:
			//! @获取压缩后所需的缓冲
			static ULONG CompressBound(ULONG sourceLen);
			static int Compress(BYTE* dest, ULONG* destLen, const BYTE* source, ULONG sourceLen);
			static int UnCompress(BYTE* dest, ULONG* destLen, const BYTE* source, ULONG sourceLen);
		private://!@ GZip
			//! @获取压缩后所需的缓冲
			static ULONG GZGetBound(ULONG sourceLen);
			static int GZCompress(BYTE* pSrc, ULONG nSrc, BYTE* pDest, ULONG *nDest);
			static int GZUnCompress(BYTE* pSrc, ULONG nSrc, BYTE* pDest, ULONG *nDest);
		private:
			// GZip Compression
			// @param data - the data to compress (does not have to be string, can be binary data)
			// @param compressedData - the resulting gzip compressed data
			// @param level - the gzip compress level -1 = default, 0 = no compression, 1= worst/fastest compression, 9 = best/slowest compression
			// @return - true on success, false on failure
			static bool GZCompress2(const std::string& data, std::string& compressedData, int level = -1);
			// GZip Decompression
			// @param compressedData - the gzip compressed data
			// @param data - the resulting uncompressed data (may contain binary data)
			// @return - true on success, false on failure
			static bool GZUnCompress2(const std::string& compressedData, std::string& data);
		};
	}//compress
}//xslc




/// /*新生联创（上海）**/
/// /*2019年03月9日 19:17:35|127**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_zlib_H__A9DF7_76E3BA44_93C19DF11500421_C233__TAIL__