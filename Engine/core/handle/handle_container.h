#pragma once

#include <core/assert/assert.h>
#include <vector>
#include "handle.h"

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

namespace ue
{

template <typename T>
class HandleContainer
{
public:

	enum { COUNTER_MAX = (1 << COUNTER_BITS) };

	HandleContainer() : _next_free_index(0), _use_count(0) {};

	Handle insert(void *ptr, unsigned type)
	{
		unsigned index = _next_free_index;
		if (index >= _handles.size()) {
			grow(_handles.size() + 10);
		}
		
		HandleEntry& entry = _handles[index];
		UASSERT(entry._in_use == FALSE, "handle in use");
		_next_free_index = entry._next_free_index;

		entry._in_use = TRUE;
		entry._counter = (entry._counter + 1) % COUNTER_MAX;
		entry._next_free_index = 0;
		entry._data = ptr;
		Handle handle = Handle(index, type, entry._counter);
		_use_count++;
		return handle;
	}

	void remove(Handle handle)
	{
		unsigned index = handle._index;
		HandleEntry& entry = _handles[index];
		UASSERT(entry._counter == handle._counter, "trying to remove recycled handle");
		UASSERT(entry._in_use == TRUE, "trying to remove already removed handle handle");
		entry._in_use = 0;
		entry._data = nullptr;
		entry._next_free_index = _next_free_index;
		_next_free_index = index;
		_use_count--;
	}

	inline bool is_valid(Handle handle)
	{
		HandleEntry &entry = _handles[handle._index];
		if (entry._in_use == false || entry._counter != handle._counter)
			return false;
		return true;
	}

	template <typename T>
	inline const T& operator()(Handle handle) const
	{
		const HandleEntry &entry = _handles[handle._index];
		UASSERT(entry._in_use == true && entry._counter == handle._counter, "Handle is expired");
		return *reinterpret_cast<const T*>(entry._data);
	}

	template <typename T>
	inline T* get_ptr(Handle handle) const
	{
		const HandleEntry &entry = _handles[handle._index];
		UASSERT(entry._in_use == true && entry._counter == handle._counter, "Handle is expired");
		return reinterpret_cast<T*>(entry._data);
	}

private:
	struct HandleEntry
	{
		HandleEntry() : _next_free_index(0), _counter(0), _in_use(0) {}
		explicit HandleEntry(unsigned next_free_index) : _next_free_index(next_free_index), _counter(0), _in_use(0) {}

		unsigned _next_free_index:INDEX_BITS;
		unsigned _counter:COUNTER_BITS;
		unsigned _in_use:1;
		void *_data;
	};

	inline void grow(unsigned new_size)
	{
		unsigned old_size = _handles.size();
		_handles.resize(new_size);
		for (unsigned i = old_size; i < new_size; ++i) {
			_handles[i] = HandleEntry(i + 1);
		}
	}


	//inline bool get_ptr(Handle handle, T* ptr)
	//{
	//	unsigned index = handle._index;
	//	HandleEntry& entry = _handles[index];
	//	if (entry._in_use == FALSE ||
	//		entry._counter != handle._counter)
	//		return false;

	//	*ptr = entry._data;
	//	return true;
	//}

	std::vector<HandleEntry> _handles;
	unsigned _next_free_index;
	unsigned _use_count;
};

}


