#include "RequestMessage.h"
#include "Utils.hpp"


RequestMessage::RequestMessage()
    : xml(&buffer)
{
    xml.writeStartDocument();
    xml.writeStartElement("ThinkThink");
}

void RequestMessage::setMessage(RequestBody type, const char* value)
{
    xml.writeTextElement(QtEnumToString(type), value);
}

void RequestMessage::commit()
{
    xml.writeEndElement();
    xml.writeEndDocument();
}

QByteArray RequestMessage::data()
{
    return buffer;
}
