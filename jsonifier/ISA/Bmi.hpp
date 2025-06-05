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

#include <jsonifier/TypeEntities.hpp>

namespace simd_internal {

#if JSONIFIER_CHECK_FOR_INSTRUCTION(JSONIFIER_BMI) || JSONIFIER_CHECK_FOR_INSTRUCTION(JSONIFIER_ANY_AVX)

	#define blsr(value) _blsr_u64(value)

	template<jsonifier::concepts::uns16_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
	#if defined(JSONIFIER_LINUX)
		return __tzcnt_u16(value);
	#else
		return _tzcnt_u16(value);
	#endif
	}

	template<jsonifier::concepts::uns32_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
		return _tzcnt_u32(value);
	}

	template<jsonifier::concepts::uns64_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
		return _tzcnt_u64(value);
	}

#elif JSONIFIER_CHECK_FOR_INSTRUCTION(JSONIFIER_NEON)

	#define blsr(value) (value & (value - 1))

	template<jsonifier::concepts::uns16_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
		if (value != 0) {
	#if JSONIFIER_REGULAR_VISUAL_STUDIO
			return _tzcnt_u16(value);
	#else
			return __builtin_ctz(value);
	#endif
		} else {
			return sizeof(value_type) * 8;
		}
	}

	template<jsonifier::concepts::uns32_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
		if (value != 0) {
	#if JSONIFIER_REGULAR_VISUAL_STUDIO
			return _tzcnt_u32(value);
	#else
			return __builtin_ctz(value);
	#endif
		} else {
			return sizeof(value_type) * 8;
		}
	}

	template<jsonifier::concepts::uns64_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
		if (value != 0) {
	#if JSONIFIER_REGULAR_VISUAL_STUDIO
			return _tzcnt_u64(value);
	#else
			return __builtin_ctzll(value);
	#endif
		} else {
			return sizeof(value_type) * 8;
		}
	}

#else

	#define blsr(value) (value & (value - 1))

	template<jsonifier::concepts::unsigned_t value_type> JSONIFIER_ALWAYS_INLINE value_type tzcnt(value_type value) noexcept {
		if (value == 0) {
			return sizeof(value_type) * 8;
		}

		value_type count{};
		while ((value & 1) == 0) {
			value >>= 1;
			++count;
		}

		return count;
	}

#endif

}
