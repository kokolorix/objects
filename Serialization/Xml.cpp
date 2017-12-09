#include "stdafx.h"
#include "Json.h"
#include "Xml.h"
#include <xml/rapidxml_print.hpp>

using namespace obj;
using namespace rapidxml;

String obj::xml::ELEMENT_NAME = "#element_name#";
String obj::xml::ROOT_NAME = "root";
String obj::xml::TEXT_NODE = "#text_node#";

namespace obj
{
bool isDataNode(XmlNode* node)
{
	XmlAttribute *fa = node->first_attribute(), *la = node->last_attribute();
	if (fa)
		return false;
	XmlNode* fn = node->first_node(), *ln = node->last_node();
	if (fn && fn == ln && fn->type() == node_data)
		return true;
	return false;
}
ObjectPtr objectFromXml(XmlNode* node);
ValuePtr valueFromXml(XmlNode* node)
{
	if (isDataNode(node))
		return Value::parse(node->value());
	ObjectPtr o = objectFromXml(node);
	if (o->properties().empty())
		return Value::make();
	if (o->properties().size() == 1)
		if (auto v = dynamic_pointer_cast<VectorValue>(o->properties().front()->value()))
			return v;
	return Value::make(o);
}
ValuePtr valueFromXml(XmlAttribute* attribute)
{
	return Value::parse(attribute->value());
}
PropertyPtr propertyFromXml(XmlNode* node)
{
	return PropertyPtr();
}
PropertyPtr propertyFromXml(XmlAttribute* attribute)
{
	return Property::make(attribute->name(), valueFromXml(attribute));
}
ObjectPtr objectFromXml(XmlNode* node)
{
	ObjectPtr obj = Object::make();

	for (XmlAttribute*a = node->first_attribute(); a; a = a->next_attribute())
	{
		static const String n("null"), t("true");
		if (a->name() == n && a->value() == t)
			return Object::make();

		PropertyPtr p = propertyFromXml(a);
		obj->properties().push_back(p);
	}
	for (XmlNode* n = node->first_node(); n; n = n->next_sibling())
	{
		PropertyPtr p = obj->property(n->name());
		if (auto v = dynamic_pointer_cast<VectorValue>(p ? p->value() : ValuePtr()))
		{
			ValuePtr value = valueFromXml(n);
			const_cast<ValuePtrVector&>(v->value()).push_back(value);
		}
		else if (n->next_sibling(n->name()))
		{
			auto v = VectorValue::make(ValuePtrVector());
			ValuePtr value = valueFromXml(n);
			v->value().push_back(value);
			PropertyPtr p = Property::make(n->name(), v);
			obj->properties().push_back(p);
		}
		else
		{
			ObjectPtr o = objectFromXml(n);
			if (o->properties().empty())
				obj->properties().push_back(Property::make(n->name(), Value::make()));
			else
				obj->properties().push_back(Property::make(n->name(), o));
		}
	}
	auto& p = obj->properties();
	std::sort(p.begin(), p.end(), [](PropertyPtr x, PropertyPtr y) {return x->name() < y->name(); });
	return obj;
}
XmlNode* toXml(XmlNode* parent, ObjectPtr object);
XmlNode* toXml(XmlNode* parent, ValuePtr value, const String& name = "")
{
	XmlDocument* doc = parent->document();
	if (auto v = std::dynamic_pointer_cast<ObjectValue>(value))
	{
		if (name.empty())
		{
			return toXml(parent, v->value());
		}
		else
		{
			XmlNode* objectNode = doc->allocate_node(node_element, doc->allocate_string(name.c_str()));
			parent->append_node(objectNode);
			return toXml(objectNode, v->value());
		}
	}
	else if (auto v = std::dynamic_pointer_cast<VectorValue>(value))
	{
		for (ValuePtr element : v->value())
		{
			String::value_type* str = name.empty() ? parent->name() : doc->allocate_string(name.c_str());
			XmlNode* elementNode = doc->allocate_node(node_element, str);
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
	if (property->name() == xml::TEXT_NODE)
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
	if (object->properties().empty())
	{
		XmlDocument* doc = parent->document();
		XmlAttribute* attribute = doc->allocate_attribute("null", "true");
		parent->append_attribute(attribute);
		return parent;
	}

	for (PropertyPtr p : object->properties())
	{
		if (p->name() == xml::ELEMENT_NAME)
			continue;
		XmlNode* propertyNode = toXml(parent, p);
		if (propertyNode != parent)
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
	ObjectPtr obj = objectFromXml(doc.first_node());
	return obj;
}

ObjectPtr obj::xml::readString(String & xmlString)
{
	XmlDocumentPtr doc = make_shared<XmlDocument>();
	doc->parse<0>(const_cast<String::value_type*>(xmlString.c_str()));
	ObjectPtr obj = readXml(doc);
	return obj;
}

ObjectPtr obj::xml::readFile(const Path & filePath)
{
	std::ostringstream os;
	fs::ifstream fs(filePath);
	String line;
	while (std::getline(fs, line))
		os << line << std::endl;
	String xmlString = os.str();
	return readString(xmlString);
}

XmlDocumentPtr obj::xml::writeXml(ObjectPtr object, const String& rootName_ /*= ""*/)
{
	XmlDocumentPtr doc = make_shared<XmlDocument>();
	String rootName = rootName_.empty() ? object->value(ELEMENT_NAME, ROOT_NAME) : rootName_;
	XmlNode* root = doc->allocate_node(node_element, doc->allocate_string(rootName.c_str()));
	doc->append_node(root);
	XmlNode* obj = toXml(root, object);
	return doc;
}

void obj::xml::writeFile(const Path & filePath, ObjectPtr object)
{
	XmlDocumentPtr doc = writeXml(object);
	fs::ofstream os(filePath);
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
