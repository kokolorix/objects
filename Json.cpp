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
    if (auto v = std::dynamic_pointer_cast<StringValue>(p))
        j.back() = v->value();
    // else if (BooleanValuePtr v = p)
    //     j.back() = v->value();
    // else if (Int32ValuePtr v = p)
    //     j.back() = v->value();
    // else if (FloatValuePtr v = p)
    //     j.back() = v->value();
    // else if (UuIdValuePtr v = p
    //     j.back() = to_string(v->value();
    // else if (ObjectValuePtr v = p
    // {
    //     json jo = v->value();
    //     j.back() = jo;
    // }
    // else if (VectorValuePtr v = p
    // {
    // }

    String s = j.dump(4);
    std::cout << s;
}

void to_json(json &j, const PropertyPtr &p)
{
    j[p->name()] = p->value();
    // json jv = p->value();
    //j = json{ { "name", p.name },{ "address", p.address },{ "age", p.age } };
}

void to_json(json &j, const ObjectPtr &o)
{
    using boost::uuids::to_string;
    String id = to_string(o->id());
    j["id"] = id;
    for (PropertyPtr p : o->properties())
    {
        json j_p = p;
        // j = p;
    }

    //j = json{ { "name", p.name },{ "address", p.address },{ "age", p.age } };
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
