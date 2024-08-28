#include "ThinkThinkMessage.h"
#include <sstream>


ResponseMessage::ResponseMessage()
{
	m_doc.append_child("ThinkThink");
}

std::string ResponseMessage::GetData()
{
	std::stringstream ss;
	m_doc.save(ss, "");
	return ss.str();
}

void ResponseMessage::SetMessage(const char* key, const char* value)
{
	pugi::xml_node root = m_doc.child("ThinkThink");
	root.append_child(key).text().set(value);
}

RequestMessage::RequestMessage(const std::string& data)
{
	m_doc = std::make_shared<pugi::xml_document>();
	m_doc->load_string(data.c_str());
}

std::string RequestMessage::GetValue(const char* key) const
{
	pugi::xml_node node = m_doc->child("ThinkThink");
	return node.child(key).text().as_string();
}

std::string RequestMessage::GetType() const
{
	pugi::xml_node node = m_doc->child("ThinkThink");
	return node.child("RequestType").text().as_string();
}

std::string RequestMessage::GetRawData() const
{
	std::stringstream ss;
	m_doc->save(ss, " ");
	return ss.str();
}