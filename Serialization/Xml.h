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
extern String ELEMENT_NAME;
extern String ROOT_NAME;
extern String TEXT_NODE;
ObjectPtr readXml(const XmlDocumentPtr doc);
ObjectPtr readXml(const XmlDocument& doc);
ObjectPtr readFile(const Path& filePath);
ObjectPtr readString(String& xmlString);
XmlDocumentPtr writeXml(ObjectPtr object, const String& rootName = "");
void writeFile(const Path& filePath, ObjectPtr object);
String writeString(ObjectPtr object);
}//xml
}//obj
