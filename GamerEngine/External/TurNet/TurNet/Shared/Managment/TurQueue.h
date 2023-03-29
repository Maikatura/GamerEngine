#pragma once
#include <mutex>
#include <deque>

namespace TurNet
{
	template<typename T>
	class TurQueue
	{
	public:
		TurQueue() = default;
		TurQueue(const TurQueue<T>&) = delete;
		virtual ~TurQueue() { clear(); }

	public:
		const T& front() 
		{
			std::scoped_lock lock(myQueueMutex);
			return myDeque.front();
		}

		const T& back()
		{
			std::scoped_lock lock(myQueueMutex);
			return myDeque.back();
		}

		void push_back(const T& aItem)
		{
			std::scoped_lock lock(myQueueMutex);
			myDeque.emplace_back(std::move(aItem));
		}

		void push_front(const T& aItem)
		{
			std::scoped_lock lock(myQueueMutex);
			myDeque.emplace_front(std::move(aItem));
		}

		bool empty()
		{
			std::scoped_lock lock(myQueueMutex);
			return myDeque.empty();
		}

		size_t count()
		{
			std::scoped_lock lock(myQueueMutex);
			return myDeque.size();
		}

		size_t size()
		{
			std::scoped_lock lock(myQueueMutex);
			return myDeque.size();
		}

		void clear()
		{
			std::scoped_lock lock(myQueueMutex);
			myDeque.clear();
		}

		T pop_front()
		{
			std::scoped_lock lock(myQueueMutex);
			auto t = std::move(myDeque.front());
			myDeque.pop_front();
			return t;
		}

		T pop_back()
		{
			std::scoped_lock lock(myQueueMutex);
			auto t = std::move(myDeque.back());
			myDeque.pop_back();
			return t;
		}

	protected:
		std::mutex myQueueMutex;
		std::deque<T> myDeque;
	};
}