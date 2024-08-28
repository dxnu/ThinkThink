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

	// ��ȡ��Ϣֵ
	std::string GetValue(const char* key) const;

	// ��ȡ��Ϣ����
	std::string GetType() const;

	// ��ȡԭʼ��Ϣ�ı�
	std::string GetRawData() const;

private:
	std::shared_ptr<pugi::xml_document> m_doc;
};

