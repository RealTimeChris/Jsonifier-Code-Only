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
/// Nov 13, 2023
#pragma once

#include <jsonifier/Allocator.hpp>

#include <concepts>
#include <cstdint>
#include <cstring>
#include <array>

namespace jsonifier::internal {

	struct int_serializing_package_1 {
		mutable uint64_t value01;
	};

	struct int_serializing_package_2 {
		mutable uint64_t value01;
		mutable uint64_t value02;
	};

	struct int_serializing_package_3 {
		mutable uint64_t value01;
		mutable uint64_t value02;
		mutable uint64_t value03;
	};

	alignas(2) JSONIFIER_INLINE_VARIABLE char charTable1[]{ 0x30u, 0x31u, 0x32u, 0x33u, 0x34u, 0x35u, 0x36u, 0x37u, 0x38u, 0x39u };

	alignas(2) JSONIFIER_INLINE_VARIABLE char charTable2[]{ 0x30u, 0x30u, 0x30u, 0x31u, 0x30u, 0x32u, 0x30u, 0x33u, 0x30u, 0x34u, 0x30u, 0x35u, 0x30u, 0x36u, 0x30u, 0x37u, 0x30u,
		0x38u, 0x30u, 0x39u, 0x31u, 0x30u, 0x31u, 0x31u, 0x31u, 0x32u, 0x31u, 0x33u, 0x31u, 0x34u, 0x31u, 0x35u, 0x31u, 0x36u, 0x31u, 0x37u, 0x31u, 0x38u, 0x31u, 0x39u, 0x32u,
		0x30u, 0x32u, 0x31u, 0x32u, 0x32u, 0x32u, 0x33u, 0x32u, 0x34u, 0x32u, 0x35u, 0x32u, 0x36u, 0x32u, 0x37u, 0x32u, 0x38u, 0x32u, 0x39u, 0x33u, 0x30u, 0x33u, 0x31u, 0x33u,
		0x32u, 0x33u, 0x33u, 0x33u, 0x34u, 0x33u, 0x35u, 0x33u, 0x36u, 0x33u, 0x37u, 0x33u, 0x38u, 0x33u, 0x39u, 0x34u, 0x30u, 0x34u, 0x31u, 0x34u, 0x32u, 0x34u, 0x33u, 0x34u,
		0x34u, 0x34u, 0x35u, 0x34u, 0x36u, 0x34u, 0x37u, 0x34u, 0x38u, 0x34u, 0x39u, 0x35u, 0x30u, 0x35u, 0x31u, 0x35u, 0x32u, 0x35u, 0x33u, 0x35u, 0x34u, 0x35u, 0x35u, 0x35u,
		0x36u, 0x35u, 0x37u, 0x35u, 0x38u, 0x35u, 0x39u, 0x36u, 0x30u, 0x36u, 0x31u, 0x36u, 0x32u, 0x36u, 0x33u, 0x36u, 0x34u, 0x36u, 0x35u, 0x36u, 0x36u, 0x36u, 0x37u, 0x36u,
		0x38u, 0x36u, 0x39u, 0x37u, 0x30u, 0x37u, 0x31u, 0x37u, 0x32u, 0x37u, 0x33u, 0x37u, 0x34u, 0x37u, 0x35u, 0x37u, 0x36u, 0x37u, 0x37u, 0x37u, 0x38u, 0x37u, 0x39u, 0x38u,
		0x30u, 0x38u, 0x31u, 0x38u, 0x32u, 0x38u, 0x33u, 0x38u, 0x34u, 0x38u, 0x35u, 0x38u, 0x36u, 0x38u, 0x37u, 0x38u, 0x38u, 0x38u, 0x39u, 0x39u, 0x30u, 0x39u, 0x31u, 0x39u,
		0x32u, 0x39u, 0x33u, 0x39u, 0x34u, 0x39u, 0x35u, 0x39u, 0x36u, 0x39u, 0x37u, 0x39u, 0x38u, 0x39u, 0x39u };

