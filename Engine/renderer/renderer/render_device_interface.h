#pragma once

namespace ue
{

struct RenderDeviceDesc
{
	void * window_handle;
	unsigned width;
	unsigned height;
	unsigned back_buffer_count;
	bool fullscreen;
	bool debug;
};

class IRenderDevice
{
public:
	virtual ~IRenderDevice() {};

	virtual void open() = 0;
	virtual void close() = 0;
	virtual unsigned current_back_buffer() = 0;
	virtual unsigned back_buffer_count() = 0;
	virtual void present() = 0;
	virtual void wait_for_fence() = 0;
};

}