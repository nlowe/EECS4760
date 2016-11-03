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
 * opts.h - Command Line Options
 */

#pragma once
#include "crypto.h"
#include "DESMath.h"
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <regex>

/**
 * The project spec does not specify a means for using a different IV, only that this one
 * should be used. I guess recompile with a different IV if you need a different one
 */
#define DEFAULT_IV 0xFB3C718924605AED

/**
 * A class for parsing command-line options
 */
class Options
{
public:
	/** The action to perform */
	DES::Action Action = DES::Action::UNKNOWN_ACTION;
	/** The mode to operate in */
	DES::Mode Mode = DES::Mode::UNKNOWN_MODE;

	/** The key to use */
	uint64_t Key = 0;
	/** The path to the input file */
	std::string Input;
	/** The path to the output file */
	std::string Output;

	/** The initialization vector to use in CBC Mode. Not specified for ECB mode */
	Optional<uint64_t> IV;

	/** Whether or not errors were encountered */
	bool Errors = false;

	/**
	 * Construct the options using the specified arguments
	 */
	Options(int argc, char* argv[])
	{
		if(argc != 6)
		{
			Errors = true;
			return;
		}

		std::string actionFlag(argv[1]);
		std::transform(actionFlag.begin(), actionFlag.end(), actionFlag.begin(), ::tolower);

		if(actionFlag == "-e")
		{
			Action = DES::Action::ENCRYPT;
		}
		else if(actionFlag == "-d")
		{
			Action = DES::Action::DECRYPT;
		}
		else
		{
			std::cerr << "Unknown action " << actionFlag << std::endl;
			Errors = true;
			return;
		}

		std::string key(argv[2]);
		if(key.length() == 16 && std::regex_match(key, std::regex("^[0-9a-fA-F]{16}$")))
		{
			// Hex Digits
			Key = strtoull(key.c_str(), static_cast<char **>(nullptr), 16);
		}
		else if(key.length() == 8)
		{
			// ASCII characters
			Key |= charToUnsigned64(argv[2][0]) << 56;
			Key |= charToUnsigned64(argv[2][1]) << 48;
			Key |= charToUnsigned64(argv[2][2]) << 40;
			Key |= charToUnsigned64(argv[2][3]) << 32;
			Key |= charToUnsigned64(argv[2][4]) << 24;
			Key |= charToUnsigned64(argv[2][5]) << 16;
			Key |= charToUnsigned64(argv[2][6]) << 8;
			Key |= charToUnsigned64(argv[2][7]);
		}
		else if(key.length() == 10)
		{
			// ASCII w/ space, because you totally need an extra set of quotes...
			Key |= charToUnsigned64(argv[2][1]) << 56;
			Key |= charToUnsigned64(argv[2][2]) << 48;
			Key |= charToUnsigned64(argv[2][3]) << 40;
			Key |= charToUnsigned64(argv[2][4]) << 32;
			Key |= charToUnsigned64(argv[2][5]) << 24;
			Key |= charToUnsigned64(argv[2][6]) << 16;
			Key |= charToUnsigned64(argv[2][7]) << 8;
			Key |= charToUnsigned64(argv[2][8]);
		}
		else
		{
			std::cerr << "Malformed key" << std::endl;
			Errors = true;
			return;
		}

		std::string mode(argv[3]);
		std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

		if(mode == "ecb")
		{
			Mode = DES::Mode::ECB;
		}
		else if(mode == "cbc")
		{
			Mode = DES::Mode::CBC;
			IV.SetValue(DEFAULT_IV);
		}
		else
		{
			std::cerr << "Unrecognized mode: " << mode << std::endl;
			Errors = true;
			return;
		}

		Input = std::string(argv[4]);
		Output = std::string(argv[5]);
	}
};
