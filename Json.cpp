#include "Json.h"

using json = nlohmann::json;

void from_json(const json &j, ValuePtr &v)
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

void from_json(const json &j, PropertyPtr &p)
{
    p = Property::make(String());

    if (j.is_array())
    {
        VectorValuePtr v = VectorValue::make(std::vector<ValuePtr>());

        for (json::const_iterator i = j.begin(); i != j.end(); ++i)
            v->value().push_back(*i);

        p->value() = v;
    }
    else
    {
        p->value() = j;
    }
}

void from_json(const json &j, ObjectPtr &o)
{
    o = Object::make();
    for (obj::json::const_iterator i = j.begin(); i != j.end(); ++i)
    {
        PropertyPtr p = *i;
        p->name() = i.key();
        o->properties().push_back(p);
    }
}

void to_json(json &j, const ValuePtr &p)
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

void to_json(json &j, const PropertyPtr &p)
{
	json jv = p->value();
	j[p->name()] = jv;
}

void to_json(json &j, const ObjectPtr &o)
{
	if (o->id() != GenerateNullId())
	{
		using boost::uuids::to_string;
		String id = to_string(o->id());
		j["id"] = id;
	}
	//std::transform(o->properties().begin(), o->properties().end(), std::inserter(j, j.end()), [](PropertyPtr p) {return std::make_pair(p->name(), json(p->value())); });
	for (PropertyPtr p : o->properties())
	{
		json jv = p->value();
		j[p->name()] = jv;
	}
}

ObjectPtr obj::serialize_json::ReadFromJson(const json& j)
{
    ObjectPtr object = j;
    return object;
}

ObjectPtr obj::serialize_json::ReadFromFile(const Path &filePath)
{
    boost::filesystem::ifstream is(filePath);
    json j;
    is >> j;
    ObjectPtr object = j;
    return object;
}

ObjectPtr obj::serialize_json::ReadFromString(const String &jsonString)
{
    std::istringstream is(jsonString);
    json j;
    is >> j;
    ObjectPtr object = j;
    return object;
}

json obj::serialize_json::WriteToJson(ObjectPtr object)
{
   json j = object;
	return j;
}

void obj::serialize_json::WriteToFile(const Path &filePath, ObjectPtr object)
{
    boost::filesystem::ofstream os(filePath);
    json j = object;
    os << j;
}

String obj::serialize_json::WriteToString(ObjectPtr object)
{
    std::ostringstream os;
    json j = object;
    os << j;
    return os.str();
}
