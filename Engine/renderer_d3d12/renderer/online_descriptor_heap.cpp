#include "online_descriptor_heap.h"

namespace ue
{

__THREAD_LOCAL OnlineDescriptorHeap *OnlineDescriptorHeapFactory::_cbv_uav_srv_heap = nullptr;
__THREAD_LOCAL OnlineDescriptorHeap *OnlineDescriptorHeapFactory::_sampler_heap = nullptr;


OnlineDescriptorHeapFactory::OnlineDescriptorHeapFactory()
{

}

OnlineDescriptorHeapFactory::~OnlineDescriptorHeapFactory()
{}

void OnlineDescriptorHeapFactory::thread_initialize(ID3D12Device *device)
{
	unsigned max_count = 1000; // what is a good number?
	_cbv_uav_srv_heap = new OnlineDescriptorHeap(max_count);
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = max_count; 
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_cbv_uav_srv_heap->_heap));
	_cbv_uav_srv_heap->_cpu_handle = _cbv_uav_srv_heap->_heap->GetCPUDescriptorHandleForHeapStart();
	_cbv_uav_srv_heap->_gpu_handle = _cbv_uav_srv_heap->_heap->GetGPUDescriptorHandleForHeapStart();

	_sampler_heap = new OnlineDescriptorHeap(max_count);
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_sampler_heap->_heap));
	_sampler_heap->_cpu_handle = _sampler_heap->_heap->GetCPUDescriptorHandleForHeapStart();
	_sampler_heap->_gpu_handle = _sampler_heap->_heap->GetGPUDescriptorHandleForHeapStart();

}

OnlineDescriptorHeap *OnlineDescriptorHeapFactory::checkout_descriptor_heap(Type type)
{
	OnlineDescriptorHeap *ret = nullptr;
	if (type == CBV_UAV_SRV)
		ret = _cbv_uav_srv_heap;
	if (type == Sampler)
		ret = _sampler_heap;

	return ret;
}

}