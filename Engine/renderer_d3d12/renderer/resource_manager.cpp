#include "resource_manager.h"
#include "render_device.h"

namespace ue
{


static const D3D12_RESOURCE_DIMENSION dimension_lut[] = 
{
	D3D12_RESOURCE_DIMENSION_UNKNOWN,
	D3D12_RESOURCE_DIMENSION_BUFFER,
	D3D12_RESOURCE_DIMENSION_TEXTURE1D,
	D3D12_RESOURCE_DIMENSION_TEXTURE2D,
	D3D12_RESOURCE_DIMENSION_TEXTURE3D,
};

static const D3D12_SRV_DIMENSION srv_dimension_lut[] = 
{
	D3D12_SRV_DIMENSION_UNKNOWN,
	D3D12_SRV_DIMENSION_BUFFER,
	D3D12_SRV_DIMENSION_TEXTURE1D,
	D3D12_SRV_DIMENSION_TEXTURE1DARRAY,
	D3D12_SRV_DIMENSION_TEXTURE2D,
	D3D12_SRV_DIMENSION_TEXTURE2DARRAY,
	D3D12_SRV_DIMENSION_TEXTURE2DMS,
	D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY,
	D3D12_SRV_DIMENSION_TEXTURE3D,
	D3D12_SRV_DIMENSION_TEXTURECUBE,
	D3D12_SRV_DIMENSION_TEXTURECUBEARRAY
};

static const DXGI_FORMAT format_lut[] =
{
	DXGI_FORMAT_R8G8B8A8_UNORM,
};

ResourceManager::ResourceManager(D3D12RenderDevice & device)
	: _render_device(device)
{
	//// create some heaps
	{
		ComPtr<ID3D12DescriptorHeap> heap;
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NodeMask = 0;
		desc.NumDescriptors = 8; 
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; 
		device.device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
		_srv_offline_heap.push_back(heap);
	}

	//{
	//	D3D12_DESCRIPTOR_HEAP_DESC desc;
	//	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//	desc.NodeMask = 0;
	//	desc.NumDescriptors = 2; // something determined in advance
	//	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // renter target views does not need shader visibility
	//	device.device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_dsv_heap));
	//	_dsv_desc_size = device.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//}

	//{
	//	D3D12_DESCRIPTOR_HEAP_DESC desc;
	//	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//	desc.NodeMask = 0;
	//	desc.NumDescriptors = 20; // something determined in advance
	//	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//	device.device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_cbv_srv_uav_heap));
	//	_cbv_srv_uav_desc_size = device.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//}

	//{
	//	D3D12_DESCRIPTOR_HEAP_DESC desc;
	//	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	//	desc.NodeMask = 0;
	//	desc.NumDescriptors = 20; // something determined in advance
	//	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//	device.device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_sampler_heap));
	//	_sampler_desc_size = device.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	//}
}

ResourceManager::~ResourceManager()
{
}

void* ResourceManager::lookup_resource(RenderHandle resource_handle)
{
	unsigned type = extract_type(resource_handle);
	unsigned index = extract_index(resource_handle);
	switch (type)
	{
	case RenderResource::Texture:
		return _textures[index];
		break;
	}

	return nullptr;
}


void ResourceManager::create_texture(const TextureDesc &desc, const void *data)
{
	size_t required_size = sizeof(TextureResource);
	
	required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * desc.dynamic ? _render_device.buffer_count() : 1;
	// Todo, UAV access

	// Alloc enough memory to hold all required data
	TextureResource *texture = (TextureResource*)malloc(required_size);

	// fixup pointers
	char *ptr = ((char*)texture) + sizeof(TextureResource);
	texture->srv = (D3D12_CPU_DESCRIPTOR_HANDLE*)ptr;

	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.Dimension = dimension_lut[desc.dimension];
	resource_desc.Width = desc.width;
	resource_desc.Height = desc.height;
	resource_desc.DepthOrArraySize = desc.depth;
	resource_desc.MipLevels = desc.mip_levels;
	resource_desc.Format = format_lut[desc.format];
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;

	ID3D12Device *device = _render_device.device();

	// TODO, multiple resources for dynamic version
	device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texture->resource));

	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.ViewDimension = srv_dimension_lut[4]; // tdodo
	srv_desc.Format = format_lut[desc.format];
	srv_desc.Texture2D.MipLevels = desc.mip_levels;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


	CD3DX12_CPU_DESCRIPTOR_HANDLE srv_handle; // todo, take handle from offline heap
	device->CreateShaderResourceView(texture->resource.Get(), &srv_desc, srv_handle);

	// copy handle data to resource part
	memcpy(texture->srv, &srv_handle, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

}

}