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
 * crypto.h - Public Header for DES Crypto
 */

#pragma once
#include <cstdint>
#include <string>
#include "Util.h"

#define DES_BLOCK_SIZE_BYTES 8

/**
 * A file-based implementation of the Data Encryption Standard. Both ECB and CBC modes are supported
 */
namespace DES
{
	/**
	 * The action to perform on the block
	 */
	enum Action { ENCRYPT, DECRYPT, UNKNOWN_ACTION };

	/**
	 * The Mode to operate in
	 */
	enum Mode { ECB, CBC, UNKNOWN_MODE };

	/**
	 * Encrypt the file at the specified path to the specified output path, using the provided key.
	 *
	 * If the mode is CBC, the IV should also be specified
	 */
	int EncryptFile(std::string inputFile, std::string outputFile, uint64_t key, Mode mode, Optional<uint64_t> CBCInitialVector);
	
	/**
	 * Decrypt the file at the specified path to the specified output path, using the provided key.
	 *
	 * If the mode is CBC, the IV should also be specified
	 */
	int DecryptFile(std::string inputFile, std::string outputFile, uint64_t key, Mode mode, Optional<uint64_t> CBCInitialVector);
}
