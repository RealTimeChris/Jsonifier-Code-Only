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
/// Nov 13ull, 2023
#pragma once

#include <jsonifier/Config.hpp>

#include <concepts>
#include <cstdint>
#include <cstring>
#include <limits>
#include <array>

namespace jsonifier::internal {

	enum float_round_style { round_indeterminate = -1, round_toward_zero = 0, round_to_nearest = 1, round_toward_infinity = 2, round_toward_neg_infinity = 3 };

	struct _Num_base {
		static constexpr bool has_infinity			   = false;
		static constexpr bool has_quiet_NaN			   = false;
		static constexpr bool has_signaling_NaN		   = false;
		static constexpr bool is_bounded			   = false;
		static constexpr bool is_exact				   = false;
		static constexpr bool is_iec559				   = false;
		static constexpr bool is_integer			   = false;
		static constexpr bool is_modulo				   = false;
		static constexpr bool is_signed				   = false;
		static constexpr bool is_specialized		   = false;
		static constexpr bool tinyness_before		   = false;
		static constexpr bool traps					   = false;
		static constexpr float_round_style round_style = round_toward_zero;
		static constexpr int digits					   = 0;
		static constexpr int digits10				   = 0;
		static constexpr int max_digits10			   = 0;
		static constexpr int max_exponent			   = 0;
		static constexpr int max_exponent10			   = 0;
		static constexpr int min_exponent			   = 0;
		static constexpr int min_exponent10			   = 0;
		static constexpr int radix					   = 0;
	};

	template<class value_type> class numeric_limits : public _Num_base {
	  public:
		JSONIFIER_INLINE static constexpr value_type(min)() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type(max)() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type lowest() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type epsilon() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type round_error() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type denorm_min() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type infinity() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type quiet_NaN() noexcept {
			return value_type();
		}

		JSONIFIER_INLINE static constexpr value_type signaling_NaN() noexcept {
			return value_type();
		}
	};

	template<class value_type> class numeric_limits<const value_type> : public numeric_limits<value_type> {};

	template<class value_type> class numeric_limits<volatile value_type> : public numeric_limits<value_type> {};

	template<class value_type> class numeric_limits<const volatile value_type> : public numeric_limits<value_type> {};

	struct _Num_int_base : _Num_base {
		static constexpr bool is_bounded	 = true;
		static constexpr bool is_exact		 = true;
		static constexpr bool is_integer	 = true;
		static constexpr bool is_specialized = true;
		static constexpr int radix			 = 2;
	};

	struct _Num_float_base : _Num_base {
		static constexpr bool has_infinity			   = true;
		static constexpr bool has_quiet_NaN			   = true;
		static constexpr bool has_signaling_NaN		   = true;
		static constexpr bool is_bounded			   = true;
		static constexpr bool is_iec559				   = true;
		static constexpr bool is_signed				   = true;
		static constexpr bool is_specialized		   = true;
		static constexpr float_round_style round_style = round_to_nearest;
		static constexpr int radix					   = FLT_RADIX;
	};

	template<> class numeric_limits<bool> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr bool(min)() noexcept {
			return false;
		}

		JSONIFIER_INLINE static constexpr bool(max)() noexcept {
			return true;
		}

		JSONIFIER_INLINE static constexpr bool lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr bool epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr bool round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr bool denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr bool infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr bool quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr bool signaling_NaN() noexcept {
			return 0;
		}

