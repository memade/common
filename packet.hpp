#if !defined(AFX_SKSTU_packet_H__FE4AB_D5E85A09_412DB504D138CC2_F887__HEAD__)
#define AFX_SKSTU_packet_H__FE4AB_D5E85A09_412DB504D138CC2_F887__HEAD__

#include <map>
#include <vector>
#include <string>

#include "gpl/rapidjson/reader.h"
#include "gpl/rapidjson/document.h"
#include "gpl/rapidjson/rapidjson.h"
#include "gpl/rapidjson/stringbuffer.h"
#include "gpl/rapidjson/writer.h"
#include "gpl/rapidjson/stream.h"
using namespace rapidjson;

#include "gpl/rapidxml/rapidxml.hpp"
#include "gpl/rapidxml/rapidxml_iterators.hpp"
#include "gpl/rapidxml/rapidxml_print.hpp"
#include "gpl/rapidxml/rapidxml_utils.hpp"
using namespace rapidxml;

#include "../common/network_interface.h"

namespace sk {
	namespace json {
		typedef std::map<std::string, std::string> DefJsonNode;
		typedef std::vector<DefJsonNode> DefJsonComments;
		typedef std::vector<DefJsonNode> DefJsongNodes;
		typedef std::map<size_t, DefJsonNode> DefJsonValues;
		typedef std::map<size_t, DefJsongNodes> DefJsonArrays;
	}///namespace json

#pragma pack(push,1)
	struct tagPacket {
		char* data;
		unsigned long len;
		tagPacket() { memset(this, 0x00, sizeof(*this)); }
		tagPacket(const char* _data, const unsigned long& _len) {
			if (_data && _len) {
				len = _len;
				data = new char[_len];
				memcpy(data, _data, _len);
			}
		}
	};
#pragma pack(pop)

