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

#if !defined(NOMINMAX)
	#define NOMINMAX
#endif

#include <jsonifier/Parsing/Parse_Impl.hpp>
#include <jsonifier/Serializing/Serialize_Impl.hpp>
#include <jsonifier/Core/JsonifierCore.hpp>
#include <jsonifier/Utilities/RawJsonData.hpp>
#include <jsonifier/Serializing/Prettifier.hpp>
#include <jsonifier/Serializing/Serializer.hpp>
#include <jsonifier/Serializing/Minifier.hpp>
#include <jsonifier/Utilities/HashMap.hpp>
#include <jsonifier/Parsing/Parser.hpp>
#include <jsonifier/Utilities/String.hpp>
#include <jsonifier/Utilities/Simd.hpp>
