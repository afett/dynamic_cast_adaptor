/*
 * Copyright (c) 2019 Andreas Fett.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DYNAMIC_CAST_ADAPTOR_H
#define DYNAMIC_CAST_ADAPTOR_H

#include <algorithm>
#include <memory>

template <typename Derived, typename BaseIt>
class dynamic_cast_adaptor {
public:
	dynamic_cast_adaptor(BaseIt const& begin, BaseIt const& end)
	:
		begin_{begin},
		end_{end}
	{ }

	dynamic_cast_adaptor() = delete;
	dynamic_cast_adaptor(dynamic_cast_adaptor const&) = default;
	dynamic_cast_adaptor(dynamic_cast_adaptor &&) = default;
	dynamic_cast_adaptor & operator=(dynamic_cast_adaptor const&) = default;
	dynamic_cast_adaptor & operator=(dynamic_cast_adaptor &&) = default;
	~dynamic_cast_adaptor() = default;

	class iterator {
	public:
		using difference_type = typename BaseIt::difference_type;
		using value_type = decltype(std::dynamic_pointer_cast<Derived>(*BaseIt()));
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::input_iterator_tag;

		iterator() = default;
		iterator(iterator const&) = default;
		iterator(iterator &&) = default;
		iterator & operator=(iterator const&) = default;
		iterator & operator=(iterator &&) = default;
		~iterator() = default;

		friend bool operator!=(iterator const& l, iterator const& r)
		{
			return l.it_ != r.it_;
		}

		friend bool operator==(iterator const& l, iterator const& r)
		{
			return l.it_ == r.it_;
		}

		iterator & operator++()
		{
			if (it_ != end_) {
				it_ = find_next_derived(it_ + 1, end_);
			}
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp(*this);
			(*this)++;
			return tmp;
		}

		value_type operator*() const
		{
			return std::dynamic_pointer_cast<Derived>(*it_);
		}

	private:
		friend class dynamic_cast_adaptor;
		iterator(BaseIt const& it, BaseIt const& end)
		:
			it_{find_next_derived(it, end)},
			end_{end}
		{ }

		BaseIt find_next_derived(BaseIt const& begin, BaseIt end) const
		{
			using base_ptr_type = decltype(*BaseIt());
			return std::find_if(begin, end, [](base_ptr_type const& p) {
				return std::dynamic_pointer_cast<Derived>(p) != nullptr;
			});
		}

		BaseIt it_;
		BaseIt end_;
	};

	iterator begin() const
	{
		return iterator(begin_, end_);
	}

	iterator end() const
	{
		return iterator(end_, end_);
	}

private:
	BaseIt begin_;
	BaseIt end_;
};

template <typename Derived, typename BaseIt>
dynamic_cast_adaptor<Derived, BaseIt> make_dynamic_cast_adaptor(BaseIt const& begin, BaseIt const& end)
{
	return dynamic_cast_adaptor<Derived, BaseIt>(begin, end);
}

template <typename Derived, typename Range>
dynamic_cast_adaptor<Derived, typename Range::const_iterator> make_dynamic_cast_adaptor(Range const& c)
{
	return dynamic_cast_adaptor<Derived, typename Range::const_iterator>(std::begin(c), std::end(c));
}

#endif // DYNAMIC_CAST_ADAPTOR_H