	class Packet final {
	public:
		Packet() {}
		~Packet() = delete;
	public:
		static std::string JsonLog(const std::map<std::string, std::string>& Keys, std::map<std::string, std::string> Vals = {}, const char* ValsName = nullptr) {
			rapidjson::Document JsonDoc;
			JsonDoc.SetObject();
			rapidjson::Document::AllocatorType& JsonAllocator = JsonDoc.GetAllocator();

			for (const auto& node_key : Keys) {
				JsonDoc.AddMember(
					rapidjson::Value().SetString(node_key.first.c_str(), JsonAllocator).Move(),
					rapidjson::Value().SetString(node_key.second.c_str(), JsonAllocator).Move(),
					JsonAllocator);
			}///for (const auto& node_comment : comments)

			rapidjson::Value JsonObject(rapidjson::kObjectType);
			for (const auto& node_val : Vals) {
				JsonObject.AddMember(
					rapidjson::Value().SetString(node_val.first.c_str(), JsonAllocator).Move(),
					rapidjson::Value().SetString(node_val.second.c_str(), JsonAllocator).Move(),
					JsonAllocator);
			}
			if (JsonObject.MemberCount() > 0) {
				if (ValsName) {
					JsonDoc.AddMember(
						rapidjson::Value().SetString(ValsName, JsonAllocator).Move(),
						JsonObject,
						JsonAllocator);
				}
				else {
					JsonDoc.AddMember(
						rapidjson::Value().SetString("values", JsonAllocator).Move(),
						JsonObject,
						JsonAllocator);
				}
			}


			rapidjson::StringBuffer JsonBuffer;
			if (JsonDoc.IsObject()) {
				rapidjson::Writer<StringBuffer> JsonWriter(JsonBuffer);
				JsonDoc.Accept(JsonWriter);
			}
			return JsonBuffer.GetString();
		}
		static std::string Json(
			const json::DefJsonComments& _KeyComments,
			const char* _KeyMain = nullptr,
			json::DefJsonValues _Values = {},
			const char* _KeyData = nullptr) {
			rapidjson::Document JsonDoc;
			JsonDoc.SetObject();
			rapidjson::Document::AllocatorType& JsonAllocator = JsonDoc.GetAllocator();

			if (auto nNode = _KeyComments.size()) {
				std::vector<std::shared_ptr<rapidjson::Value>> JsonArrays;

				for (auto it = _KeyComments.begin(), begin = _KeyComments.begin(); it != _KeyComments.end(); ++it) {
					auto Index = it - begin;
					auto pJsonArray = std::make_shared<rapidjson::Value>(rapidjson::kArrayType);
					rapidjson::Value JsonObject(rapidjson::kObjectType);
					JsonObject.SetObject();
					for (const auto& node_node : *it) {
						JsonObject.AddMember(
							rapidjson::Value().SetString(node_node.first.c_str(), JsonAllocator).Move(),
							rapidjson::Value().SetString(node_node.second.c_str(), JsonAllocator).Move(),
							JsonAllocator);
					}///for (const auto& node_node : node)
					auto nDataCountTotal = _Values.size();
					if (nDataCountTotal > 0) {
						auto find = _Values.find((int)Index);
						if (find != _Values.end()) {
							rapidjson::Value JsonValueArray(rapidjson::kArrayType);
							for (auto ritNode = find->second.rbegin(); ritNode != find->second.rend(); ++ritNode) {
								rapidjson::Value JsonValueObject(rapidjson::kObjectType);
								JsonValueObject.SetObject();
								JsonValueObject.AddMember(
									rapidjson::Value().SetString(ritNode->first.c_str(), JsonAllocator).Move(),
									rapidjson::Value().SetString(ritNode->second.c_str(), JsonAllocator).Move(),
									JsonAllocator);
								JsonValueArray.PushBack(JsonValueObject, JsonAllocator);
							}
							if (_KeyData) {
								JsonObject.AddMember(
									rapidjson::Value().SetString(_KeyData, JsonAllocator).Move(), JsonValueArray, JsonAllocator);
							}
							else {
								JsonObject.AddMember(
									rapidjson::Value().SetString("", JsonAllocator).Move(), JsonValueArray, JsonAllocator);
							}
						}
					}

					pJsonArray->PushBack(JsonObject, JsonAllocator);
					JsonArrays.push_back(pJsonArray);
				}
				if (auto nArray = JsonArrays.size()) {
					for (decltype(nArray) i = 1; i < nArray; ++i) {
						JsonArrays[nArray - (i + 1)]->PushBack(*JsonArrays[nArray - i], JsonAllocator);
					}
				}
				if (_KeyMain) {
					JsonDoc.AddMember(rapidjson::Value().SetString(_KeyMain, JsonAllocator).Move(), *JsonArrays[0], JsonAllocator);
				}
				else {
					JsonDoc.AddMember(rapidjson::Value().SetString("", JsonAllocator).Move(), *JsonArrays[0], JsonAllocator);
				}
			}///if

			rapidjson::StringBuffer JsonBuffer;
			if (JsonDoc.IsObject()) {
				rapidjson::Writer<StringBuffer> JsonWriter(JsonBuffer);
				JsonDoc.Accept(JsonWriter);
			}
			return JsonBuffer.GetString();
		}
		static std::string Json(
			const std::function<void(const size_t&, json::DefJsongNodes&)> & cb_insert_values,
			const json::DefJsonComments & _KeyComments,
			const char* _KeyMain = nullptr,
			const char* _KeyData = nullptr) {

			rapidjson::Document JsonDoc;
			JsonDoc.SetObject();
			rapidjson::Document::AllocatorType& JsonAllocator = JsonDoc.GetAllocator();

			if (auto nNode = _KeyComments.size()) {
				json::DefJsonArrays _DefArrays = {};
				std::vector<std::shared_ptr<rapidjson::Value>> JsonArrays;
				for (auto it = _KeyComments.begin(), begin = _KeyComments.begin(); it != _KeyComments.end(); ++it) {
					auto Index = it - begin;
					cb_insert_values(Index, _DefArrays[Index]);

					auto pJsonArray = std::make_shared<rapidjson::Value>(rapidjson::kArrayType);
					rapidjson::Value JsonObject(rapidjson::kObjectType);
					JsonObject.SetObject();
					for (const auto& node_node : *it) {
						JsonObject.AddMember(
							rapidjson::Value().SetString(node_node.first.c_str(), JsonAllocator).Move(),
							rapidjson::Value().SetString(node_node.second.c_str(), JsonAllocator).Move(), JsonAllocator);
					}///for (const auto& node_node : node)

					auto _Nodes = _DefArrays[Index];
					auto _nNodes = _Nodes.size();
					if (_nNodes > 0) {
						rapidjson::Value JsonValueArray(rapidjson::kArrayType);
						for (auto ritNode = _Nodes.rbegin(); ritNode != _Nodes.rend(); ++ritNode) {
							rapidjson::Value JsonValueObject(rapidjson::kObjectType);
							JsonValueObject.SetObject();
							for (auto __node : *ritNode) {							
								JsonValueObject.AddMember(
									rapidjson::Value().SetString(__node.first.c_str(), JsonAllocator).Move(),
									rapidjson::Value().SetString(__node.second.c_str(), JsonAllocator).Move(),
									JsonAllocator);						
							}
							JsonValueArray.PushBack(JsonValueObject, JsonAllocator);
						}
						if (_KeyData) {
							JsonObject.AddMember(rapidjson::Value().SetString(_KeyData, JsonAllocator).Move(), JsonValueArray, JsonAllocator);
						}
						else {
							JsonObject.AddMember(rapidjson::Value().SetString("", JsonAllocator).Move(), JsonValueArray, JsonAllocator);
						}
					}

					pJsonArray->PushBack(JsonObject, JsonAllocator);
					JsonArrays.push_back(pJsonArray);
				}
				if (auto nArray = JsonArrays.size()) {
					for (decltype(nArray) i = 1; i < nArray; ++i) {
						JsonArrays[nArray - (i + 1)]->PushBack(*JsonArrays[nArray - i], JsonAllocator);
					}
				}
				if (_KeyMain) {

					JsonDoc.AddMember(rapidjson::Value().SetString(_KeyMain, JsonAllocator).Move(), *JsonArrays[0], JsonAllocator);
				}
				else {
					JsonDoc.AddMember(rapidjson::Value().SetString("", JsonAllocator).Move(), *JsonArrays[0], JsonAllocator);
				}
			}///if

			rapidjson::StringBuffer JsonBuffer;
			if (JsonDoc.IsObject()) {
				rapidjson::Writer<StringBuffer> JsonWriter(JsonBuffer);
				JsonDoc.Accept(JsonWriter);
			}
			return JsonBuffer.GetString();
		}
		/*
		*
		*
		*
		*/
		static std::string UnJson() {
			return "";
		}

