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
 * Mask.h - Common bitmasks
 */

#pragma once
#include <cstdint>

/** A 28-bit mask */
const uint32_t MASK28 = 0xFFFFFFF;
/** A 32-bit mask */
const uint64_t MASK32 = 0xFFFFFFFF;
/** A 56-bit mask */
const uint64_t MASK56 = 0xFFFFFFFFFFFFFF;

/** A 6-bit mask */
const uint8_t MASK6 = 0x3F;

/** A mask for the least significant bit */
const uint8_t MASK_LSB = 0x1;

/** A mask for the 6th bit */
const uint8_t MASK6_MSB = 0x20;
/** A mask for the middle 4 bits in a 6 bit number */
const uint8_t MASK6_MIDDLE4 = 0x1E;
