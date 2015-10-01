#pragma once
#include <wrl/client.h>

// TODO, roll own perhaps

namespace ue
{

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

}