		//!@ auto pack = JsonHttpHeadMake(std::map<std::string, std::string>{ {"a", "bb"}, {"c","cc"} });
		static std::string MakeHttpJson(const std::map<std::string,std::string>& _InMap) {
			if (!_InMap.empty()) {
				rapidjson::Document JsonDoc;
				JsonDoc.SetObject();
				rapidjson::Document::AllocatorType& jsonAllocator = JsonDoc.GetAllocator();

				for (const auto& node : _InMap) {
					JsonDoc.AddMember(
						rapidjson::Value().SetString(node.first.c_str(), jsonAllocator).Move(),
						rapidjson::Value().SetString(node.second.c_str(), jsonAllocator).Move(),
						jsonAllocator);
				}

				rapidjson::StringBuffer JsonBuffer;
				if (JsonDoc.IsObject()) {
					rapidjson::Writer<StringBuffer> JsonWriter(JsonBuffer);
					JsonDoc.Accept(JsonWriter);
				}
				return JsonBuffer.GetString();
			}
			return "";
		}
		static int UnMakeHttpJson(const std::string& _InData,std::map<std::string, std::string>& _OutMap) {
			rapidjson::Document JsonDoc;
			if (JsonDoc.Parse(_InData.c_str()).HasParseError()) {
				return -1;
			}
			if (JsonDoc.MemberCount()) {
				_OutMap.clear();
				for (auto it = JsonDoc.MemberBegin(); it != JsonDoc.MemberEnd(); ++it) {
					if (!it->name.IsNull() && it->name.IsString()) {
						if (it->value.IsString()) {
							_OutMap.insert({ {it->name.GetString(),it->value.GetString()} });
						}
						else {
							_OutMap.insert({ {it->name.GetString(),""} });
						}
					}
				}
			}
			return 0;
		}










