#if !defined(AFX_SKSTU_ZLIBS_H__6B85E_4A2261B0_F93DD5C21E75553_AFC3__HEAD__)
#define AFX_SKSTU_ZLIBS_H__6B85E_4A2261B0_F93DD5C21E75553_AFC3__HEAD__

#if defined _M_IX86
#if defined(_DEBUG)
#pragma comment(lib,"../../bin/lib/Win32/Debug/zlibstatic.lib")
#else
#pragma comment(lib,"../../bin/lib/Win32/Release/zlibstatic.lib")
#endif
#elif defined _M_X64
#if defined(_DEBUG)
#pragma comment(lib,"../../bin/lib/x64/Debug/zlibstatic.lib")
#else
#pragma comment(lib,"../../bin/lib/x64/Release/zlibstatic.lib")
#endif
#else
#endif

#include <string>
#include <memory>

#pragma warning(disable : 4267)
#include "../gpl/zlib-1.2.11/zlib.h"
#include "../gpl/zlib-1.2.11/build/zconf.h"

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


namespace sk {

	class ZLibs final {
	public:
		ZLibs() {}
		~ZLibs() = delete;
	public:
		static int zipCompress(__in const std::string& src,__out std::string& dest) noexcept {
			dest.clear();
			uLongf nCompress = compressBound(static_cast<uLongf>(src.size()));
			Bytef* pCompress = new Bytef[nCompress];
			int ret = compress(pCompress, &nCompress, (Bytef*)src.data(), static_cast<uLong>(src.size()));
			switch (ret) {
			case Z_OK: {
				dest.append((char*)pCompress, nCompress);
			}break;
			case Z_MEM_ERROR: {//!如果内存不够
			}break;
			case Z_BUF_ERROR: {//!目标缓冲区太小
			}break;
			}
			delete[] pCompress;
			pCompress = nullptr;
			return ret;
		}
		static int zipUnCompress(__in const std::string& src, __in const size_t& nraw,__out std::string& dest) noexcept {
			dest.clear();
			Bytef* pUnCompress = new Bytef[nraw];
			int ret = uncompress(pUnCompress, (uLongf*)&nraw, (Bytef*)src.data(), static_cast<uLong>(src.size()));
			switch (ret) {
			case Z_OK: {
				dest.append((char*)pUnCompress, nraw);
			}break;
			case Z_STREAM_ERROR: {
			}break;
			case Z_MEM_ERROR: {//!如果内存不够
			}break;
			case Z_BUF_ERROR: {//!目标缓冲区太小
			}break;
			}
			delete[] pUnCompress;
			pUnCompress = nullptr;
			return ret;
		}
	public:
		inline static bool gzipCompress(const std::string& src, std::string& dest, int level = -1);
		inline static bool gzipUnCompress(const std::string& src, std::string& dest);
	private://!@ GZip
		inline static size_t GZGetBound(const size_t& sourceLen);
		inline static int GZCompress(BYTE* pSrc, ULONG nSrc, BYTE* pDest, ULONG* nDest);
		inline static int GZUnCompress(BYTE* pSrc, ULONG nSrc, BYTE* pDest, ULONG* nDest);
	};

