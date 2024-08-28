#pragma once
#include "pugixml.hpp"
#include <string>



struct ResponseMessage {
	ResponseMessage();

	void SetMessage(const char* key, const char* value);

	std::string GetData();

private:
	pugi::xml_document m_doc;
};


struct RequestMessage {
	RequestMessage(const std::string& data);

	// 获取消息值
	std::string GetValue(const char* key) const;

	// 获取消息类型
	std::string GetType() const;

	// 获取原始消息文本
	std::string GetRawData() const;

private:
	std::shared_ptr<pugi::xml_document> m_doc;
};

