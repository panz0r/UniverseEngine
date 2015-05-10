#include "raytracer.h"
#include <math/matrix3.h>
#include <math/matrix4.h>
#include <math/sphere.h>

#include <core/thread/job_declaration.h>
#include <core/thread/fiber_system.h>

#include "camera.h"

#include <vector>

namespace ue
{

__declspec(align(64)) struct PixelResult
{
	char r,g,b;
};

struct RayParams
{
	std::vector<Sphere>* spheres;
	Ray ray;
};

void ray_job(FiberSystem *fiber_system, void *params)
{
	RayParams* p = (RayParams*)params;
	std::vector<Sphere> &spheres = *p->spheres;
	Ray &ray = p->ray;
	float closest_distance = Math::kFLOAT_MAX;
	unsigned prim_count = spheres.size();
	float p0, p1 = 0.f;
	Vector3 intersect_point;
	Vector3 normal;

	for(unsigned i = 0; i < prim_count; ++i)
	{
		if(intersectSphereRay(spheres[i], p->ray, p0, p1))
		{
			float dist = p0 < 0 ? p1 : p0;
			if (dist < closest_distance)
			{
				closest_distance = dist;
				intersect_point = ray.origin + ray.direction * closest_distance;
				normal = intersect_point - spheres[i].center;
			}
		}
	}

	float diffuse = dot(ray.direction, normal);
	if (diffuse <= 0.f)
	{
		//return Vector3::kZero;
		return;
	}

}

struct TileParams
{
	unsigned tile_width, tile_height;
	unsigned left, top;
	std::vector<Sphere>* spheres;
	Camera* camera;
	char *pixels;
};

void tile_job(FiberSystem *fiber_system, void *params)
{
	TileParams *p = (TileParams*)params;

	unsigned ray_count = p->tile_width*p->tile_height;
	Job *ray_jobs = new Job[ray_count];
	RayParams *ray_params = new RayParams[ray_count];
	
	for(unsigned y = 0; y < p->tile_height; ++y) {
		for(unsigned x = 0; x < p->tile_width; ++x) {
			unsigned p_x = x + p->left;
			unsigned p_y = y + p->top;
			unsigned index = y * p->tile_width + x;
			Ray ray;
			p->camera->GenerateRay(p_x, p_y, &ray);
			ray_params[index].spheres = p->spheres;
			ray_params[index].ray = ray;
			ray_jobs[index] = Job(ray_job, &ray_params[index]);
		}
	}
	Counter counter = fiber_system->schedule_jobs(ray_jobs, ray_count);
	fiber_system->wait_for_counter(counter, ray_count);
}


struct ScanlineParams
{
	unsigned line;
	unsigned width;
	std::vector<Sphere>* spheres;
	Camera* camera;
};

void start_job(FiberSystem *fiber_system, void *params)
{
	RaytraceParams* raytrace_params = (RaytraceParams*)params;
		
	Matrix4x4 lookat = identity4x4();
	lookat = translation(0, 0.f, 10.f);
	Matrix4x4 rotation_y = matrix4x4(rotationY(-0.7f));
	Matrix4x4 rotation_x = matrix4x4(rotationX(0.2f));
	lookat = lookat * rotation_x * rotation_y;
	lookat = inverse(lookat);

	Camera camera = Camera(raytrace_params->width, raytrace_params->height, 90.f, lookat);
	std::vector<Sphere> spheres;
	spheres.push_back(sphere(0,0,0,5.f));
	spheres.push_back(sphere(3.f,0,0,5.f));
	spheres.push_back(sphere(-3.f, 0, 0, 5.f));
	
	unsigned tile_width = 32;
	unsigned tile_height = 32;

	unsigned tiles_x = ceilf(raytrace_params->width / (float)tile_width);
	unsigned tiles_y = ceilf(raytrace_params->height / (float)tile_height);

	Job *tile_jobs = new Job[tiles_x * tiles_y];
	TileParams *tile_params = new TileParams[tiles_x * tiles_y];
	
	for(unsigned y = 0; y < tiles_y; ++y) {
		for(unsigned x = 0; x < tiles_x; ++x) {
			unsigned index = y * tiles_x + x;
			tile_params[index].left = x * tile_width;
			tile_params[index].top = y * tile_height;
			tile_params[index].tile_width = tile_width;
			tile_params[index].tile_height = tile_height;
			tile_params[index].camera = &camera;
			tile_params[index].spheres = &spheres;
			tile_params[index].pixels = raytrace_params->pixels;
			tile_jobs[index] = Job(tile_job, &tile_params[index]);
		}
	}

	
	Counter counter = fiber_system->schedule_jobs(tile_jobs, tiles_x*tiles_y);
	fiber_system->wait_for_counter(counter, tiles_x*tiles_y);
	

}

}