	inline bool ZLibs::gzipCompress(const std::string& data, std::string& compressedData, int level /*= -1*/) {
		auto out = new unsigned char[CHUNK];
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		if (deflateInit2_(&strm, level, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, 0, ZLIB_VERSION, (int)sizeof(z_stream)/*Z_DEFAULT_STRATEGY*/) != Z_OK)
		{
			SK_DELETE_PTR_BUFFER(out);
			return false;
		}
		strm.next_in = (unsigned char*)data.c_str();
		strm.avail_in = static_cast<unsigned int>(data.size());
		do {
			int have;
			strm.avail_out = CHUNK;
			strm.next_out = out;
			if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
			{
				SK_DELETE_PTR_BUFFER(out);
				return false;
			}
			have = CHUNK - strm.avail_out;
			compressedData.append((char*)out, have);
		} while (strm.avail_out == 0);
		if (deflateEnd(&strm) != Z_OK)
		{
			SK_DELETE_PTR_BUFFER(out);
			return false;
		}
		SK_DELETE_PTR_BUFFER(out);
		return true;
	}
	inline bool ZLibs::gzipUnCompress(const std::string& compressedData, std::string& data) {
		int ret;
		unsigned have;
		z_stream strm;
		auto out = new unsigned char[CHUNK];
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		if (inflateInit2_(&strm, 16 + MAX_WBITS, ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK)
		{
			SK_DELETE_PTR_BUFFER(out);
			return false;
		}
		strm.avail_in = static_cast<unsigned int>(compressedData.size());
		strm.next_in = (unsigned char*)compressedData.c_str();
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			switch (ret) {
			case Z_NEED_DICT:
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&strm);
				SK_DELETE_PTR_BUFFER(out);
				return false;
			}
			have = CHUNK - strm.avail_out;
			data.append((char*)out, have);
		} while (strm.avail_out == 0);

		if (inflateEnd(&strm) != Z_OK) {
			SK_DELETE_PTR_BUFFER(out);
			return false;
		}
		SK_DELETE_PTR_BUFFER(out);
		return true;
	}
	size_t ZLibs::GZGetBound(const size_t& sourceLen)
	{
		double len = (double)sourceLen;
		if (!len)
			return 0;
		if (len < 0)
			return 0;
		if (len > 250.0)
			len *= 1.5;
		else
			len *= 2.0;
		return (size_t)len;
	}

	inline int ZLibs::GZCompress(BYTE* data, ULONG ndata, BYTE* zdata, ULONG* nzdata)
	{
		z_stream c_stream;
		int err = 0;
		if (data && ndata > 0) {
			c_stream.zalloc = NULL;
			c_stream.zfree = NULL;
			c_stream.opaque = NULL;
			//只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
			if (deflateInit2_(&c_stream, -1, 8, 15 + 16, 8, 0, ZLIB_VERSION, (int)sizeof(z_stream)) != 0) return -1;
			c_stream.next_in = data;
			c_stream.avail_in = ndata;
			c_stream.next_out = zdata;
			c_stream.avail_out = *nzdata;
			while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
				if (inflate(&c_stream, 0) != 0) return -1;
			}
			if (c_stream.avail_in != 0) return c_stream.avail_in;
			for (;;) {
				if ((err = inflate(&c_stream, 4)) == 1) break;
				if (err != 0) return -1;
			}
			if (deflateEnd(&c_stream) != 0) return -1;
			*nzdata = c_stream.total_out;
			return 0;
		}
		return -1;
	}
	inline int ZLibs::GZUnCompress(BYTE* zdata, ULONG nzdata, BYTE* data, ULONG* ndata)
	{
		int err = 0;
		z_stream d_stream = { 0 }; /* decompression stream */
		static char dummy_head[2] = {
			0x8 + 0x7 * 0x10,
			(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
		};
		d_stream.zalloc = NULL;
		d_stream.zfree = NULL;
		d_stream.opaque = NULL;
		d_stream.next_in = zdata;
		d_stream.avail_in = 0;
		d_stream.next_out = data;
		//只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
		if (inflateInit2_(&d_stream, 15 + 16, ZLIB_VERSION, (int)sizeof(z_stream)) != 0) return -1;
		//if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
		while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
			d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
			if ((err = inflate(&d_stream, 0)) == 1) break;
			if (err != 0) {
				if (err == -3) {
					d_stream.next_in = (BYTE*)dummy_head;
					d_stream.avail_in = sizeof(dummy_head);
					if ((err = inflate(&d_stream, 0)) != 0) {
						return -1;
					}
				}
				else return -1;
			}
		}
		if (inflateEnd(&d_stream) != 0) return -1;
		*ndata = d_stream.total_out;
		return 0;
	}
}///namespace sk





/// /*新生联创（上海）**/
/// /*2019年11月15日 21:44:44|625**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_ZLIBS_H__6B85E_4A2261B0_F93DD5C21E75553_AFC3__TAIL__