		//! @make bin packet support compress
		static std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>>
			network_packet(
				sk::int64 _Command,
				std::function<std::tuple<std::shared_ptr<char>, unsigned long>(const char*, unsigned long)> _cbCompress = nullptr,
				const char* _pData = nullptr, unsigned long _Length = 0, bool _IsHead = true) {
			std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> val;
			if (_IsHead) {
				if (!_pData || !_Length || !_cbCompress) {//!@ 只有包头
					SK_SHARED_PTR(char, packet, network::LENHEADBIN);
					auto pBegin = packet.get();
					memcpy(pBegin, &network::HEADBIN(), network::LENHEADBIN);
					auto pHead = reinterpret_cast<network::HEADBIN*>(pBegin);
					pHead->command = _Command;
					pHead->length_total = network::LENHEADBIN;
					pHead->length_raw = 0;
					pHead->length = 0;
					pHead->data[0] = '!';
					val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(packet, network::LENHEADBIN));
					return val;
				}
			}
			if (512 > _Length) {//! 不用压缩		
				if (_IsHead) {
					const unsigned long newLength = network::LENHEADBIN + _Length - sizeof(char);
					SK_SHARED_PTR(char, pak, newLength);
					auto pBegin = pak.get();
					memcpy(pBegin, &network::HEADBIN(), network::LENHEADBIN);
					auto pHead = reinterpret_cast<network::HEADBIN*>(pBegin);
					pHead->command = _Command;
					pHead->length_total = newLength;
					pHead->length_raw = _Length;
					memcpy(pHead->data, _pData, _Length);
					val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(pak, newLength));
				}
				else {
					SK_SHARED_PTR(char, pak, _Length);
					memcpy(pak.get(), _pData, _Length);
					val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(pak, _Length));
				}
			}
			else {//! 压缩
				if (_IsHead) {
					auto compress_pak = _cbCompress(_pData, _Length);
					auto newPak = std::get<0>(compress_pak);
					auto newPakLen = std::get<1>(compress_pak);
					const unsigned long newLength = network::LENHEADBIN + newPakLen - sizeof(char);
					SK_SHARED_PTR(char, pak, newLength);
					auto pBegin = pak.get();
					memcpy(pBegin, &network::HEADBIN(), network::LENHEADBIN);
					auto pHead = reinterpret_cast<network::HEADBIN*>(pBegin);
					pHead->command = _Command;
					pHead->type_wrapped = sk::network::HEADBIN::EnTypeDataWrapped::TYPE_DATA_WRAPPED_COMPRESS;
					pHead->type_compress = sk::network::HEADBIN::EnTypeCompress::TYPE_DATA_COMPRESS_ZIP;
					pHead->length_total = newLength;
					pHead->length_raw = _Length;
					pHead->length = newLength;
					pHead->length_compress = newPakLen;
					memcpy(pHead->data, newPak.get(), newPakLen);
					val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(pak, newLength));
				}
				else {
					auto compress_pak = _cbCompress(_pData, _Length);
					auto newPak = std::get<0>(compress_pak);
					auto newPakLen = std::get<1>(compress_pak);
					val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(newPak, newPakLen));
				}
			}
			return val;
		}
		static std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>>
			network_unpacket(
				const char* _pPacket, /*完整的封包包含包头*/
				unsigned long _nPacketLen, /*完整的封包长度*/
				std::function<std::tuple<std::shared_ptr<char>, unsigned long>(const char*, unsigned long, unsigned long)> _cbUnCompress, bool _IsHead = true) {
			std::shared_ptr<std::tuple<std::shared_ptr<char>, unsigned long>> val;
			auto pHead = reinterpret_cast<network::HEADBIN*>(const_cast<char*>(_pPacket));
			if (pHead->length_compress) {//! 压缩的封包处理
				val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(_cbUnCompress(pHead->data, pHead->length_compress, pHead->length_raw));
			}
			else {//! 非压缩包处理
				SK_SHARED_PTR(char, pak, pHead->length_raw);
				memcpy(pak.get(), pHead->data, pHead->length_raw);
				val = std::make_shared<std::tuple<std::shared_ptr<char>, unsigned long>>(std::make_tuple(pak, pHead->length_raw));
			}
			return val;
		}




	};
}///namespace xslc





/// /*新生联创（上海）**/
/// /*2019年04月12日 8:26:03|360**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_packet_H__FE4AB_D5E85A09_412DB504D138CC2_F887__TAIL__