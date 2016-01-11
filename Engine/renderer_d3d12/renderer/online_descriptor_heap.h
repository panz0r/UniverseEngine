#pragma once
#include <renderer_d3d12\stdinclude.h>
#include <core/platform/platform.h>
#include <renderer_d3d12\com\com_ptr.h>
#include <d3d12.h>
#include <assert.h>

namespace ue
{

struct OnlineDescriptorHeap
{
	OnlineDescriptorHeap(unsigned max_count_)
		: _max_count(max_count_)
		, _use_count(0)
	{}
	ComPtr<ID3D12DescriptorHeap> _heap;
	ComPtr<ID3D12Fence> _fence;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _cpu_handle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE _gpu_handle;
	unsigned _max_count;
	unsigned _use_count;

	bool require(unsigned count)
	{
		bool ret = true;
		// check to see if we need to wrap around
		if ((_max_count - _use_count) < count)
		{
			//assert((_fence->GetCompletedValue() > count) && "heap to small!");

			{
				_cpu_handle = _heap->GetCPUDescriptorHandleForHeapStart();
				_gpu_handle = _heap->GetGPUDescriptorHandleForHeapStart();
				_use_count = 0;
				ret = false;
			}
		}
		_use_count += count;
		return ret;
	}
};


class OnlineDescriptorHeapFactory
{
public:
	enum Type { CBV_UAV_SRV, Sampler };
	OnlineDescriptorHeapFactory();
	~OnlineDescriptorHeapFactory();

	static void thread_initialize(ID3D12Device *device);

	OnlineDescriptorHeap *checkout_descriptor_heap(Type type);

private:
	static __THREAD_LOCAL OnlineDescriptorHeap *_cbv_uav_srv_heap;
	static __THREAD_LOCAL OnlineDescriptorHeap *_sampler_heap;

};


}