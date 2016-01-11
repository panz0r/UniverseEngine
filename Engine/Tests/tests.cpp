#include "tests.h"
#include "run_test.h"

///////////////////////////////
// Tests

#include "fullscreen_triangle.h"
#include "basic_object.h"
#include "constant_buffer.h"

///////////////////////////////

extern "C" {

__declspec(dllexport) void run_tests()
{ 
	//RUN_TEST(FullScreenTriangleTest, 1024, 768);
	//RUN_TEST(ConstantBufferTest, 1024, 768);
	RUN_TEST(BasicObjectTest, 1024, 768);
}

} 
