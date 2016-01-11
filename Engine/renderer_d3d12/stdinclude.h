#pragma once

#include <renderer_d3d12/com/com_ptr.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <stdint.h>

#define ALIGN(size, align) ((size) + (align-1) & ~(align-1))
#define UE_NO_IMPL
