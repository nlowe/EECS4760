/*
 * Copyright (c) 2016 Nathan Lowe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Util.h - General utility functions
 */
#pragma once
#include <stdexcept>

inline uint64_t charToUnsigned64(char c)
{
	return 0ull | reinterpret_cast<unsigned char&>(c);
}

inline uint64_t extract64FromBuff(char* buff, size_t offset)
{
	return *(reinterpret_cast<unsigned long long*>(buff + offset));
}

template<typename T> class Optional
{
public:
	Optional() : hasValue(false)
	{
		
	}

	explicit Optional(T initialValue) : value(initialValue), hasValue(true)
	{
		
	}

	T GetValue() { if (!hasValue) { throw std::domain_error("No value set"); } return value; }
	void SetValue(T v) { value = v; hasValue = true; }
	bool HasValue() const { return hasValue; }

private:
	T value;
	bool hasValue;
};
