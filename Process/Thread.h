#pragma once

#include "Value.h"
#include <queue>
#include <map>
#include <memory>
#include <atomic>
#include <boost\thread.hpp>
namespace obj
{
	using std::queue;
	using std::map;
	using std::atomic_bool;
	using std::function;
	using std::bind;
	using boost::thread;
	using boost::mutex;
	using boost::condition_variable;
	using boost::lock_guard;
	using boost::unique_lock;
	using boost::shared_lock;
	namespace this_thread = boost::this_thread;
}

namespace obj
{
	class Thread;
	using ThreadPtr = shared_ptr<Thread>;
	typedef UuId ThreadId;


	class Thread : public std::enable_shared_from_this<Thread>
	{
		explicit Thread(const String& name);
		friend inline shared_ptr<Thread> make_shared();

	public:
		~Thread();
		static struct ThreadPool
		{
			ThreadPool();
			~ThreadPool() = default;
			ThreadPtr operator [] (const String& name);
			ThreadPtr operator [] (const ThreadId& id) const;

		private:
			struct Impl;
			unique_ptr<Impl> _pImpl;
		} Pool;



	public:
		template<class _Fn, class... _Args>
		void start(_Fn&& _Fx, _Args&&... _Ax)
		{
			_thread = thread(_Fx, forward<_Args>(_Ax)...);
		}


		template<class _Fn, class... _Args>
		void call(_Fn&& _Fx, _Args&&... _Ax)
		{
			auto f = bind(_Fx, forward<_Args>(_Ax)...);
			enqueue(f);
		}

		void start();
		void stop();
		static void standardLoop(ThreadPtr pThread);

		bool joinable() { return _thread.joinable(); }
		void join() { _thread.join(); }
		thread::id get_id() { return _thread.get_id(); }

	private:
		String _name;
		ThreadId _id;
		thread _thread;

		mutex _mutex;
		condition_variable _condition;
		atomic_bool _enqueued = false, _stopped = false;
		queue<function<void()>> _queue;
		void enqueue(function<void()> f);

	public:
		MemberProperty<String> Name;
		MemberProperty<ThreadId> const Id;
	};
}