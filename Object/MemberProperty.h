#pragma once
#include <functional>
#include <type_traits>
#undef max
#include <limits>

using std::function;
template<class T> struct Nothing { T nothing_; };

using std::is_integral;
using std::is_signed;
using std::make_signed;
using std::make_unsigned;

template<class L, class R, bool Integer, bool LeftSigned, bool RightSigned>
struct LessSignedUnsigned
{
	inline bool operator()(const L& l, const R& r) { return l < r; }
};
template<class L, class R>
struct LessSignedUnsigned<L, R, true, true, false>
{
	inline bool operator()(L l, R r) { return l < static_cast<make_signed<R>::type>(r); }
};
template<class L, class R>
struct LessSignedUnsigned<L, R, true, false, true>
{
	inline bool operator()(L l, R r) { return l < static_cast<make_unsigned<R>::type>(r); }
};
template<class L, class R>
bool less_compare(const L& l, const R& r)
{
	return LessSignedUnsigned<L, R, is_integral<L>::value, is_signed<L>::value, is_signed<R>::value>()(l, r);
}

template<class L, class R, bool Integer, bool LeftSigned, bool RightSigned>
struct GreaterSignedUnsigned
{
	inline bool operator()(const L& l, const R& r) { return l > r; }
};
template<class L, class R>
struct GreaterSignedUnsigned<L, R, true, true, false>
{
	inline bool operator()(L l, R r) { return l > static_cast<make_signed<R>::type>(r); }
};
template<class L, class R>
struct GreaterSignedUnsigned<L, R, true, false, true>
{
	inline bool operator()(L l, R r) { return l > static_cast<make_unsigned<R>::type>(r); }
};
template<class L, class R>
bool greater_compare(const L& l, const R& r)
{
	return GreaterSignedUnsigned<L, R, is_integral<L>::value, is_signed<L>::value, is_signed<R>::value>()(l, r);
}

template<class T, class HostT = Nothing<T>, T HostT::*member = &Nothing<T>::nothing_>
struct ReadMember
{
	using Getter = function<T()>;
	using Type = T;

	ReadMember(Getter g) : getter_(g) {}
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
	template<typename R>
	T operator = (const R& m)
	{
		static_assert(0, "bullshit!");
	}
	template<typename R> inline bool operator == (const R& r) { return getter_() == r; }
	template<typename R> inline bool operator != (const R& r) { return getter_() != r; }
	template<typename R> inline bool operator < (const R& r) { return less_compare(getter_(), r); }
	template<typename R> inline bool operator > (const R& r) { return greater_compare(getter_(), r); }
	template<typename R> inline bool operator <= (const R& r) { return *this == r || *this < r; }
	template<typename R> inline bool operator >= (const R& r) { return return *this == r || *this > r; }

protected:
	Getter getter_;
};
template<class T, class HostT, T HostT::*member>
struct WriteMember : public ReadMember<T, HostT, member>
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

template<typename L, class R, class H, R H::*m>
bool operator == (const L& l, const ReadMember<R, H, m>& r) { return l == r.operator R(); }
template<typename L, class R, class H, R H::*m>
bool operator != (const L& l, const ReadMember<R, H, m>& r) { return l != r.operator R(); }
template<typename L, class R, class H, R H::*m>
bool operator < (const L& l, const ReadMember<R, H, m>& r) { return less_compare(l, r.operator R()); }
template<typename L, class R, class H, R H::*m>
bool operator > (const L& l, const ReadMember<R, H, m>& r) { return greater_compare(l, r.operator R()); }
template<typename L, class R, class H, R H::*m>
bool operator <= (const L& l, const ReadMember<R, H, m>& r) { return l == r || l < r; }
template<typename L, class R, class H, R H::*m>
bool operator >= (const L& l, const ReadMember<R, H, m>& r) { return l == r || l > r; }
