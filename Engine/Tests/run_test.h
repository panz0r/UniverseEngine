#pragma once

namespace ue
{

#define RUN_TEST(klass, width, height) { ue::RunTest<ue::klass>(width, height, #klass); }

template <class T>
class RunTest
{
public:
	RunTest(unsigned width, unsigned height, const char* name)
	{
		_test = new T(width, height, name);
		_test->run();
	}

	~RunTest()
	{
		delete _test;
	}
private:
	T* _test;
};


}