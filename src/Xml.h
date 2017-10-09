#pragma once

#include "Object.h"
#include "xml/rapidxml.hpp"

namespace obj
{
using XmlDocument = rapidxml::xml_document<String::value_type>;
using XmlDocumentPtr = shared_ptr<XmlDocument>;
using XmlBase = rapidxml::xml_base<String::value_type>;
using XmlNode = rapidxml::xml_node<String::value_type>;
using XmlAttribute = rapidxml::xml_attribute<String::value_type>;

namespace xml
{
ObjectPtr readXml(const XmlDocumentPtr doc);
ObjectPtr readXml(const XmlDocument& doc);
ObjectPtr readFile(const Path& filePath);
ObjectPtr readString(const String& xmlString);
XmlDocumentPtr writeXml(ObjectPtr object);
void writeFile(const Path& filePath, ObjectPtr object);
String writeString(ObjectPtr object);
}//xml
}//obj