		static constexpr int digits = 1;
	};

	template<> class numeric_limits<char> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr char(min)() noexcept {
			return CHAR_MIN;
		}

		JSONIFIER_INLINE static constexpr char(max)() noexcept {
			return CHAR_MAX;
		}

		JSONIFIER_INLINE static constexpr char lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr char epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_signed = CHAR_MIN != 0;
		static constexpr bool is_modulo = CHAR_MIN == 0;
		static constexpr int digits		= 8 - (CHAR_MIN != 0);
		static constexpr int digits10	= 2;
	};

	template<> class numeric_limits<signed char> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr signed char(min)() noexcept {
			return SCHAR_MIN;
		}

		JSONIFIER_INLINE static constexpr signed char(max)() noexcept {
			return SCHAR_MAX;
		}

		JSONIFIER_INLINE static constexpr signed char lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr signed char epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr signed char round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr signed char denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr signed char infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr signed char quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr signed char signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_signed = true;
		static constexpr int digits		= 7;
		static constexpr int digits10	= 2;
	};

	template<> class numeric_limits<unsigned char> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr unsigned char(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned char(max)() noexcept {
			return UCHAR_MAX;
		}

		JSONIFIER_INLINE static constexpr unsigned char lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr unsigned char epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned char round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned char denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned char infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned char quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned char signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_modulo = true;
		static constexpr int digits		= 8;
		static constexpr int digits10	= 2;
	};

	template<> class numeric_limits<char16_t> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr char16_t(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char16_t(max)() noexcept {
			return USHRT_MAX;
		}

		JSONIFIER_INLINE static constexpr char16_t lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr char16_t epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char16_t round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char16_t denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char16_t infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char16_t quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char16_t signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_modulo = true;
		static constexpr int digits		= 16;
		static constexpr int digits10	= 4;
	};

	template<> class numeric_limits<char32_t> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr char32_t(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char32_t(max)() noexcept {
			return UINT_MAX;
		}

		JSONIFIER_INLINE static constexpr char32_t lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr char32_t epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char32_t round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char32_t denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char32_t infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char32_t quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr char32_t signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_modulo = true;
		static constexpr int digits		= 32;
		static constexpr int digits10	= 9;
	};

	template<> class numeric_limits<wchar_t> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr wchar_t(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr wchar_t(max)() noexcept {
			return 0xffff;
		}

		JSONIFIER_INLINE static constexpr wchar_t lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr wchar_t epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr wchar_t round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr wchar_t denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr wchar_t infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr wchar_t quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr wchar_t signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_modulo = true;
		static constexpr int digits		= 16;
		static constexpr int digits10	= 4;
	};

	template<> class numeric_limits<short> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr short(min)() noexcept {
			return SHRT_MIN;
		}

		JSONIFIER_INLINE static constexpr short(max)() noexcept {
			return SHRT_MAX;
		}

		JSONIFIER_INLINE static constexpr short lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr short epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr short round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr short denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr short infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr short quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr short signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_signed = true;
		static constexpr int digits		= 15;
		static constexpr int digits10	= 4;
	};

	template<> class numeric_limits<int> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr int(min)() noexcept {
			return INT_MIN;
		}

		JSONIFIER_INLINE static constexpr int(max)() noexcept {
			return INT_MAX;
		}

		JSONIFIER_INLINE static constexpr int lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr int epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr int round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr int denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr int infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr int quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr int signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_signed = true;
		static constexpr int digits		= 31;
		static constexpr int digits10	= 9;
	};

	template<> class numeric_limits<long> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr long(min)() noexcept {
			return LONG_MIN;
		}

		JSONIFIER_INLINE static constexpr long(max)() noexcept {
			return LONG_MAX;
		}

		JSONIFIER_INLINE static constexpr long lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr long epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long signaling_NaN() noexcept {
			return 0;
		}

		static_assert(sizeof(int) == sizeof(long), "LLP64 assumption");
		static constexpr bool is_signed = true;
		static constexpr int digits		= 31;
		static constexpr int digits10	= 9;
	};

	template<> class numeric_limits<long long> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr long long(min)() noexcept {
			return LLONG_MIN;
		}

		JSONIFIER_INLINE static constexpr long long(max)() noexcept {
			return LLONG_MAX;
		}

		JSONIFIER_INLINE static constexpr long long lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr long long epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long long round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long long denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long long infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long long quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr long long signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_signed = true;
		static constexpr int digits		= 63;
		static constexpr int digits10	= 18;
	};

	template<> class numeric_limits<unsigned int> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr unsigned int(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned int(max)() noexcept {
			return UINT_MAX;
		}

		JSONIFIER_INLINE static constexpr unsigned int lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr unsigned int epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned int round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned int denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned int infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned int quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned int signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_modulo = true;
		static constexpr int digits		= 32;
		static constexpr int digits10	= 9;
	};

	template<> class numeric_limits<unsigned long> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr unsigned long(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long(max)() noexcept {
			return ULONG_MAX;
		}

		JSONIFIER_INLINE static constexpr unsigned long lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr unsigned long epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long signaling_NaN() noexcept {
			return 0;
		}

		static_assert(sizeof(unsigned int) == sizeof(unsigned long), "LLP64 assumption");
		static constexpr bool is_modulo = true;
		static constexpr int digits		= 32;
		static constexpr int digits10	= 9;
	};

	template<> class numeric_limits<unsigned long long> : public _Num_int_base {
	  public:
		JSONIFIER_INLINE static constexpr unsigned long long(min)() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long long(max)() noexcept {
			return ULLONG_MAX;
		}

		JSONIFIER_INLINE static constexpr unsigned long long lowest() noexcept {
			return ( min )();
		}

		JSONIFIER_INLINE static constexpr unsigned long long epsilon() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long long round_error() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long long denorm_min() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long long infinity() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long long quiet_NaN() noexcept {
			return 0;
		}

		JSONIFIER_INLINE static constexpr unsigned long long signaling_NaN() noexcept {
			return 0;
		}

		static constexpr bool is_modulo = true;
		static constexpr int digits		= 64;
		static constexpr int digits10	= 19;
	};

	template<> class numeric_limits<float> : public _Num_float_base {
	  public:
		JSONIFIER_INLINE static constexpr float(min)() noexcept {
			return FLT_MIN;
		}

		JSONIFIER_INLINE static constexpr float(max)() noexcept {
			return FLT_MAX;
		}

		JSONIFIER_INLINE static constexpr float lowest() noexcept {
			return -( max )();
		}

		JSONIFIER_INLINE static constexpr float epsilon() noexcept {
			return FLT_EPSILON;
		}

		JSONIFIER_INLINE static constexpr float round_error() noexcept {
			return 0.5F;
		}

		JSONIFIER_INLINE static constexpr float denorm_min() noexcept {
			return FLT_TRUE_MIN;
		}

		JSONIFIER_INLINE static constexpr float infinity() noexcept {
			return std::numeric_limits<float>::infinity();
		}

		JSONIFIER_INLINE static constexpr float quiet_NaN() noexcept {
			return std::numeric_limits<float>::quiet_NaN();
		}

		JSONIFIER_INLINE static constexpr float signaling_NaN() noexcept {
			return std::numeric_limits<float>::signaling_NaN();
		}

		static constexpr int digits			= FLT_MANT_DIG;
		static constexpr int digits10		= FLT_DIG;
		static constexpr int max_digits10	= 9;
		static constexpr int max_exponent	= FLT_MAX_EXP;
		static constexpr int max_exponent10 = FLT_MAX_10_EXP;
		static constexpr int min_exponent	= FLT_MIN_EXP;
		static constexpr int min_exponent10 = FLT_MIN_10_EXP;
	};

	template<> class numeric_limits<double> : public _Num_float_base {
	  public:
		JSONIFIER_INLINE static constexpr double(min)() noexcept {
			return DBL_MIN;
		}

		JSONIFIER_INLINE static constexpr double(max)() noexcept {
			return DBL_MAX;
		}

		JSONIFIER_INLINE static constexpr double lowest() noexcept {
			return -( max )();
		}

		JSONIFIER_INLINE static constexpr double epsilon() noexcept {
			return DBL_EPSILON;
		}

		JSONIFIER_INLINE static constexpr double round_error() noexcept {
			return 0.5;
		}

		JSONIFIER_INLINE static constexpr double denorm_min() noexcept {
			return DBL_TRUE_MIN;
		}

		JSONIFIER_INLINE static constexpr double infinity() noexcept {
			return std::numeric_limits<double>::infinity();
		}

		JSONIFIER_INLINE static constexpr double quiet_NaN() noexcept {
			return std::numeric_limits<double>::quiet_NaN();
		}

		JSONIFIER_INLINE static constexpr double signaling_NaN() noexcept {
			return std::numeric_limits<double>::signaling_NaN();
		}

		static constexpr int digits			= DBL_MANT_DIG;
		static constexpr int digits10		= DBL_DIG;
		static constexpr int max_digits10	= 17;
		static constexpr int max_exponent	= DBL_MAX_EXP;
		static constexpr int max_exponent10 = DBL_MAX_10_EXP;
		static constexpr int min_exponent	= DBL_MIN_EXP;
		static constexpr int min_exponent10 = DBL_MIN_10_EXP;
	};

	template<> class numeric_limits<long double> : public _Num_float_base {
	  public:
		JSONIFIER_INLINE static constexpr long double(min)() noexcept {
			return LDBL_MIN;
		}

		JSONIFIER_INLINE static constexpr long double(max)() noexcept {
			return LDBL_MAX;
		}

		JSONIFIER_INLINE static constexpr long double lowest() noexcept {
			return -( max )();
		}

		JSONIFIER_INLINE static constexpr long double epsilon() noexcept {
			return LDBL_EPSILON;
		}

		JSONIFIER_INLINE static constexpr long double round_error() noexcept {
			return 0.5L;
		}

		JSONIFIER_INLINE static constexpr long double denorm_min() noexcept {
			return LDBL_TRUE_MIN;
		}

		JSONIFIER_INLINE static constexpr double infinity() noexcept {
			return std::numeric_limits<long double>::infinity();
		}

		JSONIFIER_INLINE static constexpr double quiet_NaN() noexcept {
			return std::numeric_limits<long double>::quiet_NaN();
		}

		JSONIFIER_INLINE static constexpr double signaling_NaN() noexcept {
			return std::numeric_limits<long double>::signaling_NaN();
		}

		static constexpr int digits			= LDBL_MANT_DIG;
		static constexpr int digits10		= LDBL_DIG;
		static constexpr int max_digits10	= 2 + LDBL_MANT_DIG * 301L / 1000;
		static constexpr int max_exponent	= LDBL_MAX_EXP;
		static constexpr int max_exponent10 = LDBL_MAX_10_EXP;
		static constexpr int min_exponent	= LDBL_MIN_EXP;
		static constexpr int min_exponent10 = LDBL_MIN_10_EXP;
	};

}