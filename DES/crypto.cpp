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
 * crypto.cpp - An implementation of DES in ECB mode
 */

#include "stdafx.h"
#include "crypto.h"
#include "Boxes.h"
#include "DESMath.h"

inline uint64_t permute(uint64_t in, const uint8_t* table, size_t inputSize, size_t outputSize)
{
	uint64_t out = 0;

	for(size_t i = 0; i < outputSize; i++)
	{
		out |= ((in >> (inputSize - table[outputSize - 1 - i])) & 1) << i;
	}

	return out;
}

inline uint32_t substitute(uint64_t in)
{
	auto b1 = extract6(in, 1);
	auto b2 = extract6(in, 2);
	auto b3 = extract6(in, 3);
	auto b4 = extract6(in, 4);
	auto b5 = extract6(in, 5);
	auto b6 = extract6(in, 6);
	auto b7 = extract6(in, 7);
	auto b8 = extract6(in, 8);

	return S[0][srow(b1)][scol(b1)] << 28 |
		   S[1][srow(b2)][scol(b2)] << 24 |
		   S[2][srow(b3)][scol(b3)] << 20 |
		   S[3][srow(b4)][scol(b4)] << 16 |
		   S[4][srow(b5)][scol(b5)] << 12 |
		   S[5][srow(b6)][scol(b6)] << 8  |
		   S[6][srow(b7)][scol(b7)] << 4  |
		   S[7][srow(b8)][scol(b8)];
}

inline void rotateKey(uint64_t& key)
{
	uint32_t left, right;
	split56(key, left, right);
}

inline uint64_t computeRoundKey(uint32_t& left, uint32_t& right, uint8_t round)
{
	rotL28(left, RotationSchedule[round]);
	rotL28(right, RotationSchedule[round]);

	return permute(join56(left, right), KeyPC56To48, 56, 48);
}

void init(uint64_t key, uint32_t& left, uint32_t& right)
{
	split56(permute(key, KeyPC64To56, 64, 56), left, right);
}

uint64_t DES::TransformBlock(uint64_t block, uint64_t key, DES::Mode mode)
{
	// Initialize the key
	//   1. Compress and Permute the key into 56 bits
	//   2. Split the key into two 28 bit halves
	uint32_t keyLeft, keyRight;
	init(key, keyLeft, keyRight);

	// Perform the initial permutation on the plaintext
	uint64_t permutedBlock = permute(block, InitalBlockPermutation, 64, 64);
	
	// Split the plaintext into 32 bit left and right halves
	uint32_t left, right;

	// Perform the initial permutation
	split64(permutedBlock, left, right);

	// Precompute round keys
	uint64_t keys[16];
	for(auto i = 0; i < 16; i++)
	{
		keys[i] = computeRoundKey(keyLeft, keyRight, i);
	}

	// 16 fistel rounds
	for(auto i = 0; i < 16; i++)
	{
		// Expand and permute the right half of the block to 48 bits
		auto expandedRightHalf = permute(right, BlockPE32To48, 32, 48);

		// XOR with the round key
		expandedRightHalf ^= keys[mode == ENCRYPT ? i : 16-i];

		// Substitute via S-Boxes
		auto substituted = substitute(expandedRightHalf);

		// Perform the final permutation
		auto ciphertext = permute(substituted, BlockP32, 32, 32) & MASK32;

		// XOR with the left half
		ciphertext ^= left;

		// Swap the half-blocks for the next round
		left = right;
		right = ciphertext;
	}

	return permute(join64(left, right), FinalBlockPermutation, 64, 64);
}
