#if !defined(AFX_SKSTU_CURLPP_H__D4E2C_68BCD900_47E2F5F6F1DF441_08F1__HEAD__)
#define AFX_SKSTU_CURLPP_H__D4E2C_68BCD900_47E2F5F6F1DF441_08F1__HEAD__

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

namespace sk {
	class HttpClient final {
	public:
		HttpClient() {

		}
		~HttpClient() {

		}
	public:
		static size_t WriteMemoryCallback(char* ptr, size_t size, size_t nmemb)
		{
			// Calculate the real size of the incoming buffer
			size_t realsize = size * nmemb;

			//// (Re)Allocate memory for the buffer
			//m_pBuffer = (char*)Realloc(m_pBuffer, m_Size + realsize);

			//// Test if Buffer is initialized correctly & copy memory
			//if (m_pBuffer == NULL) {
			//	realsize = 0;
			//}

			//memcpy(&(m_pBuffer[m_Size]), ptr, realsize);
			//m_Size += realsize;

			// return the real size of the buffer...
			return realsize;
		};
		static int HttpGet(const sk::stringa& url, sk::stringa& response_data) {

			try
			{
				curlpp::Cleanup cleaner;
				curlpp::Easy request;

				curlpp::types::WriteFunctionFunctor functor(WriteMemoryCallback);
				curlpp::options::WriteFunction* test = new curlpp::options::WriteFunction(functor);
				request.setOpt(test);

				// Setting the URL to retrive.
				request.setOpt(new curlpp::options::Url(url));
				request.setOpt(new curlpp::options::Verbose(true));
				//request.setOpt(new Timeout(3));
				request.perform();
				auto sk = 0;
			}
			catch (curlpp::LogicError& e)
			{
				std::cout << e.what() << std::endl;
			}
			catch (curlpp::RuntimeError& e)
			{
				std::cout << e.what() << std::endl;
			}

			return 0;
		}
		static int HttpPost(const sk::stringa& url, const sk::stringa& post_submit_data, sk::stringa& response_data) {
			return 0;
		}

	};
}///namespace sk


/// /*新生联创（上海）**/
/// /*2019年08月10日 21:38:54|165**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CURLPP_H__D4E2C_68BCD900_47E2F5F6F1DF441_08F1__TAIL__