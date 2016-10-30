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
 */


#include "stdafx.h"
#include "ExitCodes.h"
#include "AVL.h"
#include <iostream>
#include <fstream>

int printStats(std::string prefix);

AVL* singleByteCount = new AVL();
AVL* digraphCount = new AVL();
AVL* trigraphCount = new AVL();
AVL* blockCounter = new AVL();

int cleanup(int exitCode)
{
	delete singleByteCount;
	delete digraphCount;
	delete trigraphCount;
	delete blockCounter;

	return exitCode;
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cerr << "syntax: fstats <file> <output prefix>" << std::endl;
		return EXIT_ERR_SYNTAX;
	}

	std::cout << "Collecting stats on '" << argv[1] << "' to '" << argv[2] << "'" << std::endl;

	std::ifstream reader;
	reader.open(argv[1], std::ios::binary | std::ios::ate | std::ios::in);

	if(!reader.good())
	{
		std::cerr << "Unable to open file for read: " << argv[1] << std::endl;
		return cleanup(EXIT_ERR_BAD_INPUT);
	}

	auto hasGrandparentByte = false;
	auto hasParentByte = false;
	uint8_t grandparentByte=0, parentByte=0, byte=0;
	uint64_t block = 0;

	uint8_t blockByteCount = 0;

	auto len = reader.tellg();
	auto bytes = new char[len]{ 0 };
	reader.seekg(0, std::ios::beg);
	reader.read(bytes, len);

	auto currentByte = 0;
	while(currentByte < len)
	{
		byte = bytes[currentByte++];
		singleByteCount->add(byte);

		if(hasGrandparentByte)
		{
			// trigraph analysis
			trigraphCount->add(((0ull | grandparentByte) << 16) | ((0ull | parentByte) << 8) | byte);
		}

		if(hasParentByte)
		{
			// digraph analysis
			digraphCount->add((0ul | parentByte) << 8 | byte);
			grandparentByte = parentByte;
			hasGrandparentByte = true;
		}

		block <<= 8;
		block |= byte;
		blockByteCount++;

		if(blockByteCount == 8)
		{
			// Block level analysis
			blockCounter->add(block);
			block = blockByteCount = 0;
		}

		hasParentByte = true;
		parentByte = byte;
	}

	reader.close();
	delete[] bytes;
	return cleanup(printStats(std::string(argv[2])));
}

int printStats(std::string prefix)
{
	std::ofstream singleByteWriter, digraphWriter, trigraphWriter, blockWriter;
	singleByteWriter.open(prefix + ".single.tsv", std::ios::out);
	digraphWriter.open(prefix + ".digraph.tsv", std::ios::out);
	trigraphWriter.open(prefix + ".trigraph.tsv", std::ios::out);
	blockWriter.open(prefix + ".blocks.tsv", std::ios::out);

	if (singleByteWriter.bad()) return EXIT_ERR_BAD_OUTPUT;
	if (digraphWriter.bad()) return EXIT_ERR_BAD_OUTPUT;
	if (trigraphWriter.bad()) return EXIT_ERR_BAD_OUTPUT;
	if (blockWriter.bad()) return EXIT_ERR_BAD_OUTPUT;

	singleByteCount->inOrderPrint(singleByteWriter);
	digraphCount->inOrderPrint(digraphWriter);
	trigraphCount->inOrderPrint(trigraphWriter);
	blockCounter->inOrderPrint(blockWriter);

	singleByteWriter.flush();
	singleByteWriter.close();

	digraphWriter.flush();
	digraphWriter.close();

	trigraphWriter.flush();
	trigraphWriter.close();

	blockWriter.flush();
	blockWriter.close();

	return EXIT_SUCCESS;
}
