#include "stdafx.h"
#include "Json.h"

namespace obj
{

void from_json(const Json &j, ValuePtr &v)
{
	if (j.is_boolean())
		v = BooleanValue::make(j.get<bool>());
	else if (j.is_number_integer())
		v = Int32Value::make(j.get<int32_t>());
	else if (j.is_string())
		v = StringValue::make(j.get<String>());
	else if (j.is_number_float())
		v = FloatValue::make(j.get<float>());
	else if (j.is_object())
	{
		ObjectPtr o = j;
		v = ObjectValue::make(o);
	}
}

void from_json(const Json &j, PropertyPtr &p)
{
	auto _p = make_shared<Property>();

	if (j.is_array())
	{
		std::vector<ValuePtr> v;
		for (Json::const_iterator i = j.begin(); i != j.end(); ++i)
			v.push_back(*i);

		_p->value() = VectorValue::make(v);
	}
	else
	{
		_p->value() = j;
	}
	p = _p;
}

void from_json(const Json &j, ObjectPtr &o)
{
	o = Object::make();
	for (obj::Json::const_iterator i = j.begin(); i != j.end(); ++i)
	{
		PropertyPtr p = *i;
		p->name() = i.key();
		o->properties().push_back(p);
	}
}

void to_json(Json &j, const ValuePtr &p)
{
	using boost::uuids::to_string;
	if (auto v = std::dynamic_pointer_cast<StringValue>(p))
		j = v->value();
	else if (auto v = std::dynamic_pointer_cast<BooleanValue>(p))
		j = v->value();
	else if (auto v = std::dynamic_pointer_cast<Int32Value>(p))
		j = v->value();
	else if (auto v = std::dynamic_pointer_cast<FloatValue>(p))
		j = v->value();
	else if (auto v = std::dynamic_pointer_cast<UuIdValue>(p))
		j = to_string(v->value());
	else if (auto v = std::dynamic_pointer_cast<ObjectValue>(p))
		j = v->value();
	else if (auto v = std::dynamic_pointer_cast<VectorValue>(p))
		std::copy(v->value().begin(), v->value().end(), std::back_inserter(j));
}

void to_json(Json &j, const PropertyPtr &p)
{
	Json jv = p->value();
	j[p->name()] = jv;
}

void to_json(Json &j, const ObjectPtr &o)
{
	//if (o->id() != GenerateNullId())
	//{
	//	using boost::uuids::to_string;
	//	String id = to_string(o->id());
	//	j["id"] = id;
	//}
	//std::transform(o->properties().begin(), o->properties().end(), std::inserter(j, j.end()), [](PropertyPtr p) {return std::make_pair(p->name(), Json(p->value())); });
	for (PropertyPtr p : o->properties())
	{
		Json jv = p->value();
		j[p->name()] = jv;
	}
}

}//obj

obj::ObjectPtr obj::js::readJson(const Json& j)
{
	 ObjectPtr object = j;
	 return object;
}

obj::ObjectPtr obj::js::readFile(const Path &filePath)
{
	 boost::filesystem::ifstream is(filePath);
	 Json j;
	 is >> j;
	 ObjectPtr object = j;
	 return object;
}

obj::ObjectPtr obj::js::readString(const String &jsonString)
{
	 istringstream is(jsonString);
	 Json j;
	 is >> j;
	 ObjectPtr object = j;
	 return object;
}

obj::Json obj::js::writeJson(ObjectPtr object)
{
	Json j = object;
	return j;
}

void obj::js::writeFile(const Path &filePath, ObjectPtr object)
{
	 boost::filesystem::ofstream os(filePath);
	 Json j = object;
	 os << j;
}

obj::String obj::js::writeString(ObjectPtr object)
{
	 ostringstream os;
	 Json j = object;
	 os << j;
	 return os.str();
}
