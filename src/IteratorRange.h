/*
* MIT License
* 
* Copyright (c) 2013 Alex Gliesch
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
#pragma once
#include <utility>
#include "FakeIterator.h"


template<typename T>
class IteratorRange {
public:
	IteratorRange(const T& first, const T& last) 
		: first_(first)
		, last_(last) {			
		}

	T begin() { return first_; }
	T end() { return last_; }

private:
	T first_;
	T last_;
};

template<typename T>
IteratorRange<T> iterator_range(const std::pair<T, T>& range) {
	return IteratorRange<T>(range.first, range.second);
}

template<typename T>
IteratorRange<T> iterator_range(const T& first, const T& last) {
	return IteratorRange<T>(first, last);
}

inline IteratorRange<FakeIterator<int>> range(int first, int last) {
	return iterator_range(FakeIterator<int>(first), 
	                      FakeIterator<int>(last));
}

