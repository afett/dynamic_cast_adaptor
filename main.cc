#include <vector>
#include "dynamic-cast-adaptor.h"

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

int main()
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

	return 0;
}
