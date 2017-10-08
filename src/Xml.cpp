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
	XmlNode* toXml(XmlNode* parent, ValuePtr value)
	{
		if (auto v = std::dynamic_pointer_cast<ObjectValue>(value))
		{
			XmlNode* objectNode = toXml(parent, v->value());
			return objectNode;
		}
		else if (auto v = std::dynamic_pointer_cast<VectorValue>(value))
		{
			XmlNode* elementNode = parent;
			for (ValuePtr element : v->value())
			{
				String strValue = element;
				if (!elementNode)
				{
					elementNode = parent->document()->allocate_node(node_element, parent->name());
					parent->append_node(elementNode);
				}
				elementNode->value(parent->document()->allocate_string(strValue.c_str()));
				elementNode = nullptr;
			}
			return parent;
		}
		else
		{
			String strValue = value;
			parent->value(parent->document()->allocate_string(strValue.c_str()));
			return parent;
		}
	}
	XmlNode* toXml(XmlNode* parent, PropertyPtr property)
	{
		if (property->name() == TEXT_NODE)
		{
			XmlNode* valueNode = toXml(parent, property->value());
			if (valueNode == parent)
				return valueNode;
			parent->append_node(valueNode);
			return parent;
		}
		else
		{
			XmlNode* propertyNode = parent->document()->allocate_node(node_element,
				parent->document()->allocate_string(property->name().c_str())
			);
			parent->append_node(propertyNode);
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
	XmlNode* obj = toXml(root, object);
	doc->append_node(root);
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
