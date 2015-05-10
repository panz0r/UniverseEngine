#pragma once

namespace ue
{

struct RaytraceParams
{
	unsigned width, height;
	char* pixels;
};

void start_job(void *params);


}