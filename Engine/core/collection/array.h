#pragma once

#include <memory>
#include <core/assert/assert.h>

namespace ue
{

template <typename T>
class Array
{
public:
	static const size_t item_size = sizeof(T);

public:
	Array() : _ptr(nullptr), _size(0), _capacity(0) {}
	// wtf is this?
	Array(const Array<T>& other) : _ptr(other._ptr), _size(other._size), _capacity(other._capacity) {}
	Array(unsigned capacity) : _ptr(nullptr), _size(0), _capacity(0)
	{
		reserve(capacity);
	}

	unsigned size() const {
		return _size;
	}

	const T& operator[](unsigned index) const {
		UENSURE(index >= 0 && index < _size);
		return _ptr[index];
	}

	T& operator[](unsigned index) {
		UENSURE(index >= 0 && index < _size);
		return _ptr[index];
	}

	void reserve(unsigned size) {
		if(size > _capacity)
			grow(size);
	}

	void resize(unsigned size) {
		if (size > _capacity)
			grow(size);
		_size = size;
	}
	void push_back(const reference item) {
		if (_size + 1 > _capacity)
			grow(_size*2 + 10);
		pointer ptr = back();
		memcpy(ptr, &item, item_size);
		_size++;
	}

	void pop_back() {
		shrink(1);
	}

	const T* back() const {
		return <const T*>(_ptr + _size-1);
	}

	const T& back_reference() const {
		return *(_ptr + _size-1);
	}

	T* back() {
		return reinterpret_cast<T*>(_ptr + _size-1);
	}

	T& back_reference() {
		return *(_ptr + _size-1);
	}

private:

	void grow(unsigned new_capacity) {
		UENSURE(new_capacity > 0);
		void* new_ptr = malloc(new_capacity * item_size);
		if (_ptr) {
			memcpy(new_ptr, _ptr, _size * item_size);
			free(_ptr);
		}
		_ptr = new_ptr;
		_capacity = new_capacity;
	}

	void shrink(unsigned amount) {
		UENSURE(amount <= _size);
		_size -= amount;
	}

	T* _ptr;
	unsigned _size;
	unsigned _capacity;
};

}