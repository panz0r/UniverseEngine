#pragma once

#include <core/assert/assert.h>
#include <core/collection/array.h>
#include "handle.h"

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

namespace ue
{

class HandleContainer
{
public:

	enum { COUNTER_MAX = (1 << COUNTER_BITS) };

	HandleContainer() : _next_free_index(0), _use_count(0) {};

	Handle add(void* ptr, unsigned type)
	{
		unsigned index = _next_free_index;
		if (index >= _handles.size()) {
			grow(_handles.size() + 10);
		}
		
		HandleEntry& entry = _handles[index];
		UEASSERT(entry._in_use == FALSE && "handle in use");
		_next_free_index = entry._next_free_index;

		Handle handle = Handle(index, type, entry._counter);
		entry._in_use = TRUE;
		entry._counter = (entry._counter + 1) % COUNTER_MAX;
		entry._next_free_index = 0;
		entry._data = ptr;
		_use_count++;
		return handle;
	}

	void remove(Handle handle)
	{
		unsigned index = handle._index;
		HandleEntry& entry = _handles[index];
		UEASSERT(entry._counter == handle._counter && "trying to remove recycled handle");
		UEASSERT(entry._in_use == TRUE && "trying to remove already removed handle handle");
		entry._in_use = 0;
		entry._data = 0;
		entry._next_free_index = _next_free_index;
		_next_free_index = index;
		_use_count--;
	}

	inline void* get(Handle handle)
	{
		void* ptr;
		if (get_ptr(handle, &ptr))
			return ptr;
		return 0;
	}

private:
	struct HandleEntry
	{
		HandleEntry() : _next_free_index(0), _counter(0), _in_use(0),  _data(0) {}
		explicit HandleEntry(unsigned next_free_index) : _next_free_index(next_free_index), _counter(0), _in_use(0), _data(0) {}

		unsigned _next_free_index:INDEX_BITS;
		unsigned _counter:COUNTER_BITS;
		unsigned _in_use:1;
		void* _data;
	};

	inline void grow(unsigned new_size)
	{
		unsigned old_size = _handles.size();
		_handles.resize(new_size);
		for (unsigned i = old_size; i < new_size; ++i) {
			_handles[i] = HandleEntry(i + 1);
		}
	}

	inline bool get_ptr(Handle handle, void** ptr)
	{
		unsigned index = handle._index;
		HandleEntry& entry = _handles[index];
		if (entry._in_use == FALSE ||
			entry._counter != handle._counter)
			return false;

		*ptr = entry._data;
		return true;
	}

	Array<HandleEntry> _handles;
	unsigned _next_free_index;
	unsigned _use_count;
};

}


