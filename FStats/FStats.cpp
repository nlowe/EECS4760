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
#include <iostream>
#include <fstream>

void printStats(std::ofstream &writer);

size_t singleByteCount[0xFF] = { 0 };
size_t digraphCount[0xFFFF] = { 0 };
size_t trigraphCount[0xFFFFFF] = { 0 };

// TODO: Tree for counting unique blocks

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cerr << "syntax: fstats <file> <output.tsv>" << std::endl;
		return EXIT_ERR_SYNTAX;
	}

	std::cout << "Collecting stats on '" << argv[1] << "' to '" << argv[2] << "'" << std::endl;

	std::ifstream reader;
	reader.open(argv[1], std::ios::binary | std::ios::in);

	if(!reader.good())
	{
		std::cerr << "Unable to open file for read: " << argv[1] << std::endl;
		return EXIT_ERR_BAD_INPUT;
	}

	std::ofstream writer;
	writer.open(argv[2], std::ios::out);

	if(!writer.good())
	{
		std::cerr << "Unable to open file for write: " << argv[2] << std::endl;
		return EXIT_ERR_BAD_OUTPUT;
	}

	auto hasGrandparentByte = false;
	auto hasParentByte = false;
	uint8_t grandparentByte=0, parentByte=0, byte=0;
	uint64_t block = 0;

	uint8_t blockByteCount = 0;

	while(!reader.eof())
	{
		reader.read(reinterpret_cast<char*>(&byte), 1);

		singleByteCount[byte]++;

		if(hasGrandparentByte)
		{
			// trigraph analysis
			trigraphCount[((0ull | grandparentByte) << 16) | ((0ull | parentByte) << 8) | byte]++;
		}

		if(hasParentByte)
		{
			// digraph analysis
			digraphCount[(0ul | parentByte) << 8 | byte]++;
			grandparentByte = parentByte;
			hasGrandparentByte = true;
		}

		block <<= 8;
		block |= byte;
		blockByteCount++;

		if(blockByteCount == 8)
		{
			// TODO: block analysis
			block = blockByteCount = 0;
		}
	
		hasParentByte = true;
		parentByte = byte;
	}

	reader.close();
	printStats(writer);

	writer.flush();
	writer.close();

	return EXIT_SUCCESS;
}

void printCount(std::ofstream &writer, const size_t b[], size_t count)
{
	for(size_t i = 0; i < count; i++)
	{
		if (b[i] == 0) continue;
		writer << std::uppercase << std::hex << i;
		writer << "\t" << std::dec << b[i] << std::endl;
	}
}

void printStats(std::ofstream &writer)
{
	writer << "# Byte Stats" << std::endl;
	printCount(writer, singleByteCount, 0xFF);

	writer << std::endl << std::endl << "# Digraph Stats" << std::endl;
	printCount(writer, digraphCount, 0xFFFF);

	writer << std::endl << std::endl << "# Trigraph Stats" << std::endl;
	printCount(writer, trigraphCount, 0xFFFFFF);

	writer << std::endl << std::endl << "# Block Stats" << std::endl;
}