	JSONIFIER_INLINE_VARIABLE uint16_t charTable02[]{ 0x3030u, 0x3130u, 0x3230u, 0x3330u, 0x3430u, 0x3530u, 0x3630u, 0x3730u, 0x3830u, 0x3930u, 0x3031u, 0x3131u, 0x3231u, 0x3331u,
		0x3431u, 0x3531u, 0x3631u, 0x3731u, 0x3831u, 0x3931u, 0x3032u, 0x3132u, 0x3232u, 0x3332u, 0x3432u, 0x3532u, 0x3632u, 0x3732u, 0x3832u, 0x3932u, 0x3033u, 0x3133u, 0x3233u,
		0x3333u, 0x3433u, 0x3533u, 0x3633u, 0x3733u, 0x3833u, 0x3933u, 0x3034u, 0x3134u, 0x3234u, 0x3334u, 0x3434u, 0x3534u, 0x3634u, 0x3734u, 0x3834u, 0x3934u, 0x3035u, 0x3135u,
		0x3235u, 0x3335u, 0x3435u, 0x3535u, 0x3635u, 0x3735u, 0x3835u, 0x3935u, 0x3036u, 0x3136u, 0x3236u, 0x3336u, 0x3436u, 0x3536u, 0x3636u, 0x3736u, 0x3836u, 0x3936u, 0x3037u,
		0x3137u, 0x3237u, 0x3337u, 0x3437u, 0x3537u, 0x3637u, 0x3737u, 0x3837u, 0x3937u, 0x3038u, 0x3138u, 0x3238u, 0x3338u, 0x3438u, 0x3538u, 0x3638u, 0x3738u, 0x3838u, 0x3938u,
		0x3039u, 0x3139u, 0x3239u, 0x3339u, 0x3439u, 0x3539u, 0x3639u, 0x3739u, 0x3839u, 0x3939u };

	JSONIFIER_INLINE_VARIABLE uint8_t digitCounts[]{ 19, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10,
		10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1 };

	JSONIFIER_INLINE_VARIABLE uint64_t digitCountThresholds[]{ 0ull, 9ull, 99ull, 999ull, 9999ull, 99999ull, 999999ull, 9999999ull, 99999999ull, 999999999ull, 9999999999ull,
		99999999999ull, 999999999999ull, 9999999999999ull, 99999999999999ull, 999999999999999ull, 9999999999999999ull, 99999999999999999ull, 999999999999999999ull,
		9999999999999999999ull };

	JSONIFIER_INLINE uint64_t fastDigitCount(const uint64_t inputValue) {
		const uint64_t originalDigitCount{ digitCounts[jsonifier::simd::lzcnt(inputValue)] };
		return originalDigitCount + static_cast<uint64_t>(inputValue > digitCountThresholds[originalDigitCount]);
	}

	JSONIFIER_INLINE static string_buffer_ptr length1(string_buffer_ptr buf, const uint64_t value) noexcept {
		std::memcpy(buf, charTable1 + value, 1);
		return buf + 1;
	}

	JSONIFIER_INLINE static string_buffer_ptr length2(string_buffer_ptr buf, const uint64_t value) noexcept {
		std::memcpy(buf, charTable02 + value, 2);
		return buf + 2;
	}

	JSONIFIER_INLINE static string_buffer_ptr length3(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_1 intPackage{};
		intPackage.value01 = (value * 5243) >> 19;
		std::memcpy(buf, charTable1 + intPackage.value01, 1);
		std::memcpy(buf + 1, charTable02 + (value - intPackage.value01 * 100), 2);
		return buf + 3;
	}

	JSONIFIER_INLINE static string_buffer_ptr length4(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_1 intPackage{};
		intPackage.value01 = (value * 5243) >> 19;
		std::memcpy(buf, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 2, charTable02 + (value - intPackage.value01 * 100), 2);
		return buf + 4;
	}

	JSONIFIER_INLINE static string_buffer_ptr length5(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_2 intPackage{};
		intPackage.value01 = (value * 429497) >> 32;
		std::memcpy(buf, charTable1 + intPackage.value01, 1);
		intPackage.value02 = value - intPackage.value01 * 10000;
		intPackage.value01 = (intPackage.value02 * 5243) >> 19;
		std::memcpy(buf + 1, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 3, charTable02 + (intPackage.value02 - intPackage.value01 * 100), 2);
		return buf + 5;
	}

	JSONIFIER_INLINE static string_buffer_ptr length6(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_2 intPackage{};
		intPackage.value01 = (value * 429497) >> 32;
		std::memcpy(buf, charTable02 + intPackage.value01, 2);
		intPackage.value02 = value - intPackage.value01 * 10000;
		intPackage.value01 = (intPackage.value02 * 5243) >> 19;
		std::memcpy(buf + 2, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 4, charTable02 + (intPackage.value02 - intPackage.value01 * 100), 2);
		return buf + 6;
	}

