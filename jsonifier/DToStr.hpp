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
/// Note: Most of the code in this header was sampled from Glaze library: https://github.com/stephenberry/glaze
/// https://github.com/RealTimeChris/jsonifier
/// Nov 13, 2023
#pragma once

#include <jsonifier/Allocator.hpp>
#include <jsonifier/DragonBox.hpp>

#include <concepts>
#include <cstdint>
#include <cstring>
#include <array>

namespace jsonifier_internal {

	JSONIFIER_ALWAYS_INLINE_VARIABLE uint64_t digitCountTable[]{ 4294967296, 8589934582, 8589934582, 8589934582, 12884901788, 12884901788, 12884901788, 17179868184, 17179868184,
		17179868184, 21474826480, 21474826480, 21474826480, 21474826480, 25769703776, 25769703776, 25769703776, 30063771072, 30063771072, 30063771072, 34349738368, 34349738368,
		34349738368, 34349738368, 38554705664, 38554705664, 38554705664, 41949672960, 41949672960, 41949672960, 42949672960, 42949672960 };

	// https://lemire.me/blog/2021/06/03/computing-the-number-of-digits-of-an-integer-even-faster/
	JSONIFIER_ALWAYS_INLINE uint64_t fastDigitCount(const uint32_t x) noexcept {
		return (x + digitCountTable[31 - simd_internal::lzcnt(x | 1)]) >> 32;
	}

	JSONIFIER_ALWAYS_INLINE_VARIABLE uint8_t decTrailingZeroTable[]{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0 };

	JSONIFIER_ALWAYS_INLINE string_buffer_ptr writeU64Len15To17Trim(string_buffer_ptr buf, uint64_t sig) noexcept {
		uint32_t tz1, tz2, tz;

		uint32_t abbccddee = static_cast<uint32_t>(sig / 100000000);
		uint32_t ffgghhii  = static_cast<uint32_t>(sig - static_cast<uint64_t>(abbccddee) * 100000000);
		uint32_t abbcc	   = abbccddee / 10000;
		uint32_t ddee	   = abbccddee - abbcc * 10000;
		uint32_t abb	   = static_cast<uint32_t>((static_cast<uint64_t>(abbcc) * 167773) >> 24);
		uint32_t a		   = (abb * 41) >> 12;
		uint32_t bb		   = abb - a * 100;
		uint32_t cc		   = abbcc - abb * 100;

		buf[0] = char(a + '0');
		buf += a > 0;
		bool lz = bb < 10 && a == 0;
		std::memcpy(buf, charTable + (bb * 2 + lz), 2);
		buf -= lz;
		std::memcpy(buf + 2, charTable + 2 * cc, 2);

		if (ffgghhii) {
			uint32_t dd	  = (ddee * 5243) >> 19;
			uint32_t ee	  = ddee - dd * 100;
			uint32_t ffgg = static_cast<uint32_t>((static_cast<uint64_t>(ffgghhii) * 109951163) >> 40);
			uint32_t hhii = ffgghhii - ffgg * 10000;
			uint32_t ff	  = (ffgg * 5243) >> 19;
			uint32_t gg	  = ffgg - ff * 100;
			std::memcpy(buf + 4, charTable + 2 * dd, 2);
			std::memcpy(buf + 6, charTable + 2 * ee, 2);
			std::memcpy(buf + 8, charTable + 2 * ff, 2);
			std::memcpy(buf + 10, charTable + 2 * gg, 2);
			if (hhii) {
				uint32_t hh = (hhii * 5243) >> 19;
				uint32_t ii = hhii - hh * 100;
				std::memcpy(buf + 12, charTable + 2 * hh, 2);
				std::memcpy(buf + 14, charTable + 2 * ii, 2);
				tz1 = decTrailingZeroTable[hh];
				tz2 = decTrailingZeroTable[ii];
				tz	= ii ? tz2 : (tz1 + 2);
				buf += 16 - tz;
				return buf;
			} else {
				tz1 = decTrailingZeroTable[ff];
				tz2 = decTrailingZeroTable[gg];
				tz	= gg ? tz2 : (tz1 + 2);
				buf += 12 - tz;
				return buf;
			}
		} else {
			if (ddee) {
				uint32_t dd = (ddee * 5243) >> 19;
				uint32_t ee = ddee - dd * 100;
				std::memcpy(buf + 4, charTable + 2 * dd, 2);
				std::memcpy(buf + 6, charTable + 2 * ee, 2);
				tz1 = decTrailingZeroTable[dd];
				tz2 = decTrailingZeroTable[ee];
				tz	= ee ? tz2 : (tz1 + 2);
				buf += 8 - tz;
				return buf;
			} else {
				tz1 = decTrailingZeroTable[bb];
				tz2 = decTrailingZeroTable[cc];
				tz	= cc ? tz2 : (tz1 + tz2);
				buf += 4 - tz;
				return buf;
			}
		}
	}

