#pragma once

namespace ue
{

#define INDEX_BITS 15
#define COUNTER_BITS 11
#define TYPE_BITS 6

struct Handle
{
	static Handle InvalidHandle;

	enum 
	{
		CounterLSH = (INDEX_BITS),
		TypeLSH = (INDEX_BITS+COUNTER_BITS),
	};

	Handle() : _index(0), _counter(0), _type(0) {}
	Handle(unsigned index, unsigned type, unsigned counter)
	: _index(index)
	, _counter(counter)
	, _type(type)
	{}
	Handle(const Handle& handle) : _index(handle._index), _counter(handle._counter), _type(handle._type) {}
	Handle(unsigned handle)
		: _index((handle & INDEX_BITS))
		, _counter((handle >> CounterLSH) & COUNTER_BITS)
		, _type((handle >> TypeLSH) & TYPE_BITS)
	{}

	inline unsigned type() const { return _type; }
	inline operator unsigned() const
	{
		return (_type << (TypeLSH)) | (_counter << (CounterLSH)) | _index;
	}

	unsigned _index:INDEX_BITS;
	unsigned _counter:COUNTER_BITS;
	unsigned _type:TYPE_BITS;
};

}