	JSONIFIER_INLINE static string_buffer_ptr length7(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_3 intPackage{};
		intPackage.value01 = (value * 109951163) >> 40;
		intPackage.value02 = value - intPackage.value01 * 10000;
		intPackage.value03 = (intPackage.value01 * 5243) >> 19;
		std::memcpy(buf, charTable1 + intPackage.value03, 1);
		std::memcpy(buf + 1, charTable02 + (intPackage.value01 - intPackage.value03 * 100), 2);
		intPackage.value01 = (intPackage.value02 * 5243) >> 19;
		std::memcpy(buf + 3, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 5, charTable02 + (intPackage.value02 - intPackage.value01 * 100), 2);
		return buf + 7;
	}

	JSONIFIER_INLINE static string_buffer_ptr length8(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_3 intPackage{};
		intPackage.value01 = (value * 109951163) >> 40;
		intPackage.value02 = value - intPackage.value01 * 10000;
		intPackage.value03 = (intPackage.value01 * 5243) >> 19;
		std::memcpy(buf, charTable02 + intPackage.value03, 2);
		std::memcpy(buf + 2, charTable02 + (intPackage.value01 - intPackage.value03 * 100), 2);
		intPackage.value01 = (intPackage.value02 * 5243) >> 19;
		std::memcpy(buf + 4, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 6, charTable02 + (intPackage.value02 - intPackage.value01 * 100), 2);
		return buf + 8;
	}

	JSONIFIER_INLINE static string_buffer_ptr length9(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_3 intPackage{};
		intPackage.value01 = value * 11258999ull >> 50;
		intPackage.value02 = value - intPackage.value01 * 100000000;
		std::memcpy(buf, charTable1 + intPackage.value01, 1);
		intPackage.value01 = (intPackage.value02 * 109951163) >> 40;
		intPackage.value02 = intPackage.value02 - intPackage.value01 * 10000;
		intPackage.value03 = (intPackage.value01 * 5243) >> 19;
		std::memcpy(buf + 1, charTable02 + intPackage.value03, 2);
		std::memcpy(buf + 3, charTable02 + (intPackage.value01 - intPackage.value03 * 100), 2);
		intPackage.value01 = (intPackage.value02 * 5243) >> 19;
		std::memcpy(buf + 5, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 7, charTable02 + (intPackage.value02 - intPackage.value01 * 100), 2);
		return buf + 9;
	}

	JSONIFIER_INLINE static string_buffer_ptr length10(string_buffer_ptr buf, const uint64_t value) noexcept {
		constexpr int_serializing_package_3 intPackage{};
		intPackage.value01 = value * 11258999ull >> 50;
		intPackage.value02 = value - intPackage.value01 * 100000000;
		std::memcpy(buf, charTable02 + intPackage.value01, 2);
		intPackage.value01 = (intPackage.value02 * 109951163) >> 40;
		intPackage.value02 = intPackage.value02 - intPackage.value01 * 10000;
		intPackage.value03 = (intPackage.value01 * 5243) >> 19;
		std::memcpy(buf + 2, charTable02 + intPackage.value03, 2);
		std::memcpy(buf + 4, charTable02 + (intPackage.value01 - intPackage.value03 * 100), 2);
		intPackage.value01 = (intPackage.value02 * 5243) >> 19;
		std::memcpy(buf + 6, charTable02 + intPackage.value01, 2);
		std::memcpy(buf + 8, charTable02 + (intPackage.value02 - intPackage.value01 * 100), 2);
		return buf + 10;
	}

	JSONIFIER_INLINE static string_buffer_ptr length11(string_buffer_ptr buf, const uint64_t value) {
		const uint64_t z = value / 10ull;
		const uint64_t u = value - z * 10ull;
		buf				 = length10(buf, z);
		return length1(buf, u);
	}

	JSONIFIER_INLINE static string_buffer_ptr length12(string_buffer_ptr buf, const uint64_t value) {
		const uint64_t z = value / 100ull;
		const uint64_t u = value - z * 100ull;
		buf				 = length10(buf, z);
		return length2(buf, u);
	}

	JSONIFIER_INLINE static string_buffer_ptr length13(string_buffer_ptr buf, const uint64_t value) {
		const uint64_t z = value / 1000ull;
		const uint64_t u = value - z * 1000ull;
		buf				 = length10(buf, z);
		return length3(buf, u);
	}

	JSONIFIER_INLINE static string_buffer_ptr length14(string_buffer_ptr buf, const uint64_t value) {
		const uint64_t z = value / 10000ull;
		const uint64_t u = value - z * 10000ull;
		buf				 = length10(buf, z);
		return length4(buf, u);
	}

