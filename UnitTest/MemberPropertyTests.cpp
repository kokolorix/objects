#include "stdafx.h"
#include "MemberPropertyTests.h"
#include "Value.h"
#include "MemberProperty.h"

using namespace obj;

namespace
{
	bool test = Test::addTestCase(make_shared<Test::MemberPropertyTests>(), "MemberPropertyTests");
}

struct Host
{	
	Host(uint32_t id, bool ready, const String& name )
		: id_(id), ready_(ready), name_(name) {}

protected:
	uint32_t id_;
	bool ready_;
	String name_;

public:
	ReadMember<uint32_t, Host, &Host::id_> Id = { this };
	ReadMember<bool, Host, &Host::ready_> Ready = { this };
	WriteMember<String, Host, &Host::name_> Name = { this };
	ReadMember<double> Pi = { 3.14 };
	ReadMember<char*> ClassName = { "Host" };
	ReadMember<UuId> Guid = { generateId() };
	ReadMember<uint64_t> BigOne = { 33 };

	ReadMember<uint32_t> Id2 = { std::bind(&Host::getId, this) };

private:
	uint32_t getId() const { return Id; }
};

obj::TestResult obj::Test::MemberPropertyTests::runTest()
{
	Host h = { 17,  true, "theName" };
	assert(h.Ready != false);
	bool ready = h.Ready;
	String name = h.Name;
	assert(h.Id == 17);
	assert(h.Id == h.Id2);
	//h.Pi = 2.77;
	assert(0 != h.Id);
	assert(20 > h.Id);
	assert(h.Id < 20);
	assert(h.Id > 10);
	assert(h.Pi == 3.14);
	assert(h.ClassName == "Host");
	assert("Host" == h.ClassName);
	assert(h.ClassName == h.ClassName);
	assert(name == h.Name);
	assert(h.Name == name);
	assert(h.Name == "theName");
	assert("theName" == h.Name);
	h.Name = "anotherName";
	assert(h.Name == "anotherName");

	assert(h.Guid != generateId());

	assert(h.BigOne < std::numeric_limits<uint64_t>::max());
	assert(15 < h.BigOne);
	return TestResult::Successful;
}
