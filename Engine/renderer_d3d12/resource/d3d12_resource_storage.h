#pragma once
#include <vector>
#include <map>

namespace ue
{

typedef unsigned RenderHandle;

//struct RenderHandle
//{
//	unsigned type : 7;
//	unsigned dynamic : 1;
//	unsigned index : 24;
//};

typedef unsigned RenderSetHandle;

inline RenderHandle make_render_handle(unsigned type, unsigned index)
{
	RenderHandle handle = (type << 24) | index;
	return handle;
}

inline RenderHandle make_render_handle(unsigned hash)
{
	return (RenderHandle)hash;
}

inline unsigned extract_type(RenderHandle handle)
{
	return (unsigned)(handle >> 24) & 0xff;
}

inline unsigned extract_index(RenderHandle handle)
{
	return (handle & 0xffffff);
}

template <typename T>
class ResourceStorage
{
public:
	RenderHandle insert(T& resource) {

		unsigned index = (unsigned)_resources.size();
		if (_free_list.size() > 0) {
			index = _free_list.back();
			_free_list.pop_back();
		}
		if (index == _resources.size())
			_resources.push_back(resource);
		else
			_resources[index] = resource;
		return make_render_handle(resource->type, index);
	}

	inline T& get(unsigned index) {
		return _resources[index];
	}

	inline void remove(unsigned index) {
		_free_list.push_back(index);
	}

private:
	typedef std::vector<T> Resources;
	typedef std::vector<unsigned> UnusedResources;
	Resources _resources;
	UnusedResources _free_list;
};

template <typename T>
class ResourceSet
{
public:
	RenderSetHandle insert(unsigned hash, T resource) {
		_map.insert_or_assign(hash, resource);
		return make_render_handle(hash);
	}

	bool contains(unsigned hash) {
		return _map.find(hash) != _map.end();
	}

	inline T get(unsigned hash) {
		return _map[hash];
	}

	inline void remove(unsigned hash) {
		//auto iter = _map.find(hash);
		//_map.erase(iter);
	}

private:
	std::map<unsigned,T> _map;
};


}