	JSONIFIER_INLINE static string_buffer_ptr length15(string_buffer_ptr buf, const uint64_t value) {
		const uint64_t z = value / 100000ull;
		const uint64_t u = value - z * 100000ull;
		buf				 = length10(buf, z);
		return length5(buf, u);
	}

	JSONIFIER_INLINE static string_buffer_ptr length16(string_buffer_ptr buf, const uint64_t value) {
		const uint64_t z = value / 1000000ull;
		const uint64_t u = value - z * 1000000ull;
		buf				 = length10(buf, z);
		return length6(buf, u);
	}

	JSONIFIER_INLINE static string_buffer_ptr length17(string_buffer_ptr buf, const uint64_t value) noexcept {
		const uint64_t tmp	   = value / 100000000;
		const uint64_t low	   = value - tmp * 100000000;
		const uint64_t high	   = tmp / 10000;
		const uint64_t mid	   = tmp - high * 10000;
		const uint64_t value01 = (low * 109951163) >> 40;
		const uint64_t value02 = low - value01 * 10000;
		uint64_t aa			   = (high * 429497) >> 32;
		const uint64_t bbcc	   = high - aa * 10000;
		uint64_t value03	   = (bbcc * 5243) >> 19;
		uint64_t cc			   = bbcc - value03 * 100;
		std::memcpy(buf, charTable1 + aa, 1);
		std::memcpy(buf + 1, charTable02 + value03, 2);
		std::memcpy(buf + 3, charTable02 + cc, 2);
		aa		= (mid * 5243) >> 19;
		value03 = mid - aa * 100;
		std::memcpy(buf + 5, charTable02 + aa, 2);
		std::memcpy(buf + 7, charTable02 + value03, 2);
		aa				  = (value01 * 5243) >> 19;
		value03			  = value01 - aa * 100;
		cc				  = (value02 * 5243) >> 19;
		const uint64_t dd = value02 - cc * 100;
		std::memcpy(buf + 9, charTable02 + aa, 2);
		std::memcpy(buf + 11, charTable02 + value03, 2);
		std::memcpy(buf + 13, charTable02 + cc, 2);
		std::memcpy(buf + 15, charTable02 + dd, 2);
		return buf + 17;
	}

	JSONIFIER_INLINE static string_buffer_ptr length18(string_buffer_ptr buf, const uint64_t value) noexcept {
		const uint64_t tmp	   = value / 100000000;
		const uint64_t low	   = value - tmp * 100000000;
		const uint64_t high	   = tmp / 10000;
		const uint64_t mid	   = tmp - high * 10000;
		const uint64_t value01 = (low * 109951163) >> 40;
		const uint64_t value02 = low - value01 * 10000;
		uint64_t aa			   = (high * 429497) >> 32;
		const uint64_t bbcc	   = high - aa * 10000;
		uint64_t value03	   = (bbcc * 5243) >> 19;
		uint64_t cc			   = bbcc - value03 * 100;
		std::memcpy(buf, charTable02 + aa, 2);
		std::memcpy(buf + 2, charTable02 + value03, 2);
		std::memcpy(buf + 4, charTable02 + cc, 2);
		aa		= (mid * 5243) >> 19;
		value03 = mid - aa * 100;
		std::memcpy(buf + 6, charTable02 + aa, 2);
		std::memcpy(buf + 8, charTable02 + value03, 2);
		aa				  = (value01 * 5243) >> 19;
		value03			  = value01 - aa * 100;
		cc				  = (value02 * 5243) >> 19;
		const uint64_t dd = value02 - cc * 100;
		std::memcpy(buf + 10, charTable02 + aa, 2);
		std::memcpy(buf + 12, charTable02 + value03, 2);
		std::memcpy(buf + 14, charTable02 + cc, 2);
		std::memcpy(buf + 16, charTable02 + dd, 2);
		return buf + 18;
	}

