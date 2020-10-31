#include "zlib.h"
#include "common.h"

namespace sk {
	namespace compress {
		static Zlib g_zlib;
		Zlib::Zlib() :
			m_zipCompressBound(nullptr),
			m_zipCompress(nullptr),
			m_zipUncompress(nullptr)
		{
			if (!LoadZLib()) {
				sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !");
			}
		}
		Zlib::~Zlib()
		{
			UnLoadZLib();
		}
		void Zlib::UnLoadZLib() {
			::FreeLibrary(m_dllZiplib);
		}

		bool Zlib::LoadZLib()
		{
			auto hSelfHandle = sk::Helper::GetSelfModuleHandle();
			if (!hSelfHandle) {
				m_dllZiplib = LoadLibraryA("zlibwapi.dll");
			}
			else {
				sk::stringa path = sk::Helper::GetCurrentProcessDirectoryA(hSelfHandle).get();
				m_dllZiplib = LoadLibraryA((path+"zlibwapi.dll").c_str());
			}
			if (!m_dllZiplib)
				return false;
			m_zipCompressBound = (lpfnCompressBound)GetProcAddress(m_dllZiplib, "compressBound");
			m_zipCompress = (lpfnCompress)GetProcAddress(m_dllZiplib, "compress"); 
			m_zipUncompress = (lpfnUncompress)GetProcAddress(m_dllZiplib, "uncompress");
			m_gzipDeflate = (lpfnDeflate)GetProcAddress(m_dllZiplib, "deflate");
			m_gzipDeflateInit2 = (lpfnDeflateInit2)GetProcAddress(m_dllZiplib, "deflateInit2_");
			m_gzipDeflateEnd = (lpfnDeflateEnd)GetProcAddress(m_dllZiplib, "deflateEnd");
			m_gzipInflate = (lpfnInflate)GetProcAddress(m_dllZiplib, "inflate");
			m_gzipInflateInit2 = (lpfnInflateInit2)GetProcAddress(m_dllZiplib, "inflateInit2_");
			m_gzipInflateEnd = (lpfnInflateEnd)GetProcAddress(m_dllZiplib, "inflateEnd");
			m_gzipDeflateBound = (lpfnDeflateBound)GetProcAddress(m_dllZiplib, "deflateBound");
			if (!m_zipCompress ||
				!m_zipCompressBound ||
				!m_zipUncompress ||
				!m_gzipDeflate ||
				!m_gzipDeflateInit2 ||
				!m_gzipDeflateEnd ||
				!m_gzipInflate ||
				!m_gzipInflateInit2 ||
				!m_gzipInflateEnd ||
				!m_gzipDeflateBound)
				return false;
			return true;
		}
		std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> 
			Zlib::ZipCompress(const char* _pSrc, unsigned long _nSrc) {
			std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> val;
			ULONG nCompressLen = static_cast<ULONG>(CompressBound(static_cast<ULONG>(_nSrc)));
			std::shared_ptr<char> spCompressBuffer(new char[nCompressLen], [](char *pbuffer) { delete[] pbuffer; });
			if (Z_OK == Compress((BYTE*)spCompressBuffer.get(), &nCompressLen, (BYTE*)_pSrc, (ULONG)_nSrc)) {
				val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(spCompressBuffer, (unsigned long)nCompressLen));
			}
			return val;
		}
		std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> 
			Zlib::ZipUnCompress(const char* _pSrc, unsigned long _nSrc, unsigned long _nSrcRaw) {
			std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> val;
			std::shared_ptr<char> spRawBuffer(new char[_nSrcRaw], [](char *pbuffer) { delete[] pbuffer; });
			ULONG nSrcRawLen = (ULONG)_nSrcRaw;
			if (Z_OK == UnCompress((BYTE*)spRawBuffer.get(), (ULONG*)&nSrcRawLen, (BYTE*)_pSrc, (ULONG)_nSrc)) {
				val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(spRawBuffer, (ULONG)nSrcRawLen));
			}
			return val;
		}

		std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>>
			Zlib::GZipCompress(const char* _pSrc, unsigned long _nSrc) {
			std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> val;
			ULONG nCompressLen = static_cast<ULONG>(CompressBound(static_cast<ULONG>(_nSrc)));
			std::shared_ptr<char> spCompressBuffer(new char[nCompressLen], [](char* pbuffer) { delete[] pbuffer; });
			if (Z_OK == GZCompress((BYTE*)_pSrc, _nSrc, (BYTE*)spCompressBuffer.get(), &nCompressLen)) {
				val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(spCompressBuffer, (unsigned long)nCompressLen));
			}
			return val;
		}
		std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>>
			Zlib::GZipUnCompress(const char* _pSrc, unsigned long _nSrc, unsigned long _nSrcRaw) {
			std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> val;
			std::shared_ptr<char> spRawBuffer(new char[_nSrcRaw], [](char* pbuffer) { delete[] pbuffer; });
			ULONG nSrcRawLen = (ULONG)_nSrcRaw;
			if (Z_OK == GZUnCompress((BYTE*)_pSrc, _nSrc, (BYTE*)spRawBuffer.get(), &nSrcRawLen)) {
				val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(spRawBuffer, (ULONG)nSrcRawLen));
			}
			return val;
		}
		bool
			Zlib::GZipUnCompress(__in const std::string& _strCompressData, __out std::string& _strUnCompressData) {
			return GZUnCompress2(_strCompressData, _strUnCompressData);
		}
		bool
			Zlib::GZipCompress(__in const std::string& _strSrc, __out std::string& _strDest) {
			return GZCompress2(_strSrc, _strDest, -1);
		}

		ULONG Zlib::CompressBound(ULONG sourceLen) {
			return g_zlib.m_zipCompressBound(sourceLen);
		}
		int Zlib::Compress(BYTE* dest, ULONG* destLen, const BYTE* source, ULONG sourceLen) {
			return g_zlib.m_zipCompress(dest, destLen, source, sourceLen);
		}
		int Zlib::UnCompress(BYTE* dest, ULONG* destLen, const BYTE* source, ULONG sourceLen) {
			return g_zlib.m_zipUncompress(dest, destLen, source, sourceLen);
		}
		ULONG Zlib::GZGetBound(ULONG sourceLen)
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
			return (ULONG)len;
		}

		int Zlib::GZCompress(BYTE* data, ULONG ndata, BYTE *zdata, ULONG *nzdata)
		{
			z_stream c_stream;
			int err = 0;
			if (data && ndata > 0) {
				c_stream.zalloc = NULL;
				c_stream.zfree = NULL;
				c_stream.opaque = NULL;
				//只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
				if (g_zlib.m_gzipDeflateInit2(&c_stream, -1, 8, 15 + 16, 8, 0, _zlibver_, (int)sizeof(z_stream)) != 0) return -1;
				c_stream.next_in = data;
				c_stream.avail_in = ndata;
				c_stream.next_out = zdata;
				c_stream.avail_out = *nzdata;
				while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
					if (g_zlib.m_gzipDeflate(&c_stream, 0) != 0) return -1;
				}
				if (c_stream.avail_in != 0) return c_stream.avail_in;
				for (;;) {
					if ((err = g_zlib.m_gzipDeflate(&c_stream, 4)) == 1) break;
					if (err != 0) return -1;
				}
				if (g_zlib.m_gzipDeflateEnd(&c_stream) != 0) return -1;
				*nzdata = c_stream.total_out;
				return 0;
			}
			return -1;
		}
		int Zlib::GZUnCompress(BYTE *zdata, ULONG nzdata, BYTE *data, ULONG *ndata)
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
			if (g_zlib.m_gzipInflateInit2(&d_stream, 15 + 16, _zlibver_, (int)sizeof(z_stream)) != 0) return -1;
			//if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
			while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
				d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
				if ((err = g_zlib.m_gzipInflate(&d_stream, 0)) == 1) break;
				if (err != 0) {
					if (err == -3) {
						d_stream.next_in = (BYTE*)dummy_head;
						d_stream.avail_in = sizeof(dummy_head);
						if ((err = g_zlib.m_gzipInflate(&d_stream, 0)) != 0) {
							return -1;
						}
					}
					else return -1;
				}
			}
			if (g_zlib.m_gzipInflateEnd(&d_stream) != 0) return -1;
			*ndata = d_stream.total_out;
			return 0;
		}
		bool Zlib::GZCompress2(const std::string& data, std::string& compressedData, int level /*= -1*/) {
			unsigned char out[CHUNK];
			z_stream strm;
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			if (g_zlib.m_gzipDeflateInit2(&strm, level, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, 0, _zlibver_, Z_DEFAULT_STRATEGY) != Z_OK)
			{
				return false;
			}
			strm.next_in = (unsigned char*)data.c_str();
			strm.avail_in = static_cast<unsigned int>(data.size());
			do {
				int have;
				strm.avail_out = CHUNK;
				strm.next_out = out;
				if (g_zlib.m_gzipDeflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
				{
					return false;
				}
				have = CHUNK - strm.avail_out;
				compressedData.append((char*)out, have);
			} while (strm.avail_out == 0);
			if (g_zlib.m_gzipDeflateEnd(&strm) != Z_OK)
			{
				return false;
			}
			return true;
		}
		bool Zlib::GZUnCompress2(const std::string& compressedData, std::string& data) {
			int ret;
			unsigned have;
			z_stream strm;
			unsigned char out[CHUNK];
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			strm.avail_in = 0;
			strm.next_in = Z_NULL;
			if (g_zlib.m_gzipInflateInit2(&strm, 16 + MAX_WBITS, _zlibver_, (int)sizeof(z_stream)) != Z_OK)
			{
				return false;
			}
			strm.avail_in = static_cast<unsigned int>(compressedData.size());
			strm.next_in = (unsigned char*)compressedData.c_str();
			do {
				strm.avail_out = CHUNK;
				strm.next_out = out;
				ret = g_zlib.m_gzipInflate(&strm, Z_NO_FLUSH);
				switch (ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					g_zlib.m_gzipInflateEnd(&strm);
					return false;
				}
				have = CHUNK - strm.avail_out;
				data.append((char*)out, have);
			} while (strm.avail_out == 0);

			if (g_zlib.m_gzipInflateEnd(&strm) != Z_OK) {
				return false;
			}
			return true;
		}
	}//compress
}//xslc