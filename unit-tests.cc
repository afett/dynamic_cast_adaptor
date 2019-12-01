#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include "dynamic-cast-adaptor.h"

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void simple_test();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(simple_test);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::simple_test()
{ }

class Base {
public:
	virtual ~Base() = default;
};

class A : public Base {
public:
};

class B : public Base {
public:
};

class C : public Base {
public:
};

int main(int, char *[])
{
	auto v = std::vector<std::shared_ptr<Base>>{
		std::make_shared<A>(),
		std::make_shared<B>(),
		std::make_shared<C>(),
		std::make_shared<A>(),
		std::make_shared<B>(),
		std::make_shared<C>(),
		std::make_shared<A>(),
		std::make_shared<B>(),
		std::make_shared<C>(),
	};

	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	std::for_each(begin(caster), end(caster), [](std::shared_ptr<A> const&) { });

	std::find_if(begin(caster), end(caster), [](std::shared_ptr<A> const&) { return false; });
	std::any_of(begin(caster), end(caster), [](std::shared_ptr<A> const&) { return false; });
	std::all_of(begin(caster), end(caster), [](std::shared_ptr<A> const&) { return false; });
	std::none_of(begin(caster), end(caster), [](std::shared_ptr<A> const&) { return false; });

	auto & registry{CppUnit::TestFactoryRegistry::getRegistry()};

	CppUnit::TextTestRunner runner;
	runner.addTest(registry.makeTest());

	CppUnit::BriefTestProgressListener progress;
	runner.eventManager().addListener(&progress);

	runner.setOutputter(new CppUnit::CompilerOutputter(
		&runner.result(), std::cerr, "%p:%l:"));

	return runner.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
