#include "stdafx.h"
#include "MemberPropertyTests.h"
#include <assert.h>
#include <functional>

using namespace obj;

namespace
{
	bool test = Test::addTestCase(make_shared<Test::MemberPropertyTests>(), "MemberPropertyTests");
}

using std::function;

template<class T, class HostT, T HostT::*member>
struct ReadMember
{
	using Getter = function<T()>;
	//using Setter = function<T(const T&)>;

	ReadMember(HostT* host) 
	{
		getter_ = [host]() 
		{ 
			return (*host).*member;
		};
	}
	operator T () const { return getter_(); }
	Getter getter_;
};

template<class T, class HostT, T HostT::*member>
bool operator == (const ReadMember<T,HostT,member>& l, const T& r) { return l.operator T() == r; }
template<class T, class HostT, T HostT::*member>
bool operator == (const ReadMember<T,HostT,member>& l, const char* r) { return l.operator T() == r; }
template<class T, class HostT, T HostT::*member>
bool operator == (const T& l, const ReadMember<T,HostT,member>& r) { return l == r.operator T(); }
template<class T, class HostT, T HostT::*member>
bool operator == (const char* l, const ReadMember<T,HostT,member>& r) { return l == r.operator T(); }

struct Host
{	
	bool ready_;
	String name_;
	ReadMember<bool, Host, &Host::ready_> Ready = {this};
	ReadMember<String, Host, &Host::name_> Name = {this};
};




obj::TestResult obj::Test::MemberPropertyTests::runTest()
{
	Host h = { true, "theName" };
	assert(h.Ready);
	bool ready = h.Ready;
	String name = h.Name;
	assert(h.Name == "theName");
	assert("theName" == /*(String)*/h.Name);
	return TestResult::Successful;
}
