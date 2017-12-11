#include "stdafx.h"
#include "Thread.h"
#include <map>

obj::Thread::Thread(const String& name)
	: _name(name)
	, Name(_name)
	, _id(generateId())
	, Id(_id)
{
}


obj::Thread::~Thread()
{
	if (_thread.joinable())
		_thread.join();
}


obj::Thread::ThreadPool obj::Thread::Pool;


obj::String obj::Thread::toString() const
{
	return _name;
}

void obj::Thread::enqueue(function<void()> f)
{
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(f);
		_enqueued = true;
	}
	_condition.notify_one();
}


void obj::Thread::standardLoop(ThreadPtr pThread)
{
	String threadName = pThread->Name;
	do
	{
		unique_lock<mutex> lock(pThread->_mutex);
		pThread->_condition.wait(lock, [pThread] { return pThread->_enqueued.load(); });
		pThread->_enqueued = false;

		while (pThread->_queue.empty() == false)
		{
			function<void()> f = pThread->_queue.back();
			pThread->_queue.pop();
			lock.unlock();

			f();
			this_thread::yield();

			lock.lock();
		}

	} while (pThread->_stopped == false);

}

bool obj::Thread::operator<(const Thing& other) const
{
	if (auto o = dynamic_cast<decltype(this)>(&other))
		return *this < *o;
	else
		return Thing::operator<(other);
}

bool obj::Thread::operator<(const Thread& other) const
{
	return get_id() < other.get_id();
}

void obj::Thread::start()
{
	_thread = thread(standardLoop, shared_from_this());
}

void obj::Thread::stop()
{
	_stopped = true;
	_condition.notify_one();
	this_thread::yield();
}

namespace obj
{
	struct Thread::ThreadPool::Impl
	{
		mutex _mutex;
		map<ThreadId, ThreadPtr> _threadMap;
		map<String, ThreadId> _threadIdMap;
	};
}


obj::Thread::ThreadPool::ThreadPool() : _pImpl(make_unique<obj::Thread::ThreadPool::ThreadPool::Impl>()) {}


obj::ThreadPtr obj::Thread::ThreadPool::operator[](const String& name)
{
	lock_guard<mutex> lock(_pImpl->_mutex);
	ThreadId& id = _pImpl->_threadIdMap[name];
	auto it = _pImpl->_threadMap.find(id);
	if (it == _pImpl->_threadMap.end())
	{
		ThreadPtr pThread(new Thread(name));
		id = pThread->Id;
		it = _pImpl->_threadMap.insert(make_pair(pThread->Id, pThread)).first;
	}
	return it->second;
}

obj::ThreadPtr obj::Thread::ThreadPool::operator[](const ThreadId& id) const
{
	lock_guard<mutex> lock(_pImpl->_mutex);
	auto it = _pImpl->_threadMap.find(id);
	if (it == _pImpl->_threadMap.end())
		return ThreadPtr();
	return it->second;
}
