#include "stdafx.h"
#include "MemberPropertyTests.h"
#include "Value.h"
#undef max
#include <limits>
//#include <assert.h>
//#include <functional>

using namespace obj;

namespace
{
	bool test = Test::addTestCase(make_shared<Test::MemberPropertyTests>(), "MemberPropertyTests");
}

using std::function;
template<class T> struct Nothing { T nothing_; };

template<class T, class HostT = Nothing<T>, T HostT::*member = &Nothing<T>::nothing_>
struct ReadMember
{
	using Getter = function<T()>;

	ReadMember(HostT* host) 
	{
		getter_ = [host]() 
		{ 
			return (*host).*member;
		};
	}
	ReadMember(const T& m) 
	{
		getter_ = [m]() 
		{ 
			return m;
		};
	}
	operator T () const 
	{
		return getter_();
	}
	template<typename OT>
	T operator = (const OT& m)
	{
		static_assert(0, "bullshit!");
	}
	template<typename OT> inline bool operator == (const OT& o) { return getter_() == o; }
	template<typename OT> inline bool operator != (const OT& o) { return getter_() != o; }
	template<typename OT> inline bool operator < (const OT& o) { return getter_() < o; }
	template<typename OT> inline bool operator > (const OT& o) { return getter_() > o; }
	template<typename OT> inline bool operator <= (const OT& o) { return getter_() <= o; }
	template<typename OT> inline bool operator >= (const OT& o){ return getter_() >= o; }

protected:
	Getter getter_;
};
template<class T, class HostT, T HostT::*member>
struct WriteMember : public ReadMember<T,HostT,member>
{
	using Setter = function<T(const T&)>;
	WriteMember(HostT* host) : ReadMember(host)
	{
		setter_ = [host](const T& m)
		{
			return (host->*member) = m;
		};
	}
	T operator = (const T& m)
	{
		return setter_(m);
	}
private:
	Setter setter_;
};

template<typename OT, class T, class HostT, T HostT::*member>
bool operator == (const OT& l, const ReadMember<T,HostT,member>& r) { return l == r.operator T(); }
template<typename OT, class T, class HostT, T HostT::*member>
bool operator != (const OT& l, const ReadMember<T, HostT, member>& r) { return l != r.operator T(); }
template<typename OT, class T, class HostT, T HostT::*member>
bool operator < (const OT& l, const ReadMember<T, HostT, member>& r) { /*if (std::is_signed<OT>::value);*/ return l < r.operator T(); }
template<typename OT, class T, class HostT, T HostT::*member>
bool operator > (const OT& l, const ReadMember<T, HostT, member>& r) { return l > r.operator T(); }
template<typename OT, class T, class HostT, T HostT::*member>
bool operator <= (const OT& l, const ReadMember<T, HostT, member>& r) { return l <= r.operator T(); }
template<typename OT, class T, class HostT, T HostT::*member>
bool operator >= (const OT& l, const ReadMember<T, HostT, member>& r) { return l >= r.operator T(); }

struct Host
{	
	uint32_t id_;
	bool ready_;
	String name_;
	ReadMember<uint32_t, Host, &Host::id_> Id = { this };
	ReadMember<bool, Host, &Host::ready_> Ready = { this };
	WriteMember<String, Host, &Host::name_> Name = { this };
	ReadMember<double> Pi = { 3.14 };
	ReadMember<char*> ClassName = { "Host" };
	ReadMember<UuId> Guid = { generateId() };
	ReadMember<uint64_t> BigOne = { 33 };
};

obj::TestResult obj::Test::MemberPropertyTests::runTest()
{
	Host h = { 17,  true, "theName" };
	assert(h.Ready != false);
	bool ready = h.Ready;
	String name = h.Name;
	assert(h.Id == 17);
	//h.Pi = 2.77;
	assert(0 != h.Id);
	assert(20 < h.Id);
	//assert(h.Id < 20);
	//assert(h.Id > 10);
	assert(h.Pi == 3.14);
	assert(h.ClassName == "Host");
	assert("Host" == h.ClassName);
	assert(h.ClassName == h.ClassName);
	assert(name == h.Name);
	assert(h.Name == name);
	assert(h.Name == "theName");
	assert("theName" == h.Name);
	//assert("theName" == /*(String)*/h.Name);
	h.Name = "anotherName";
	assert(h.Name == "anotherName");

	assert(h.BigOne < std::numeric_limits<uint64_t>::max());
	return TestResult::Successful;
}
