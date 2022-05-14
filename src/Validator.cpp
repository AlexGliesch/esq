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
#include "Validator.h"
#include "Exceptions.h"
#include "ParseExceptions.h"
#include "InterpreterExceptions.h"

#include <ciso646>
#include <algorithm>
#include <cassert>

using namespace std;


void Validator::assert_arity(const std::string& function_name, 
							 int expected_arity, int given_arity) {
	if (given_arity != expected_arity) {
		throw ArityException(function_name, expected_arity, given_arity);
	}
}

void Validator::assert_arity(const string& function_name,
							 int expected_arity, const Cells& args) {
	assert_arity(function_name, expected_arity, args.size());
}

void Validator::assert_type(const string& function_name,
							const string& expected_type, const Cell& c) {
	if (expected_type.size() >= 2 and expected_type.front() == '['
		and expected_type.back() == ']' and c.is_empty()) {
		return;
	}
	if (c.literal_type() != expected_type) {
		throw TypeException(function_name, expected_type, c.literal_type());
	}
}

void Validator::assert_list_type(const Cell& new_element, const Cells& list) {
	if (not all_of(begin(list), end(list), [&](const Cell& c) {
		return c.literal_type() == new_element.literal_type(); })) {
		throw InterpreterException("Error: a list must have all arguments "
			"of the same type");
	}
}
