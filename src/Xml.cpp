#include "stdafx.h"
#include "Json.h"
#include "Xml.h"
#include <xml/rapidxml_print.hpp>

using namespace obj;
using namespace rapidxml;

namespace
{
String ELEMENT_NAME = "#element_name#";
String ROOT_NAME = "root";
String TEXT_NODE = "#text_node#";
}

namespace obj
{
	ValuePtr valueFromXml(XmlNode* node)
	{
		return ValuePtr();
	}
	ValuePtr valuefromXml(XmlAttribute* attribute)
	{
		return ValuePtr();
	}
	PropertyPtr propertyFromXml(XmlNode* node)
	{
		return PropertyPtr();
	}
	PropertyPtr propertyFromXml(XmlAttribute* attribute)
	{
		return PropertyPtr();
	}
	ObjectPtr objectFromXml(XmlNode* node)
	{
		return ObjectPtr();
	}
	XmlNode* toXml(XmlNode* parent, ObjectPtr object);
	XmlNode* toXml(XmlNode* parent, ValuePtr value, const String& name = "")
	{
		XmlDocument* doc = parent->document();
		if (auto v = std::dynamic_pointer_cast<ObjectValue>(value))
		{
			XmlNode* objectNode = doc->allocate_node(node_element, doc->allocate_string(name.c_str()));
			parent->append_node(objectNode);
			objectNode = toXml(objectNode, v->value());
			return objectNode;
		}
		else if (auto v = std::dynamic_pointer_cast<VectorValue>(value))
		{
			for (ValuePtr element : v->value())
			{
				XmlNode* elementNode = doc->allocate_node(node_element, doc->allocate_string(name.c_str()));
				parent->append_node(elementNode);
				XmlNode* valueNode = toXml(elementNode, element);
			}
			return parent;
		}
		else
		{
			String strValue = value;
			if (name.empty())
			{
				parent->value(parent->document()->allocate_string(strValue.c_str()));
				return parent;
			}
			else
			{
				if (dynamic_pointer_cast<const NothingValue<Unknown>>(value))
				{
					XmlNode* objectNode = doc->allocate_node(node_element, doc->allocate_string(name.c_str()));
					parent->append_node(objectNode);
					XmlAttribute* attribute = doc->allocate_attribute("null", "true");
					objectNode->append_attribute(attribute);
					return objectNode;
				}
				XmlAttribute* attribute = doc->allocate_attribute(
					doc->allocate_string(name.c_str()),
					doc->allocate_string(strValue.c_str()));
				parent->append_attribute(attribute);
				return parent;
			}
		}
	}
	XmlNode* toXml(XmlNode* parent, PropertyPtr property)
	{
		if (property->name() == TEXT_NODE)
		{
			XmlNode* valueNode = toXml(parent, property->value());
			return parent;
		}
		else
		{
			XmlNode* valueNode = toXml(parent, property->value(), property->name());
			return parent;
		}
	}
	XmlNode* toXml(XmlNode* parent, ObjectPtr object)
	{
		for (PropertyPtr p : object->properties())
		{
			if (p->name() == ELEMENT_NAME)
				continue;
			XmlNode* propertyNode = toXml(parent, p);
			if(propertyNode != parent)
				parent->append_node(propertyNode);
		}
		return parent;
	}


}//obj

ObjectPtr obj::xml::readXml(const XmlDocumentPtr doc)
{
	assert(doc);
	return readXml(*doc);
}

ObjectPtr obj::xml::readXml(const XmlDocument & doc)
{
	return ObjectPtr();
}

ObjectPtr obj::xml::readFile(const Path & filePath)
{
	return ObjectPtr();
}

ObjectPtr obj::xml::readString(const String & xmlString)
{
	return ObjectPtr();
}

XmlDocumentPtr obj::xml::writeXml(ObjectPtr object)
{
	XmlDocumentPtr doc = make_shared<XmlDocument>();
	String rootName = object->value(ELEMENT_NAME, ROOT_NAME);
	XmlNode* root = doc->allocate_node(node_element, doc->allocate_string(rootName.c_str()));
	doc->append_node(root);
	XmlNode* obj = toXml(root, object);
	return doc;
}

void obj::xml::writeFile(const Path & filePath, ObjectPtr object)
{
	XmlDocumentPtr doc = writeXml(object);
	boost::filesystem::ofstream os(filePath);
	os << "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>" << std::endl;
	os << *doc;
}

String obj::xml::writeString(ObjectPtr object)
{
	XmlDocumentPtr doc = writeXml(object);
	String str("<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\r\n");
	print(std::back_inserter(str), *doc, 0);
	return str;
}
