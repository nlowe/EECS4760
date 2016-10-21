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
 * DES.cpp - The main entrypoint for the program
 */


#include "stdafx.h"
#include "opts.h"
#include <fstream>
#include "ExitCodes.h"

// Forward-declare so main is at the top as per project spec
void printHelp();

int main(int argc, char* argv[])
{
	srand(time(nullptr));
	Options opts(argc, argv);

	if(opts.Errors)
	{
		printHelp();
		return EXIT_ERR_SYNTAX;
	}

	if(opts.Action == DES::Action::ENCRYPT)
	{
		return DES::EncryptFile(opts.Input, opts.Output, opts.Key, opts.Mode, opts.IV);
	}
	else if(opts.Action == DES::Action::DECRYPT)
	{
		return DES::DecryptFile(opts.Input, opts.Output, opts.Key, opts.Mode, opts.IV);
	}
	else
	{
		return EXIT_ERR_ACTION;
	}
}

void printHelp()
{
	std::cout << "DES <action> <key> <mode> <in> <out>" << std::endl << std::endl;

	std::cout << "\tAction: -e: encrypt, -d: decrypt" << std::endl;
	std::cout << "\tKey:    an 8-byte hex or ascii sequence (16 hex digits or 8 characters)" << std::endl;
	std::cout << "\t        Non-hex literals should be surrounded in single quotes" << std::endl;
	std::cout << "\t        If the key contains spaces, surround additionally with double quotes" << std::endl;
	std::cout << "\tMode:   CBC or ECB" << std::endl;
	std::cout << "\tIn:     The path to the input file" << std::endl;
	std::cout << "\tOut:    The path to the output file" << std::endl;
}