	JSONIFIER_ALWAYS_INLINE string_buffer_ptr writeU32Len1To9(string_buffer_ptr buf, uint32_t value) noexcept {
		if (value < 10) {
			*buf = static_cast<char>(value + '0');
			return buf + 1;
		}

		if (value < 100) {
			std::memcpy(buf, charTable + (value * 2), 2);
			return buf + 2;
		}

		const uint64_t digits = fastDigitCount(value);

		auto* end = buf + digits;
		auto* p	  = end;
		while (value >= 100) {
			const uint32_t q = value / 100;
			const uint32_t r = value % 100;
			value			 = q;
			std::memcpy(p - 2, charTable + (r * 2), 2);
			p -= 2;
		}

		if (value < 10) {
			*--p = static_cast<char>(value + '0');
		} else {
			std::memcpy(p - 2, charTable + (value * 2), 2);
		}

		return end;
	}

	JSONIFIER_ALWAYS_INLINE consteval uint32_t numbits(uint32_t x) noexcept {
		return x < 2 ? x : 1 + numbits(x >> 1);
	}

	template<typename value_type> JSONIFIER_ALWAYS_INLINE static string_buffer_ptr toChars(string_buffer_ptr buf, value_type value) noexcept {
		static_assert(std::numeric_limits<value_type>::is_iec559);
		static_assert(std::numeric_limits<value_type>::radix == 2);
		static_assert(std::same_as<float, value_type> || std::same_as<double, value_type>);
		static_assert(sizeof(float) == 4 && sizeof(double) == 8);
		static constexpr bool isFloat = std::same_as<float, value_type>;
		using raw_type				  = std::conditional_t<std::same_as<float, value_type>, uint32_t, uint64_t>;
		static constexpr auto zeroNew = value_type(0.0);
		if (value != zeroNew) {
			using conversion_traits				   = jsonifier_jkj::dragonbox::default_float_bit_carrier_conversion_traits<value_type>;
			using format_traits					   = jsonifier_jkj::dragonbox::ieee754_binary_traits<typename conversion_traits::format, typename conversion_traits::carrier_uint>;
			static constexpr uint32_t expBitsCount = numbits(std::numeric_limits<value_type>::max_exponent - std::numeric_limits<value_type>::min_exponent + 1);
			const auto floatBits				   = jsonifier_jkj::dragonbox::make_float_bits<value_type, conversion_traits, format_traits>(value);
			const auto expBits					   = floatBits.extract_exponent_bits();
			const auto s						   = floatBits.remove_exponent_bits();

			if JSONIFIER_UNLIKELY (expBits == (uint32_t(1) << expBitsCount) - 1) {
				std::memcpy(buf, "null", 4);
				return buf + 4;
			}

			*buf = '-';
			buf += (value < zeroNew);

			if constexpr (isFloat) {
				const auto v =
					jsonifier_jkj::dragonbox::to_decimal_ex(s, expBits, jsonifier_jkj::dragonbox::policy::sign::ignore, jsonifier_jkj::dragonbox::policy::trailing_zero::remove);

				uint32_t sigDec			= uint32_t(v.significand);
				int32_t expDec			= v.exponent;
				const int32_t numDigits = int32_t(fastDigitCount(sigDec));
				int32_t dotPos			= numDigits + expDec;

				if (-6 < dotPos && dotPos <= 9) {
					if (dotPos <= 0) {
						*buf++ = '0';
						*buf++ = '.';
						while (dotPos < 0) {
							*buf++ = '0';
							++dotPos;
						}
						return writeU32Len1To9(buf, sigDec);
					} else {
						auto numEnd			  = writeU32Len1To9(buf, sigDec);
						int32_t digitsWritten = int32_t(numEnd - buf);
						if (dotPos < digitsWritten) {
							std::memmove(buf + dotPos + 1, buf + dotPos, digitsWritten - dotPos);
							buf[dotPos] = '.';
							return numEnd + 1;
						} else if (dotPos > digitsWritten) {
							std::memset(numEnd, '0', dotPos - digitsWritten);
							return buf + dotPos;
						} else {
							return numEnd;
						}
					}
				} else {
					auto end = writeU32Len1To9(buf + 1, sigDec);
					expDec += int32_t(end - (buf + 1)) - 1;
					buf[0] = buf[1];
					buf[1] = '.';
					if (end == buf + 2) {
						buf[2] = '0';
						++end;
					}
					*end = 'E';
					buf	 = end + 1;
					if (expDec < 0) {
						*buf = '-';
						++buf;
						expDec = -expDec;
					}
					expDec		= std::abs(expDec);
					uint32_t lz = expDec < 10;
					std::memcpy(buf, charTable + (expDec * 2 + lz), 2);
					return buf + 2 - lz;
				}
			} else {
				const auto v =
					jsonifier_jkj::dragonbox::to_decimal_ex(s, expBits, jsonifier_jkj::dragonbox::policy::sign::ignore, jsonifier_jkj::dragonbox::policy::trailing_zero::ignore);

				uint64_t sigDec = v.significand;
				int32_t expDec	= v.exponent;

				int32_t sigLen = 17;
				sigLen -= (sigDec < 100000000ull * 100000000ull);
				sigLen -= (sigDec < 100000000ull * 10000000ull);
				int32_t dotPos = sigLen + expDec;

				if (-6 < dotPos && dotPos <= 21) {
					if (dotPos <= 0) {
						auto numHdr = buf + (2 - dotPos);
						auto numEnd = writeU64Len15To17Trim(numHdr, sigDec);
						buf[0]		= '0';
						buf[1]		= '.';
						buf += 2;
						std::memset(buf, '0', size_t(numHdr - buf));
						return numEnd;
					} else {
						std::memset(buf, '0', 24);
						auto numHdr = buf + 1;
						auto numEnd = writeU64Len15To17Trim(numHdr, sigDec);
						std::memmove(buf, buf + 1, size_t(dotPos));
						buf[dotPos] = '.';
						return ((numEnd - numHdr) <= dotPos) ? buf + dotPos : numEnd;
					}
				} else {
					auto end = writeU64Len15To17Trim(buf + 1, sigDec);
					end -= (end == buf + 2);
					expDec += sigLen - 1;
					buf[0] = buf[1];
					buf[1] = '.';
					end[0] = 'E';
					buf	   = end + 1;
					buf[0] = '-';
					buf += expDec < 0;
					expDec = std::abs(expDec);
					if (expDec < 100) {
						uint32_t lz = expDec < 10;
						std::memcpy(buf, charTable + (expDec * 2 + lz), 2);
						return buf + 2 - lz;
					} else {
						const uint32_t hi = (uint32_t(expDec) * 656) >> 16;
						const uint32_t lo = uint32_t(expDec) - hi * 100;
						buf[0]			  = uint8_t(hi) + '0';
						std::memcpy(&buf[1], charTable + (lo * 2), 2);
						return buf + 3;
					}
				}
			}
		} else {
			*buf = '-';
			buf += (std::bit_cast<raw_type>(value) >> (sizeof(value_type) * 8 - 1));
			*buf = '0';
			return buf + 1;
		}
	}
}