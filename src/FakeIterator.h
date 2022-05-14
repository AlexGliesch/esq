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

#include <type_traits>
#include <ciso646>

template<typename T, typename = void>
struct FakeIterator;

template<typename T>
struct FakeIterator<T, typename std::enable_if<!std::is_pod<T>::value>::type> 
	: public T {

	FakeIterator() { };	
	FakeIterator(const T& t) : T(t) { }
	FakeIterator(T&& t) : T(std::move(t)) { }

	T& operator*() { return *this; }
	T* operator->() { return this; }
};

template<typename T>
struct FakeIterator<T, typename std::enable_if<
				std::is_pod<T>::value and std::is_integral<T>::value>::type> {
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef int difference_type;
	
	FakeIterator() { };	
	FakeIterator(T t) : t_(t) { }

	T operator*() const { return t_; }
	operator T() const { return t_; }

	FakeIterator& operator++() { ++t_; return *this; }
	FakeIterator operator++(int) { 
		FakeIterator f(*this); operator++(); return f; }

	bool operator==(const FakeIterator& f) const { return f.t_ == t_; }
	bool operator!=(const FakeIterator& f) const { return f.t_ != t_; }

private:
	T t_;
};
