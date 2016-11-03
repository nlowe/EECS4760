/*
 * Built for EECS2510 - Nonlinear Data Structures
 *	at The University of Toledo, Spring 2016
 *
 * Revised for EECS4760
 *
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
 * MaxPriorityQueue.h an implementation of a Max Priority Queue, specifically designed to work with
 *		pairs of 64-bit blocks and counts
 *
 */
#pragma once
#include <stdexcept>
#include <functional>

#include "utils.h"

// A maximum priority queue implemented with a maximum binary heap
class MaxPriorityQueue
{
public:
	explicit MaxPriorityQueue(size_t initialCapacity) : size(0), capacity(initialCapacity)
	{
		elements = new std::pair<uint64_t, size_t>*[capacity + 1]{ nullptr };
	}

	~MaxPriorityQueue()
	{
		delete[] elements;
	}

	// Add the specified element to the queue
	void enqueue(std::pair<uint64_t, size_t>* element)
	{
		if (size > capacity - 1) throw std::invalid_argument("Specified size too small");

		elements[++size] = element;
		auto i = size;
		while(i > 1 && elements[parentOf(i)]->second < elements[i]->second)
		{
			utils::swap(elements[i], elements[parentOf(i)]);
			i = parentOf(i);
		}
	}

	// Remove and return the maximum element from the queue
	std::pair<uint64_t, size_t>* dequeue()
	{
		if (size == 0) throw std::underflow_error("Nothing in the heap");

		auto max = elements[1];
		
		elements[1] = elements[size];
		elements[size--] = nullptr;
		maxHeapify(1);

		return max;
	}

	// Returns true iff the specified element is in the queue
	bool contains(std::pair<uint64_t, size_t>* k) const
	{
		for(auto i = 1; i <= size; i++)
		{
			if (elements[i] == k) return true;
		}
		return false;
	}

	// Apply the specified function to all elements in the queue
	void each(std::function<void(std::pair<uint64_t, size_t>*)> action) const
	{
		for(size_t i = 1; i <= size; i++)
		{
			action(elements[i]);
		}
	}

	// Returns true iff the queue contains no elements
	bool isEmpty() const { return size == 0; }

	// Returns the number of elements in the queue
	size_t Size() const	{ return size; }
	// Returns the maximum number of elements in the queue
	size_t Capacity() const	{ return capacity; }
private:

	size_t size;
	size_t capacity;
	std::pair<uint64_t, size_t>** elements;

	// Returns the index of the parent of the element at the specified index
	static size_t parentOf(size_t index) { return floor(index / 2); }
	// Returns the index of the left child of the element at the specified index
	static size_t leftOf(size_t index) { return 2 * index; }
	// Returns the index of the right child of the element at the specified index
	static size_t rightOf(size_t index) { return 2 * index + 1; }

	// Ensures that the subtree at the specified index is a max-heap
	void maxHeapify(size_t index) const
	{
		auto l = leftOf(index);
		auto r = rightOf(index);
		size_t largest = 0;

		if ( l <= size && elements[l]->second > elements[index]->second)
		{
			largest = l;
		}
		else
		{
			largest = index;
		}

		if ( r <= size && elements[r]->second > elements[largest]->second)
		{
			largest = r;
		}

		if (largest != index)
		{
			utils::swap(elements[index], elements[largest]);
			maxHeapify(largest);
		}
	}
};