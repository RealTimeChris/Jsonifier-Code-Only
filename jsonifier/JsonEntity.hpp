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
/// https://github.com/RealTimeChris/jsonifier
/// Feb 3, 2023
#pragma once

#include <jsonifier/Reflection.hpp>

namespace jsonifier_internal {

	template<typename value_type> struct base_json_entity {
		using member_type = std::remove_cvref_t<value_type>;
		static constexpr size_t index{ 0 };
	};

	template<auto memberPtrNew, string_literal nameNew> struct json_entity_temp {
		using member_type = remove_class_pointer_t<std::remove_cvref_t<decltype(memberPtrNew)>>;
		using class_type  = remove_member_pointer_t<std::remove_cvref_t<decltype(memberPtrNew)>>;
		static constexpr member_type class_type::* memberPtr{ memberPtrNew };
		static constexpr string_literal name{ nameNew };
	};

	template<auto memberPtrNew, string_literal nameNew, size_t indexNew, size_t maxIndex> struct json_entity {
		using member_type = remove_class_pointer_t<std::remove_cvref_t<decltype(memberPtrNew)>>;
		using class_type  = remove_member_pointer_t<std::remove_cvref_t<decltype(memberPtrNew)>>;
		static constexpr member_type class_type::* memberPtr{ memberPtrNew };
		static constexpr bool isItLast{ indexNew == maxIndex - 1 };
		static constexpr string_literal name{ nameNew };
		static constexpr size_t index{ indexNew };
	};

	template<typename value_type>
	concept is_base_json_entity = requires { typename std::remove_cvref_t<value_type>::member_type; } && !std::is_member_pointer_v<std::remove_cvref_t<value_type>>;

	template<typename value_type>
	concept is_json_entity_temp = requires {
		typename std::remove_cvref_t<value_type>::class_type;
		std::remove_cvref_t<value_type>::memberPtr;
	} && is_base_json_entity<value_type>;


	template<size_t maxIndex, size_t index, auto value> constexpr auto makeJsonEntityAuto() noexcept {
		if constexpr (is_json_entity_temp<decltype(value)>) {
			json_entity<value.memberPtr, value.name, index, maxIndex> jsonEntity{};
			return jsonEntity;
		} else {
			constexpr auto nameNew = getName<value>();
			json_entity<value, stringLiteralFromView<nameNew.size()>(nameNew), index, maxIndex> jsonEntity{};
			return jsonEntity;
		}
	}

	template<typename value_type>
	concept convertible_to_json_entity = is_json_entity_temp<value_type> || std::is_member_pointer_v<value_type>;

	template<auto... values, size_t... indices> constexpr auto createValueImpl(std::index_sequence<indices...>) {
		static_assert((convertible_to_json_entity<decltype(values)> && ...), "All arguments passed to createValue must be convertible to a json_entity.");
		return makeTuple(makeJsonEntityAuto<sizeof...(values), indices, values>()...);
	}

}

namespace jsonifier {

	template<auto memberPtr, jsonifier_internal::string_literal nameNew> constexpr auto makeJsonEntity() {
		return jsonifier_internal::json_entity_temp<memberPtr, nameNew>{};
	}

	template<auto memberPtr> constexpr auto makeJsonEntity() {
		return jsonifier_internal::json_entity_temp<memberPtr, jsonifier_internal::getName<memberPtr>()>{};
	}

	template<auto... values> constexpr auto createValue() noexcept {
		return jsonifier_internal::createValueImpl<values...>(std::make_index_sequence<sizeof...(values)>{});
	}

}