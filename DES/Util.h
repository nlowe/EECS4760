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

/**
 * Expand th especified char to a uint64_t
 */
inline uint64_t charToUnsigned64(char c)
{
	return 0ull | reinterpret_cast<unsigned char&>(c);
}

/**
 * Extract a uint64_t from the specified char buffer
 */
inline uint64_t extract64FromBuff(char* buff, size_t offset)
{
	// Windows is LE. Since that's the only platform we support, always swap the byte order
	return _byteswap_uint64(*reinterpret_cast<unsigned long long*>(buff + offset));
}

/**
 * An optional element
 */
template<typename T> class Optional
{
public:
	Optional() : hasValue(false)
	{
		
	}

	explicit Optional(T initialValue) : value(initialValue), hasValue(true)
	{
		
	}

	/**
	 * Gets the value. If no value has been set, throws std::domain_error
	 */
	T GetValue() { if (!hasValue) { throw std::domain_error("No value set"); } return value; }
	
	/**
	 * Sets the value
	 */
	void SetValue(T v) { value = v; hasValue = true; }

	/**
	 * Returns true iff a value has been set
	 */
	bool HasValue() const { return hasValue; }

private:
	T value;
	bool hasValue;
};
