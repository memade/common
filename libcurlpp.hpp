#if !defined(AFX_SKSTU_libcurlpp_H__D748C_B0360946_9DC35701537B2E3_1434__HEAD__)
#define AFX_SKSTU_libcurlpp_H__D748C_B0360946_9DC35701537B2E3_1434__HEAD__


#include "../Common/libcurl.hpp"
#include "../common/zlib.h"

#if defined _M_IX86
#if defined(_DEBUG)
#pragma comment(lib,"../lib/Win32/Debug/libCurlpp.lib")
#else
#pragma comment(lib,"../lib/Win32/Release/libCurlpp.lib")
#endif
#elif defined _M_X64
#if defined(_DEBUG)
#pragma comment(lib,"../lib/x64/Debug/libCurlpp.lib")
#else
#pragma comment(lib,"../lib/x64/Release/libCurlpp.lib")
#endif
#else
#endif


#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
using namespace curlpp::options;

namespace sk {
	class HttpClient final {
	public:
		HttpClient() {

		}
		~HttpClient() {

		}
	public:
		static int HttpGet(const sk::stringa& url, sk::stringa& response_data) {
			try
			{
				bool IsGzip = false;
				curlpp::Cleanup cleaner;
				curlpp::Easy request;

				sk::stringa res;
				using namespace std::placeholders;
				request.setOpt(new curlpp::options::WriteFunction(
					[&](char* ptr, size_t size, size_t nmemb)->size_t {
						res.append(ptr, size * nmemb);
						return size * nmemb;
					}));

				request.setOpt(new curlpp::options::Url(url));
				request.setOpt(new curlpp::options::Verbose(true));
				request.setOpt(new curlpp::options::HeaderFunction(
					[&IsGzip](char* buffer, size_t size, size_t items) -> size_t {
						if (strstr(buffer, "gzip") || strstr(buffer, "GZIP")) {
							IsGzip = true;
						}
						return size * items;
					}));
				request.setOpt(new Timeout(5));
				request.perform();

				if (IsGzip) {
					sk::compress::Zlib::GZipUnCompress(res, response_data);
				}
				else {
					res = response_data;
				}
			}
			catch (curlpp::LogicError& e)
			{
				std::cout << e.what() << std::endl;
				return -1;
			}
			catch (curlpp::RuntimeError& e)
			{
				std::cout << e.what() << std::endl;
				return -1;
			}
			return 0;
		}

		static int HttpPut(const sk::stringa& url, const sk::stringa& post_submit_data, sk::stringa& response_data) {
			try {
				std::list< std::string > headers;
				sk::stringa submit_data = post_submit_data;
				sk::stringa buffer;
				if (submit_data.length() >= 512) {
					auto gzip_buffer =sk::compress::Zlib::GZipCompress(submit_data.data(), (unsigned long)submit_data.length());
					submit_data.clear();
					if (gzip_buffer) {
						headers.push_back("Accept-Encoding: gzip,deflate");
						submit_data.insert(0, std::get<0>(*gzip_buffer).get(), std::get<1>(*gzip_buffer));
					}
					else {
						submit_data = post_submit_data;
					}
				}

				std::istringstream myStream(submit_data);
				int size = (int)myStream.str().size();
				curlpp::Cleanup cleaner;
				curlpp::Easy request;
				
				headers.push_back("Content-Type: application/octet-stream");
				headers.push_back(sk::stringa("Content-Length: ").append(tostringa(size)));

				using namespace curlpp::Options;
				request.setOpt(new Verbose(true));
				request.setOpt(new ReadStream(&myStream));
				request.setOpt(new InfileSize(size));
				request.setOpt(new Upload(true));
				request.setOpt(new HttpHeader(headers));
				request.setOpt(new Url(url));
				bool IsGzip = false;
				request.setOpt(new curlpp::options::HeaderFunction(
					[&IsGzip](char* buffer, size_t size, size_t items) -> size_t {
						if (strstr(buffer, "gzip") || strstr(buffer, "GZIP")) {
							IsGzip = true;
						}
						return size * items;
					}));

				std::ostringstream os;
				curlpp::options::WriteStream ws(&os);
				request.setOpt(ws);
				request.perform();

				auto src = os.str();
				if (IsGzip) {
					sk::compress::Zlib::GZipUnCompress(src, response_data);
				}
				else {
					response_data = src;
				}
				auto sk = 0;
			}
			catch (curlpp::LogicError& e) {
				std::cout << e.what() << std::endl;
				return -1;
			}
			catch (curlpp::RuntimeError& e) {
				std::cout << e.what() << std::endl;
				return -1;
			}
			return 0;
		}

		static int HttpPost(const sk::stringa& url, const sk::stringa& post_submit_data, sk::stringa& response_data) {
			try {
				curlpp::Cleanup cleaner;
				curlpp::Easy request;
				request.setOpt(new curlpp::options::Url(url));
				request.setOpt(new curlpp::options::Verbose(true));
				std::list<std::string> header;
				header.push_back("Content-Type: application/octet-stream");
				request.setOpt(new curlpp::options::HttpHeader(header));
				request.setOpt(new curlpp::options::PostFields(post_submit_data.data()));
				request.setOpt(new curlpp::options::PostFieldSize(static_cast<long>(post_submit_data.length())));

				std::ostringstream os;
				curlpp::options::WriteStream ws(&os);
				request.setOpt(ws);
				request.perform();
				response_data = os.str();
			}
			catch (curlpp::LogicError& e) {
				std::cout << e.what() << std::endl;
				return -1;
			}
			catch (curlpp::RuntimeError& e) {
				std::cout << e.what() << std::endl;
				return -1;
			}
			return 0;
		}
	};
}///namespace sk

/// /*新生联创（上海）**/
/// /*2019年05月29日 13:12:37|968**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_libcurlpp_H__D748C_B0360946_9DC35701537B2E3_1434__TAIL__