	JSONIFIER_INLINE static string_buffer_ptr length19(string_buffer_ptr buf, const uint64_t value) noexcept {
		const uint64_t tmp	= value / 100000000;
		const uint64_t low	= value - tmp * 100000000;
		const uint64_t high = tmp / 10000;
		const uint64_t mid	= tmp - high * 10000;
		uint64_t value01	= (high * 109951163) >> 40;
		uint64_t value02	= high - value01 * 10000;
		uint64_t aa			= (value01 * 5243) >> 19;
		uint64_t value03	= value01 - aa * 100;
		uint64_t cc			= (value02 * 5243) >> 19;
		uint64_t dd			= value02 - cc * 100;
		std::memcpy(buf, charTable1 + aa, 1);
		std::memcpy(buf + 1, charTable02 + value03, 2);
		std::memcpy(buf + 3, charTable02 + cc, 2);
		std::memcpy(buf + 5, charTable02 + dd, 2);
		aa		= (mid * 5243) >> 19;
		value03 = mid - aa * 100;
		std::memcpy(buf + 7, charTable02 + aa, 2);
		std::memcpy(buf + 9, charTable02 + value03, 2);
		value01 = (low * 109951163) >> 40;
		value02 = low - value01 * 10000;
		aa		= (value01 * 5243) >> 19;
		value03 = value01 - aa * 100;
		cc		= (value02 * 5243) >> 19;
		dd		= value02 - cc * 100;
		std::memcpy(buf + 11, charTable02 + aa, 2);
		std::memcpy(buf + 13, charTable02 + value03, 2);
		std::memcpy(buf + 15, charTable02 + cc, 2);
		std::memcpy(buf + 17, charTable02 + dd, 2);
		return buf + 19;
	}

	JSONIFIER_INLINE static string_buffer_ptr length20(string_buffer_ptr buf, const uint64_t value) noexcept {
		const uint64_t tmp	= value / 100000000;
		const uint64_t low	= value - tmp * 100000000;
		const uint64_t high = tmp / 10000;
		const uint64_t mid	= tmp - high * 10000;
		uint64_t value01	= (high * 109951163) >> 40;
		uint64_t value02	= high - value01 * 10000;
		uint64_t aa			= (value01 * 5243) >> 19;
		uint64_t value03	= value01 - aa * 100;
		uint64_t cc			= (value02 * 5243) >> 19;
		uint64_t dd			= value02 - cc * 100;
		std::memcpy(buf, charTable02 + aa, 2);
		std::memcpy(buf + 2, charTable02 + value03, 2);
		std::memcpy(buf + 4, charTable02 + cc, 2);
		std::memcpy(buf + 6, charTable02 + dd, 2);
		aa		= (mid * 5243) >> 19;
		value03 = mid - aa * 100;
		std::memcpy(buf + 8, charTable02 + aa, 2);
		std::memcpy(buf + 10, charTable02 + value03, 2);
		value01 = (low * 109951163) >> 40;
		value02 = low - value01 * 10000;
		aa		= (value01 * 5243) >> 19;
		value03 = value01 - aa * 100;
		cc		= (value02 * 5243) >> 19;
		dd		= value02 - cc * 100;
		std::memcpy(buf + 12, charTable02 + aa, 2);
		std::memcpy(buf + 14, charTable02 + value03, 2);
		std::memcpy(buf + 16, charTable02 + cc, 2);
		std::memcpy(buf + 18, charTable02 + dd, 2);
		return buf + 20;
	}

	template<jsonifier::concepts::uns64_t value_type_new> JSONIFIER_INLINE static string_buffer_ptr toChars(string_buffer_ptr buf, const value_type_new value) noexcept {
		const uint64_t index{ fastDigitCount(value) };
		switch (index) {
			case 1: {
				return length1(buf, value);
			}
			case 2: {
				return length2(buf, value);
			}
			case 3: {
				return length3(buf, value);
			}
			case 4: {
				return length4(buf, value);
			}
			case 5: {
				return length5(buf, value);
			}
			case 6: {
				return length6(buf, value);
			}
			case 7: {
				return length7(buf, value);
			}
			case 8: {
				return length8(buf, value);
			}
			case 9: {
				return length9(buf, value);
			}
			case 10: {
				return length10(buf, value);
			}
			case 11: {
				return length11(buf, value);
			}
			case 12: {
				return length12(buf, value);
			}
			case 13: {
				return length13(buf, value);
			}
			case 14: {
				return length14(buf, value);
			}
			case 15: {
				return length15(buf, value);
			}
			case 16: {
				return length16(buf, value);
			}
			case 17: {
				return length17(buf, value);
			}
			case 18: {
				return length18(buf, value);
			}
			case 19: {
				return length19(buf, value);
			}
			case 20: {
				return length20(buf, value);
			}
			default: {
				std::unreachable();
			}
		}
	}

	template<jsonifier::concepts::sig64_t value_type_new> JSONIFIER_INLINE static string_buffer_ptr toChars(string_buffer_ptr buf, const value_type_new value) noexcept {
		*buf = '-';
		return toChars(buf + (value < 0), static_cast<const uint64_t>(value ^ (value >> 63)) - (value >> 63));
	}

}// namespace jsonifier::internal