#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include "dynamic-cast-adaptor.h"

class test : public CppUnit::TestCase {
private:
	void test_range_for();
	void test_range_for_empty();
	void test_range_for_no_match();
	void test_iterator_loop();
	void test_for_each();
	void test_any_of();
	void test_all_of();
	void test_none_of();
	void test_find_if();
	void test_range_adaptor();
	void test_begin();
	void test_increment();
	void test_count_if();
	void test_count();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_range_for);
	CPPUNIT_TEST(test_range_for_empty);
	CPPUNIT_TEST(test_range_for_no_match);
	CPPUNIT_TEST(test_iterator_loop);
	CPPUNIT_TEST(test_for_each);
	CPPUNIT_TEST(test_any_of);
	CPPUNIT_TEST(test_all_of);
	CPPUNIT_TEST(test_none_of);
	CPPUNIT_TEST(test_find_if);
	CPPUNIT_TEST(test_range_adaptor);
	CPPUNIT_TEST(test_begin);
	CPPUNIT_TEST(test_increment);
	CPPUNIT_TEST(test_count_if);
	CPPUNIT_TEST(test_count);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

class Base {
public:
	Base() = default;
	Base(std::string const& name)
	:
		name_(name)
	{ }

	std::string name() const
	{
		return name_;
	}

	virtual ~Base() = default;

private:
	std::string name_;
};

class A : public Base {
public:
	using Base::Base;
};

class B : public Base {
public:
	using Base::Base;
};

class C : public Base {
public:
	using Base::Base;
};

std::vector<std::shared_ptr<Base>> make_test_vector()
{
	return {
		std::make_shared<A>("A1"),
		std::make_shared<B>("B1"),
		std::make_shared<C>("C1"),
		std::make_shared<A>("A2"),
		std::make_shared<B>("B2"),
		std::make_shared<C>("C2"),
		std::make_shared<A>("A3"),
		std::make_shared<B>("B3"),
		std::make_shared<C>("C3"),
	};
}

void test::test_range_for()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	for (auto const& p : caster) {
		++count;
	}

	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_range_for_empty()
{
	auto v = std::vector<std::shared_ptr<Base>>{};
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	for (auto const& p : caster) {
		++count;
	}

	CPPUNIT_ASSERT_EQUAL(size_t(0), count);
}

void test::test_range_for_no_match()
{
	auto v = std::vector<std::shared_ptr<Base>>{
		std::make_shared<B>(),
		std::make_shared<C>(),
		std::make_shared<B>(),
		std::make_shared<C>(),
		std::make_shared<B>(),
		std::make_shared<C>(),
	};
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	for (auto const& p : caster) {
		++count;
	}

	CPPUNIT_ASSERT_EQUAL(size_t(0), count);
}

void test::test_iterator_loop()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	for (auto it = std::begin(caster); it != std::end(caster); ++it) {
		++count;
	}

	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_for_each()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	std::for_each(begin(caster), end(caster), [&count](std::shared_ptr<A> const&) { ++count; });

	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_any_of()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	auto res = std::any_of(begin(caster), end(caster), [&count](std::shared_ptr<A> const&) { ++count; return false; });

	CPPUNIT_ASSERT(!res);
	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_all_of()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	auto res = std::all_of(begin(caster), end(caster), [&count](std::shared_ptr<A> const&) { ++count; return true; });

	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_none_of()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	auto res = std::none_of(begin(caster), end(caster), [&count](std::shared_ptr<A> const&) { ++count; return false; });

	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_find_if()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	auto it = std::find_if(begin(caster), end(caster), [&count](std::shared_ptr<A> const& a) { ++count; return a->name() == "A2"; });

	CPPUNIT_ASSERT_EQUAL(std::string("A2"), (*it)->name());
	CPPUNIT_ASSERT_EQUAL(size_t(2), count);
}

void test::test_range_adaptor()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(v);
	auto count = size_t(0);

	for (auto const& p : caster) {
		++count;
	}

	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_begin()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(v);

	auto it = std::begin(caster);

	CPPUNIT_ASSERT_EQUAL(std::string("A1"), (*it)->name());
}

void test::test_increment()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(v);

	auto it = std::begin(caster);
	using diff_t = std::iterator_traits<decltype(it)>::difference_type;

	CPPUNIT_ASSERT_EQUAL(diff_t(0), std::distance(std::begin(caster), it));
	CPPUNIT_ASSERT(it != std::end(caster));
	CPPUNIT_ASSERT_EQUAL(std::string("A1"), (*it)->name());

	++it;
	CPPUNIT_ASSERT_EQUAL(diff_t(1), std::distance(std::begin(caster), it));
	CPPUNIT_ASSERT(it != std::end(caster));
	CPPUNIT_ASSERT_EQUAL(std::string("A2"), (*it)->name());

	++it;
	CPPUNIT_ASSERT_EQUAL(diff_t(2), std::distance(std::begin(caster), it));
	CPPUNIT_ASSERT(it != std::end(caster));
	CPPUNIT_ASSERT_EQUAL(std::string("A3"), (*it)->name());

	++it;
	CPPUNIT_ASSERT_EQUAL(diff_t(3), std::distance(std::begin(caster), it));
	CPPUNIT_ASSERT(it == std::end(caster));

	++it;
	CPPUNIT_ASSERT_EQUAL(diff_t(3), std::distance(std::begin(caster), it));
	CPPUNIT_ASSERT(it == std::end(caster));
}

void test::test_count_if()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));
	auto count = size_t(0);

	auto res = std::count_if(begin(caster), end(caster), [&count](std::shared_ptr<A> const&) { ++count; return true; });

	using diff_t = std::iterator_traits<decltype(begin(caster))>::difference_type;
	CPPUNIT_ASSERT_EQUAL(diff_t(3), res);
	CPPUNIT_ASSERT_EQUAL(size_t(3), count);
}

void test::test_count()
{
	auto v = make_test_vector();
	auto caster = make_dynamic_cast_adaptor<A>(std::begin(v), std::end(v));

	auto res = std::count(begin(caster), end(caster), v[3]);

	using diff_t = std::iterator_traits<decltype(begin(caster))>::difference_type;
	CPPUNIT_ASSERT_EQUAL(diff_t(1), res);
}

int main(int, char *[])
{
	auto & registry{CppUnit::TestFactoryRegistry::getRegistry()};

	CppUnit::TextTestRunner runner;
	runner.addTest(registry.makeTest());

	CppUnit::BriefTestProgressListener progress;
	runner.eventManager().addListener(&progress);

	runner.setOutputter(new CppUnit::CompilerOutputter(
		&runner.result(), std::cerr, "%p:%l:"));

	return runner.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}

