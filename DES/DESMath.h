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
 * DESMath.h - Common math operations for DES
 */

#pragma once
#include <cstdint>
#include "Mask.h"
#include <random>
#include "Util.h"

/**
 * Generates a random 32-bit unsigned integer (Half a block)
 */
inline uint32_t RandomHalfBlock()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	std::uniform_int_distribution<uint32_t> half;
	return half(gen);
}

/**
 * Generates a random 64-bit unsigned integer (Full Block)
 */
inline uint64_t RandomBlock()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	std::uniform_int_distribution<uint64_t> full;
	return full(gen);
}

/**
 * Splits the specified 56 bit number into a two 28 bit halves.
 * The 8 most significant bits of the input are ignored, and the
 * 4 most significant bits of the output are 0.
 */
inline void split56(uint64_t in, uint64_t& left, uint64_t& right)
{
	left = (in >> 28) & MASK28;
	right = in & MASK28;
}

/**
 * Joins the two 28-bit halves into a 56-bit output
 */
inline uint64_t join56(uint64_t left, uint64_t right)
{
	return (left & MASK28) << 28 | (right & MASK28);
}

/**
* Splits the specified 64 bit number into a two 32 bit halves.
*/
inline void split64(uint64_t in, uint64_t& left, uint64_t& right)
{
	left = in >> 32;
	right = in & MASK28;
}

/**
* Joins the two 32-bit halves into a 64-bit output
*/
inline uint64_t join64(uint64_t left, uint64_t right)
{
	return left << 31 | right;
}

/**
 * Rotates the input left by the specified number of places under a 28bit mask
 */
inline void rotL28(uint64_t& in, uint64_t places)
{
	in = (in << places | in >> (28 - places)) & MASK28;
}

/**
 * Rotates the input right by the specified number of places under a 28bit mask
 */
inline void rotR28(uint64_t& in, uint64_t places)
{
	in = (in >> places | in << (28 - places)) & MASK28;
}

/**
 * Extracts the 6-bit group for the specified s-box (1-8)
 */
inline uint8_t extract6(uint64_t bits, uint8_t group)
{
	return (bits >> (6 * (8 - group))) & MASK6;
}

/**
 * Calculates the row of the SBox for the specified input
 */
inline uint8_t srow(uint8_t bits)
{
	return (bits & MASK6_MSB) >> 4 | (bits & MASK_LSB);
}

/**
 * Calculates the column of the SBox for the specified input
 */
inline uint8_t scol(uint8_t bits)
{
	return (bits & MASK6_MIDDLE4) >> 1;
}
