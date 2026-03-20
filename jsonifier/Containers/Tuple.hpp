/*
	MIT License

	Copyright (c) 2024 RealTimeChris

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit
	persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or
	substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/
/// Taken mostly from: https://github.com/codeinred/tuplet
/// https://github.com/RealTimeChris/jsonifier
/// Feb 3, 2023
#pragma once

#include <jsonifier/Utilities/TypeEntities.hpp>
#include <type_traits>
#include <cstddef>
#include <utility>

#if JSONIFIER_COMPILER_MSVC
	#define JSONIFIER_TUPLET_HAS_NO_UNIQUE_ADDRESS 1
	#define JSONIFIER_TUPLET_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
	#define JSONIFIER_TUPLET_HAS_NO_UNIQUE_ADDRESS 1
	#define JSONIFIER_TUPLET_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace jsonifier::internal {

	template<typename value_type>
	concept derivable_types = std::is_class_v<value_type> && !std::is_final_v<value_type>;

	template<typename... bases> struct type_list_impl : bases... {
		using base_list = type_list_impl<bases...>;
		using bases::operator[]...;
		static constexpr uint64_t size{ sizeof...(bases) };
	};

	template<auto index, typename value_type_new> struct type_list_elem {
		using value_type = value_type_new;

		JSONIFIER_TUPLET_NO_UNIQUE_ADDRESS value_type value;

		JSONIFIER_INLINE constexpr decltype(auto) operator[](tag<index>) & noexcept {
			return value;
		}

		JSONIFIER_INLINE constexpr decltype(auto) operator[](tag<index>) const& noexcept {
			return value;
		}

		JSONIFIER_INLINE constexpr decltype(auto) operator[](tag<index>) && noexcept {
			return std::move(value);
		}
	};

	template<auto index, derivable_types value_type_new> struct type_list_elem<index, value_type_new> : public std::remove_const_t<value_type_new> {
		using value_type = value_type_new;

		JSONIFIER_INLINE constexpr decltype(auto) operator[](tag<index>) & noexcept {
			return static_cast<value_type&>(*this);
		}

		JSONIFIER_INLINE constexpr decltype(auto) operator[](tag<index>) const& noexcept {
			return static_cast<const value_type&>(*this);
		}

		JSONIFIER_INLINE constexpr decltype(auto) operator[](tag<index>) && noexcept {
			return static_cast<value_type&&>(*this);
		}
	};

	template<typename index_sequence, typename... value_type> struct tuple_type_list;

	template<auto... indices, typename... value_type> struct tuple_type_list<std::integer_sequence<uint64_t, indices...>, value_type...> {
		using type = type_list_impl<type_list_elem<indices, value_type>...>;
	};

	template<typename... value_type> using type_list_t = typename tuple_type_list<std::make_integer_sequence<uint64_t,sizeof...(value_type)>, value_type...>::type;

	template<auto index, typename type_list_type> struct type_list_element {
		using type = remove_reference_t<decltype(std::declval<type_list_type>()[tag<cast<uint64_t>(index)>{}])>;
	};

	template<auto index, typename type_list_type> using type_list_element_raw_t = typename type_list_element<index, type_list_type>::type;

	template<auto index, typename type_list_type> using type_list_element_t = remove_reference_t<typename type_list_element<index, type_list_type>::type>;

	template<auto index, typename tuple_type> using tuple_element_t = type_list_element_raw_t<index, tuple_type>;

	enum class operation_types {
		eq,
		neq,
		ss,
		lt,
		lte,
		gt,
		gte,
	};

	template<typename index_sequence, operation_types operation_type> struct comparison_op;

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::eq> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr bool impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			return ((t1[tag<indices>{}] == t2[tag<indices>{}]) && ...);
		}
	};

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::neq> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr bool impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			return ((t1[tag<indices>{}] != t2[tag<indices>{}]) && ...);
		}
	};

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::ss> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr auto impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			using result_type = std::common_comparison_category_t<decltype(std::declval<value_types>() <=> std::declval<value_types>())...>;
			result_type cmp	  = result_type::equivalent;
			((cmp = t1[tag<indices>{}] <=> t2[tag<indices>{}], cmp != 0) || ...);
			return cmp;
		}
	};

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::lt> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr bool impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			return ((t1[tag<indices>{}] < t2[tag<indices>{}]) && ...);
		}
	};

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::lte> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr bool impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			return ((t1[tag<indices>{}] <= t2[tag<indices>{}]) && ...);
		}
	};

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::gt> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr bool impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			return ((t1[tag<indices>{}] > t2[tag<indices>{}]) && ...);
		}
	};

	template<auto... indices> struct comparison_op<std::integer_sequence<uint64_t, indices...>, operation_types::gte> {
		template<template<typename...> typename tuple_type, typename... value_types>
		JSONIFIER_INLINE static constexpr bool impl(const tuple_type<value_types...>& t1, const tuple_type<value_types...>& t2) noexcept {
			return ((t1[tag<indices>{}] >= t2[tag<indices>{}]) && ...);
		}
	};

	template<typename... value_types> struct tuple : type_list_t<value_types...> {
		static constexpr uint64_t size{ sizeof...(value_types) };

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator==(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::eq>::impl(*this, other);
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator!=(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::neq>::impl(*this, other);
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr auto operator<=>(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::ss>::impl(*this, other);
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator>(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::gt>::impl(*this, other);
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator>=(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::gte>::impl(*this, other);
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator<(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::lt>::impl(*this, other);
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator<=(const tuple<other_types...>& other) const noexcept {
			static_assert((std::is_same_v<std::remove_cvref_t<value_types>, std::remove_cvref_t<other_types>> && ...), "Sorry, but these tuples must contain all equal types!");
			static_assert(sizeof...(other_types) == size, "Sorry, but these tuples must be equal in size to be compared!");
			return comparison_op<std::make_integer_sequence<uint64_t, sizeof...(value_types)>, operation_types::lte>::impl(*this, other);
		}
	};

	template<> struct tuple<> : type_list_t<> {
		static constexpr uint64_t size{ 0 };

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator==(const tuple<other_types...>&) const noexcept {
			return true;
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator!=(const tuple<other_types...>&) const noexcept {
			return false;
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr auto operator<=>(const tuple<other_types...>&) const noexcept {
			return std::strong_ordering::equal;
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator>(const tuple<other_types...>&) const noexcept {
			return false;
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator>=(const tuple<other_types...>&) const noexcept {
			return false;
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator<(const tuple<other_types...>&) const noexcept {
			return false;
		}

		template<typename... other_types> JSONIFIER_INLINE constexpr bool operator<=(const tuple<other_types...>&) const noexcept {
			return false;
		}
	};

	template<typename... types> tuple(types&&...) -> tuple<types...>;

	template<auto index, typename tuple_type> JSONIFIER_INLINE static constexpr decltype(auto) get(tuple_type&& tuple_val) noexcept {
		return std::forward<tuple_type>(tuple_val)[tag<index>()];
	}

	template<typename... types> JSONIFIER_INLINE static constexpr auto make_tuple(types&&... args) noexcept {
		return tuple<types...>{ { { std::forward<types>(args) }... } };
	}

	template<typename... value_type> struct tuple_size;

	template<typename... value_type> struct tuple_size<tuple<value_type...>> : std::integral_constant<uint64_t, sizeof...(value_type)> {};

	template<typename... value_type> static constexpr auto tuple_size_v = tuple_size<std::remove_cvref_t<value_type>...>::value;

	template<typename... tuple_types> struct join_tuples;

	template<typename... tuple_types> struct join_tuples<tuple<tuple_types...>> {
		using type = tuple<remove_reference_t<tuple_types>...>;
	};

	template<typename... left_types, typename... right_types, typename... rest_types> struct join_tuples<tuple<left_types...>, tuple<right_types...>, rest_types...> {
		using type = typename join_tuples<tuple<left_types..., right_types...>, rest_types...>::type;
	};

	template<typename... left_types, typename... right_types> struct join_tuples<tuple<left_types...>, tuple<right_types...>> {
		using type = tuple<left_types..., right_types...>;
	};

	template<typename... tuple_types> using join_tuples_t = typename join_tuples<remove_cvref_t<tuple_types>...>::type;

	template<typename... list_types> struct tuple_cat_impl {
		static constexpr uint64_t total{ (list_types::size + ...) };
		using result_type	   = join_tuples_t<list_types...>;
		using lists_tuple_type = type_list_t<list_types*...>;

		struct tuple_cat_index_map {
			uint64_t list_idx[total]{};
			uint64_t local_idx[total]{};
		};

		static consteval tuple_cat_index_map get_map_values() {
			constexpr uint64_t sizes[]{ list_types::size... };
			tuple_cat_index_map m{};
			uint64_t g{};
			for (uint64_t i = 0; i < sizeof...(list_types); ++i)
				for (uint64_t j = 0; j < sizes[i]; ++j, ++g) {
					m.list_idx[g]  = i;
					m.local_idx[g] = j;
				}
			return m;
		}

		static constexpr auto map{ get_map_values() };

		template<auto index, typename list_type> JSONIFIER_INLINE static constexpr decltype(auto) get_individual_element(list_type&& list) {
			auto* __restrict tuple_ptr = std::forward<list_type>(list)[tag<map.list_idx[index]>{}];
			using list_ref_type		   = type_list_element_raw_t<map.list_idx[index], list_type>;
			using element_ref_type	   = type_list_element_raw_t<map.local_idx[index], list_ref_type>;
			return static_cast<element_ref_type>(*static_cast<list_ref_type>(tuple_ptr))[tag<map.local_idx[index]>{}];
		}

		template<typename integer_sequence> struct tuple_cat_impl_internal;

		template<auto... indices> struct tuple_cat_impl_internal<std::integer_sequence<uint64_t, indices...>> {
			template<typename list_type> JSONIFIER_INLINE static constexpr decltype(auto) impl(list_type&& lists) noexcept {
				return result_type{ { { get_individual_element<indices>(std::forward<list_type>(lists)) }... } };
			}
		};

		template<typename... list_types_new> JSONIFIER_INLINE static constexpr decltype(auto) impl(list_types_new&&... list_vals) noexcept {
			return tuple_cat_impl_internal<std::make_integer_sequence<uint64_t, total>>::impl(remove_reference_t<lists_tuple_type>{ { { &list_vals } }... });
		}
	};

	template<typename... list_types_new> JSONIFIER_INLINE static constexpr decltype(auto) tuple_cat(list_types_new&&... list_vals) noexcept {
		return tuple_cat_impl<remove_reference_t<list_types_new>...>::impl(std::forward<list_types_new>(list_vals)...);
	}

}

namespace std {

	template<typename... value_type> struct tuple_size<jsonifier::internal::tuple<value_type...>> : jsonifier::internal::integral_constant<size_t, sizeof...(value_type)> {};

	template<size_t index, typename... value_type> struct tuple_element<index, jsonifier::internal::tuple<value_type...>> {
		using type = decltype(jsonifier::internal::tuple<value_type...>::decl_elem(jsonifier::internal::tag<index>()));
	};

}